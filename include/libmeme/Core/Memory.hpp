#ifndef _ML_MEMORY_HPP_
#define _ML_MEMORY_HPP_

#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/Singleton.hpp>

// TEST RESOURCE
namespace ml::util
{
	// passthrough memory resource for collecting metrics
	struct test_resource final : public pmr::memory_resource, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using pointer			= typename byte_t *;
		using const_pointer		= typename byte_t const *;
		using reference			= typename byte_t &;
		using const_reference	= typename byte_t const &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit test_resource(pmr::memory_resource * u, pointer const b, size_t s) noexcept
			: m_upstream{ u }, m_buffer{ b }, m_total_bytes{ s }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pmr::memory_resource * upstream() const noexcept { return m_upstream; }

		pointer const buffer() noexcept { return m_buffer; }

		const_pointer const buffer() const noexcept { return m_buffer; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool has_buffer() const noexcept { return m_buffer && (0 < m_total_bytes); }

		bool has_upstream() const noexcept { return m_upstream; }

		bool is_default() const noexcept { return (this == pmr::get_default_resource()); }

		bool is_valid_size() const noexcept { return (0 < m_total_bytes); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		size_t addr() const noexcept { return (size_t)m_buffer; }

		size_t num_allocations() const noexcept { return m_num_alloc; }

		size_t capacity() const noexcept { return m_total_bytes; }

		size_t used_bytes() const noexcept { return m_bytes_used; }

		size_t free_bytes() const noexcept { return m_total_bytes - m_bytes_used; }

		float_t fraction_used() const noexcept { return (float_t)m_bytes_used / (float_t)m_total_bytes; }

		float_t percent_used() const noexcept { return fraction_used() * 100.f; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		reference front() & noexcept { return m_buffer[0]; }

		const_reference front() const & noexcept { return m_buffer[0]; }

		reference back() & noexcept { return m_buffer[m_total_bytes - 1]; }

		const_reference back() const & noexcept { return m_buffer[m_total_bytes - 1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pointer const begin() noexcept { return m_buffer; }

		const_pointer const begin() const noexcept { return m_buffer; }

		const_pointer const cbegin() const noexcept { return begin(); }

		pointer const end() noexcept { return m_buffer + m_total_bytes; }

		const_pointer const end() const noexcept { return m_buffer + m_total_bytes; }

		const_pointer const cend() const noexcept { return end(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		void * do_allocate(size_t bytes, size_t align) override
		{
			++m_num_alloc;
			m_bytes_used += bytes;
			return m_upstream->allocate(bytes, align);
		}

		void do_deallocate(void * ptr, size_t bytes, size_t align) override
		{
			--m_num_alloc;
			m_bytes_used -= bytes;
			return m_upstream->deallocate(ptr, bytes, align);
		}

		bool do_is_equal(pmr::memory_resource const & value) const noexcept override
		{
			return m_upstream->is_equal(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		pmr::memory_resource * m_upstream;
		pointer const m_buffer;
		size_t const m_total_bytes;

		size_t m_num_alloc {};
		size_t m_bytes_used {};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// MEMORY MANAGER
namespace ml
{
	struct ML_CORE_API memory_manager final : singleton<memory_manager>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		struct ML_NODISCARD record final
		{
			size_t index; size_t size; byte_t * data;
		};

		using record_map = typename ds::flat_map<void *, record>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// setup
		ML_NODISCARD static bool set_test_resource(util::test_resource * value) noexcept
		{
			static auto & inst{ get_instance() };

			if (inst.m_testres)				{ return debug::log::error("resource already set"); }
			if (!value)						{ return debug::log::error("resource cannot be null"); }
			if (!value->upstream())			{ return debug::log::error("resource upstream cannot be null"); }
			if (!value->buffer())			{ return debug::log::error("resource data cannot be null"); }
			if (!value->is_valid_size())	{ return debug::log::error("resource size must be greater than zero"); }
			if (!value->is_default())		{ return debug::log::error("resource is not the default resource"); }

			return (inst.m_testres = value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// malloc
		ML_NODISCARD static void * allocate(size_t size) noexcept
		{
			static auto & inst{ get_instance() };

			// allocate the requested bytes
			auto const temp{ inst.m_allocator.allocate(size) };

			// insert a new record
			return *inst.m_records.insert(temp, record{ inst.m_index++, size, temp }).first;
		}

		// malloc (template size)
		template <size_t Size
		> ML_NODISCARD static void * allocate() noexcept
		{
			return allocate(Size);
		}

		// malloc (template type)
		template <class Type
		> ML_NODISCARD static void * allocate() noexcept
		{
			return allocate(sizeof(Type));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// calloc
		ML_NODISCARD static void * allocate(size_t count, size_t size) noexcept
		{
			return std::memset(allocate(count * size), 0, count * size);
		}

		// calloc (template size)
		template <size_t Count, size_t Size
		> ML_NODISCARD static void * allocate() noexcept
		{
			return allocate(Count, Size);
		}

		// calloc (template type)
		template <size_t Count, class Type
		> ML_NODISCARD static void * allocate() noexcept
		{
			return allocate(Count, sizeof(Type));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// free
		static void deallocate(void * addr) noexcept
		{
			static auto & inst{ get_instance() };

			// find the record
			if (auto const it{ inst.m_records.find(addr) })
			{
				// free the allocation
				inst.m_allocator.deallocate(it->second->data, it->second->size);

				// erase the record
				inst.m_records.erase(it->first);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// realloc
		ML_NODISCARD static void * reallocate(void * addr, size_t size) noexcept
		{
			return reallocate(addr, size, size);
		}

		// realloc (sized)
		ML_NODISCARD static void * reallocate(void * addr, size_t oldsz, size_t newsz) noexcept
		{
			if (!newsz)
			{
				deallocate(addr);
				return nullptr;
			}
			else if (!addr)
			{
				return allocate(newsz);
			}
			else if (newsz <= oldsz)
			{
				return addr;
			}
			else
			{
				auto const temp{ allocate(newsz) };
				if (temp)
				{
					std::memcpy(temp, addr, oldsz);
					deallocate(addr);
				}
				return temp;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto const & get_allocator() noexcept { return get_instance().m_allocator; }

		ML_NODISCARD static auto const & get_index() noexcept { return get_instance().m_index; }

		ML_NODISCARD static auto const & get_records() noexcept { return get_instance().m_records; }

		ML_NODISCARD static auto const & get_test_resource() noexcept { return get_instance().m_testres; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<memory_manager>;

		~memory_manager();

		allocator_type			m_allocator	{};	// allocator
		size_t					m_index		{};	// record index
		record_map				m_records	{};	// record table
		util::test_resource *	m_testres	{};	// test resource

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// TRACKABLE
namespace ml
{
	struct ML_CORE_API trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~trackable() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD void * operator new(size_t size) noexcept
		{
			return memory_manager::allocate(size);
		}

		ML_NODISCARD void * operator new[](size_t size) noexcept
		{
			return memory_manager::allocate(size);
		}

		void operator delete(void * addr) noexcept
		{
			return memory_manager::deallocate(addr);
		}

		void operator delete[](void * addr) noexcept
		{
			return memory_manager::deallocate(addr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_MEMORY_HPP_