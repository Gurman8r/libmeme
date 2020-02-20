#ifndef _ML_MEMORY_HPP_
#define _ML_MEMORY_HPP_

#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// TEST RESOURCE
	namespace detail
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
		
			explicit test_resource(pmr::memory_resource * r, pointer const d, size_t const s) noexcept
				: m_res{ r }
				, m_data{ d }
				, m_size{ s }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline bool valid() const noexcept { return m_res && m_data && m_size; }

			inline operator bool() const noexcept { return valid(); }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline pmr::memory_resource * upstream_resource() const noexcept { return m_res; }

			inline size_t addr() const noexcept { return (size_t)m_data; }

			inline pointer const data() noexcept { return m_data; }

			inline const_pointer const data() const noexcept { return m_data; }

			inline size_t size() const noexcept { return m_size; }

			inline size_t count() const noexcept { return m_count; }

			inline size_t used() const noexcept { return m_used; }

			inline size_t free() const noexcept { return m_size - m_used; }

			inline float_t fraction() const noexcept { return (float_t)m_used / (float_t)m_size; }

			inline float_t percent() const noexcept { return fraction() * 100.f; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline reference front() & noexcept { return m_data[0]; }

			inline const_reference front() const & noexcept { return m_data[0]; }

			inline reference back() & noexcept { return m_data[m_size - 1]; }

			inline const_reference back() const & noexcept { return m_data[m_size - 1]; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline pointer const begin() noexcept { return m_data; }

			inline const_pointer const begin() const noexcept { return m_data; }

			inline const_pointer const cbegin() const noexcept { return m_data; }

			inline pointer const end() noexcept { return m_data + m_size; }

			inline const_pointer const end() const noexcept { return m_data + m_size; }

			inline const_pointer const cend() const noexcept { return m_data + m_size; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		protected:
			inline void * do_allocate(size_t const bytes, size_t const align) override
			{
				ML_ASSERT(m_res);
				++m_count;
				m_used += bytes;
				return m_res->allocate(bytes, align);
			}

			inline void do_deallocate(void * ptr, size_t const bytes, size_t const align) override
			{
				ML_ASSERT(m_res);
				--m_count;
				m_used -= bytes;
				return m_res->deallocate(ptr, bytes, align);
			}

			inline bool do_is_equal(pmr::memory_resource const & other) const noexcept override
			{
				ML_ASSERT(m_res);
				return m_res->is_equal(other);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		private:
			pmr::memory_resource * m_res;
			pointer const m_data;
			size_t const m_size;
			size_t m_count{}, m_used{};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// MEMORY MANAGER
	struct ML_CORE_API memory_manager final : singleton<memory_manager>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		struct ML_NODISCARD record final
		{
			size_t index; size_t size; byte_t * data;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool startup(detail::test_resource * test) noexcept
		{
			return test && (*test) && (&(ref().m_testres = test));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static void * allocate(size_t const size);

		static void deallocate(void * const addr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline auto const & get_allocator() noexcept { return cref().m_alloc; }

		ML_NODISCARD static inline auto const & get_index() noexcept { return cref().m_index; }

		ML_NODISCARD static inline auto const & get_records() noexcept { return cref().m_records; }

		ML_NODISCARD static inline auto const & get_testres() noexcept { return cref().m_testres; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<memory_manager>;

		memory_manager() noexcept;
		~memory_manager();

		allocator_type					m_alloc;
		size_t							m_index;
		ds::flat_map<void *, record>	m_records;
		detail::test_resource *			m_testres;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// TRACKABLE
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