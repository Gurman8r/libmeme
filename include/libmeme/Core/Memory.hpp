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

		inline bool good() const noexcept
		{
			return m_resource && m_buffer && (0 < m_total_bytes);
		}

		inline operator bool() const noexcept { return good(); }

		inline pmr::memory_resource * upstream_resource() const noexcept { return m_resource; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline size_t addr() const noexcept { return (size_t)m_buffer; }

		inline size_t num_allocations() const noexcept { return m_alloc_count; }

		inline size_t total_bytes() const noexcept { return m_total_bytes; }

		inline size_t used_bytes() const noexcept { return m_bytes_used; }

		inline size_t free_bytes() const noexcept { return m_total_bytes - m_bytes_used; }

		inline float_t fraction_used() const noexcept { return (float_t)m_bytes_used / (float_t)m_total_bytes; }

		inline float_t percent_used() const noexcept { return fraction_used() * 100.f; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline pointer const data() noexcept { return m_buffer; }

		inline const_pointer const data() const noexcept { return m_buffer; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline reference front() & noexcept { return m_buffer[0]; }

		inline const_reference front() const & noexcept { return m_buffer[0]; }

		inline reference back() & noexcept { return m_buffer[m_total_bytes - 1]; }

		inline const_reference back() const & noexcept { return m_buffer[m_total_bytes - 1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline pointer const begin() noexcept { return m_buffer; }

		inline const_pointer const begin() const noexcept { return m_buffer; }

		inline const_pointer const cbegin() const noexcept { return m_buffer; }

		inline pointer const end() noexcept { return m_buffer + m_total_bytes; }

		inline const_pointer const end() const noexcept { return m_buffer + m_total_bytes; }

		inline const_pointer const cend() const noexcept { return m_buffer + m_total_bytes; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		inline void * do_allocate(size_t bytes, size_t align) override
		{
			++m_alloc_count;
			m_bytes_used += bytes;
			return m_resource->allocate(bytes, align);
		}

		inline void do_deallocate(void * ptr, size_t bytes, size_t align) override
		{
			--m_alloc_count;
			m_bytes_used -= bytes;
			return m_resource->deallocate(ptr, bytes, align);
		}

		inline bool do_is_equal(pmr::memory_resource const & other) const noexcept override
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

		static inline bool initialize(util::test_resource * resource) noexcept
		{
			static auto & inst{ self_type::get() };

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

		ML_NODISCARD static inline allocator_type get_allocator() noexcept
		{
			return self_type::get().m_alloc;
		}

		ML_NODISCARD static inline size_t const & get_index() noexcept
		{
			return self_type::get().m_index;
		}

		ML_NODISCARD static inline record_map const & get_records() noexcept
		{
			return self_type::get().m_records;
		}

		ML_NODISCARD static inline util::test_resource * const get_testres() noexcept
		{
			return self_type::get().m_testres;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<memory_manager>;

		memory_manager() noexcept;
		~memory_manager();

		allocator_type			m_alloc;
		size_t					m_index;
		record_map				m_records;
		util::test_resource *	m_testres;

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

		ML_NODISCARD inline void * operator new(size_t size) noexcept
		{
			return memory_manager::allocate(size);
		}

		ML_NODISCARD inline void * operator new[](size_t size) noexcept
		{
			return memory_manager::allocate(size);
		}

		inline void operator delete(void * value) noexcept
		{
			return memory_manager::deallocate(value);
		}

		inline void operator delete[](void * value) noexcept
		{
			return memory_manager::deallocate(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_HPP_