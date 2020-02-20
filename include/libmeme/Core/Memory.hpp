#ifndef _ML_MEMORY_HPP_
#define _ML_MEMORY_HPP_

#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		// TEST RESOURCE - test to a memory resource which provides usage statistics
		struct test_resource final : public pmr::memory_resource, non_copyable
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using pointer			= typename byte_t *;
			using const_pointer		= typename byte_t const *;
			using reference			= typename byte_t &;
			using const_reference	= typename byte_t const &;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
			explicit test_resource(pmr::memory_resource * r, pointer const d, size_t const s) noexcept
				: m_mres{ r }
				, m_data{ d }
				, m_size{ s }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline bool valid() const noexcept { return m_mres && m_data && m_size; }

			inline operator bool() const noexcept { return valid(); }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline pmr::memory_resource * upstream_resource() const noexcept { return m_mres; }

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
				++m_count;
				m_used += bytes;
				return m_mres->allocate(bytes, align);
			}

			inline void do_deallocate(void * ptr, size_t const bytes, size_t const align) override
			{
				--m_count;
				m_used -= bytes;
				return m_mres->deallocate(ptr, bytes, align);
			}

			inline bool do_is_equal(pmr::memory_resource const & other) const noexcept override
			{
				return m_mres->is_equal(other);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		private:
			pmr::memory_resource * m_mres;
			pointer const m_data;
			size_t const m_size;
			size_t m_count{}, m_used{};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// MEMORY TRACKER
	struct ML_CORE_API memory_tracker final : singleton<memory_tracker>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// MEMORY RECORD
		struct record final
		{
			size_t		const index;
			size_t		const size;
			byte_t *	const data;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using record_table = typename ds::flat_map<byte_t *, record *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool startup(detail::test_resource * test)
		{
			return (test && *test) ? &(ref().m_testres = test) : false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static void * allocate(size_t const size) noexcept;

		static void deallocate(void * value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline auto const & get_allocator() noexcept
		{
			return cref().m_alloc;
		}

		ML_NODISCARD static inline auto const & get_index() noexcept
		{
			return cref().m_index;
		}

		ML_NODISCARD static inline auto const & get_records()  noexcept
		{
			return cref().m_records;
		}

		ML_NODISCARD static inline auto const & get_testres() noexcept
		{
			return cref().m_testres;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<memory_tracker>;

		memory_tracker() noexcept;
		~memory_tracker() noexcept;

		allocator_type			m_alloc;
		size_t					m_index;
		record_table			m_records;
		detail::test_resource *	m_testres;

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
			return memory_tracker::allocate(size);
		}

		ML_NODISCARD inline void * operator new[](size_t size) noexcept
		{
			return memory_tracker::allocate(size);
		}
		
		inline void operator delete(void * value) noexcept
		{
			return memory_tracker::deallocate(value);
		}

		inline void operator delete[](void * value) noexcept
		{
			return memory_tracker::deallocate(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_HPP_