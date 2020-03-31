#ifndef _ML_MEMORY_HPP_
#define _ML_MEMORY_HPP_

#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

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
		
		explicit test_resource(pmr::memory_resource * r, pointer const d, size_t s) noexcept
			: m_resource{ r }, m_buffer{ d }, m_total_bytes{ s }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool good() const noexcept
		{
			return m_resource && m_buffer && (0 < m_total_bytes);
		}

		operator bool() const noexcept { return good(); }

		pmr::memory_resource * upstream_resource() const noexcept { return m_resource; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		size_t addr() const noexcept { return (size_t)m_buffer; }

		size_t num_allocations() const noexcept { return m_alloc_count; }

		size_t total_bytes() const noexcept { return m_total_bytes; }

		size_t used_bytes() const noexcept { return m_bytes_used; }

		size_t free_bytes() const noexcept { return m_total_bytes - m_bytes_used; }

		float_t fraction_used() const noexcept { return (float_t)m_bytes_used / (float_t)m_total_bytes; }

		float_t percent_used() const noexcept { return fraction_used() * 100.f; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pointer const data() noexcept { return m_buffer; }

		const_pointer const data() const noexcept { return m_buffer; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		reference front() & noexcept { return m_buffer[0]; }

		const_reference front() const & noexcept { return m_buffer[0]; }

		reference back() & noexcept { return m_buffer[m_total_bytes - 1]; }

		const_reference back() const & noexcept { return m_buffer[m_total_bytes - 1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pointer const begin() noexcept { return m_buffer; }

		const_pointer const begin() const noexcept { return m_buffer; }

		const_pointer const cbegin() const noexcept { return m_buffer; }

		pointer const end() noexcept { return m_buffer + m_total_bytes; }

		const_pointer const end() const noexcept { return m_buffer + m_total_bytes; }

		const_pointer const cend() const noexcept { return m_buffer + m_total_bytes; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		void * do_allocate(size_t bytes, size_t align) override
		{
			++m_alloc_count;
			m_bytes_used += bytes;
			return m_resource->allocate(bytes, align);
		}

		void do_deallocate(void * ptr, size_t bytes, size_t align) override
		{
			--m_alloc_count;
			m_bytes_used -= bytes;
			return m_resource->deallocate(ptr, bytes, align);
		}

		bool do_is_equal(pmr::memory_resource const & other) const noexcept override
		{
			return m_resource->is_equal(other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		pmr::memory_resource * m_resource;
		pointer const m_buffer;
		size_t const m_total_bytes;

		size_t m_alloc_count {};
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

		static bool initialize(util::test_resource * resource) noexcept
		{
			static auto & inst{ get_instance() };

			if (inst.m_testres || !resource || !(*resource))
			{
				return false;
			}
			else
			{
				inst.m_testres = resource;

				return (pmr::get_default_resource() == inst.m_testres);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// malloc
		ML_NODISCARD static void * allocate(size_t size);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// calloc
		ML_NODISCARD static void * allocate(size_t count, size_t size);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// free
		static void deallocate(void * addr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// realloc
		ML_NODISCARD static void * reallocate(void * addr, size_t size);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// realloc (sized)
		ML_NODISCARD static void * reallocate(void * addr, size_t oldsz, size_t newsz);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto const & get_allocator() noexcept { return get_instance().m_alloc; }

		ML_NODISCARD static auto const & get_index() noexcept { return get_instance().m_index; }

		ML_NODISCARD static auto const & get_records() noexcept { return get_instance().m_records; }

		ML_NODISCARD static auto const & get_testres() noexcept { return get_instance().m_testres; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<memory_manager>;

		memory_manager() noexcept;

		~memory_manager();

		allocator_type			m_alloc;	// allocator
		size_t					m_index;	// record index
		record_map				m_records;	// record table
		util::test_resource *	m_testres;	// test resource

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

		void operator delete(void * value) noexcept
		{
			return memory_manager::deallocate(value);
		}

		void operator delete[](void * value) noexcept
		{
			return memory_manager::deallocate(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_HPP_