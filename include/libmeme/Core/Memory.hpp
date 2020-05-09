#ifndef _ML_MEMORY_HPP_
#define _ML_MEMORY_HPP_

#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/Singleton.hpp>

// memory_manager singleton
#define ML_memory_manager \
	_ML memory_manager::get_instance()

namespace ml::util
{
	// passthrough resource for collecting upstream usage metrics
	struct test_resource final : public pmr::memory_resource, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using pointer			= typename byte_t *;
		using const_pointer		= typename byte_t const *;
		using reference			= typename byte_t &;
		using const_reference	= typename byte_t const &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit test_resource(pmr::memory_resource * u, pointer const b, size_t c) noexcept
			: m_upstream{ u }, m_buffer{ b }, m_capacity{ c }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pmr::memory_resource * upstream() const noexcept { return m_upstream; }

		pointer const buffer() noexcept { return m_buffer; }

		const_pointer const buffer() const noexcept { return m_buffer; }

		bool is_valid_size() const noexcept { return (0 < m_capacity); }

		bool is_default() const noexcept { return (this == pmr::get_default_resource()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		size_t addr() const noexcept { return (size_t)m_buffer; }

		size_t num_allocations() const noexcept { return m_num_alloc; }

		size_t capacity() const noexcept { return m_capacity; }

		size_t used_bytes() const noexcept { return m_bytes_used; }

		size_t free_bytes() const noexcept { return m_capacity - m_bytes_used; }

		float_t fraction_used() const noexcept { return (float_t)m_bytes_used / (float_t)m_capacity; }

		float_t percent_used() const noexcept { return fraction_used() * 100.f; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		reference front() & noexcept { return m_buffer[0]; }

		const_reference front() const & noexcept { return m_buffer[0]; }

		reference back() & noexcept { return m_buffer[m_capacity - 1]; }

		const_reference back() const & noexcept { return m_buffer[m_capacity - 1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pointer const begin() noexcept { return m_buffer; }

		const_pointer const begin() const noexcept { return m_buffer; }

		const_pointer const cbegin() const noexcept { return begin(); }

		pointer const end() noexcept { return m_buffer + m_capacity; }

		const_pointer const end() const noexcept { return m_buffer + m_capacity; }

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
		size_t const m_capacity;

		size_t m_num_alloc {};
		size_t m_bytes_used {};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

namespace ml
{
	// global memory manager
	struct ML_CORE_API memory_manager final : singleton<memory_manager>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_NODISCARD record final
		{
			size_t index; size_t size; byte_t * data;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto const & get_allocator() noexcept { return get_instance().m_allocator; }

		ML_NODISCARD static auto const & get_index() noexcept { return get_instance().m_index; }

		ML_NODISCARD static auto const & get_records() noexcept { return get_instance().m_records; }

		ML_NODISCARD static auto const & get_testres() noexcept { return get_instance().m_testres; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// set test resource
		ML_NODISCARD static bool configure(util::test_resource * res) noexcept
		{
			static auto & inst{ get_instance() };

			if (!res)					{ return debug::error("resource cannot be null"); }
			if (!res->upstream())		{ return debug::error("resource upstream cannot be null"); }
			if (!res->buffer())			{ return debug::error("resource data cannot be null"); }
			if (!res->is_valid_size())	{ return debug::error("resource size must be greater than zero"); }
			if (!res->is_default())		{ return debug::error("resource is not the default resource"); }

			return (inst.m_testres = res);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// malloc
		ML_NODISCARD static void * allocate(size_t size) noexcept
		{
			static auto & inst{ get_instance() };

			// allocate the requested bytes
			auto const addr{ inst.m_allocator.allocate(size) };

			// create record
			return *inst.m_records.insert(addr, record{ inst.m_index++, size, addr }).first;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// calloc
		ML_NODISCARD static void * allocate(size_t count, size_t size) noexcept
		{
			// allocate (count*size) zeroed bytes
			return std::memset(allocate(count * size), 0, count * size);
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// realloc (sized)
		ML_NODISCARD static void * reallocate(void * addr, size_t oldsz, size_t newsz) noexcept
		{
			if (newsz == 0)
			{
				deallocate(addr);
				return nullptr;
			}
			else if (addr == nullptr)
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

	private:
		friend singleton<memory_manager>;

		memory_manager() noexcept = default;

		~memory_manager() noexcept;

		allocator_type			m_allocator	{};	// allocator
		size_t					m_index		{};	// record index
		ds::map<void *, record>	m_records	{};	// memory records
		util::test_resource *	m_testres	{};	// test resource

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

namespace ml
{
	// trackable base
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