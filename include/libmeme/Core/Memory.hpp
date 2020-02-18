#ifndef _ML_MEMORY_HPP_
#define _ML_MEMORY_HPP_

#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace util
	{
		// TEST RESOURCE
		struct test_resource final : public pmr::memory_resource, non_copyable
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using base_type		= typename pmr::memory_resource;
			using pointer		= typename byte_t *;
			using const_pointer = typename byte_t const *;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
			explicit test_resource(base_type * base, byte_t * const data, size_t const size) noexcept
				: m_upstream{ base }
				, m_data	{ data }
				, m_size	{ size }
				, m_used	{ 0 }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline operator bool() const noexcept
			{
				return m_upstream && m_data && m_size;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline base_type * upstream_resource() const noexcept { return m_upstream; }

			inline size_t addr() const noexcept { return (size_t)m_data; }

			inline pointer const data() noexcept { return m_data; }

			inline const_pointer const data() const noexcept { return m_data; }

			inline size_t size() const noexcept { return m_size; }

			inline size_t used() const noexcept { return m_used; }

			inline size_t available() const noexcept { return m_size - m_used; }

			inline float_t fraction() const noexcept { return (float_t)m_used / (float_t)m_size; }

			inline float_t percent() const noexcept { return fraction() * 100.f; }

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
				m_used += bytes;
				return m_upstream->allocate(bytes, align);
			}

			inline void do_deallocate(void * ptr, size_t const bytes, size_t const align) override
			{
				m_used -= bytes;
				return m_upstream->deallocate(ptr, bytes, align);
			}

			inline bool do_is_equal(pmr::memory_resource const & other) const noexcept override
			{
				return m_upstream->is_equal(other);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		private:
			base_type * m_upstream;
			byte_t * const m_data;
			size_t m_size, m_used;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// MEMORY RECORD
	struct memory_record final : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Index, ID_Size, ID_Flags, ID_Data };

		using storage_type = typename std::tuple<
			size_t,		// index
			size_t,		// size
			int32_t,	// flags
			byte_t *	// data
		>;

		constexpr decltype(auto) index() const noexcept { return std::get<ID_Index>(m_storage); }

		constexpr decltype(auto) size() const noexcept { return std::get<ID_Size>(m_storage); }

		constexpr decltype(auto) flags() const noexcept { return std::get<ID_Flags>(m_storage); }

		constexpr decltype(auto) data() const noexcept { return std::get<ID_Data>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct memory_manager;

		template <class ... Args
		> explicit memory_record(Args && ... args) noexcept
			: m_storage{ std::make_tuple(ML_FWD(args)...) }
		{
		}

		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// MEMORY MANAGER
	struct ML_CORE_API memory_manager final : singleton<memory_manager>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using record_table = typename ds::flat_map<void *, memory_record *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool startup(util::test_resource * res)
		{
			return (res && *res) ? &(((memory_manager &)get_instance()).m_resource = res) : false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static void * allocate(size_t const size, int32_t const flags = 0) noexcept;

		static void deallocate(void * value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto const & get_allocator() noexcept
		{
			return ((memory_manager const &)get_instance()).m_allocator;
		}

		ML_NODISCARD static inline auto get_current() noexcept
		{
			return ((memory_manager const &)get_instance()).m_current;
		}

		ML_NODISCARD static inline auto const & get_records() noexcept
		{
			return ((memory_manager const &)get_instance()).m_records;
		}

		ML_NODISCARD static inline auto * const get_resource() noexcept
		{
			return ((memory_manager &)get_instance()).m_resource;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<memory_manager>;

		memory_manager() noexcept;
		~memory_manager() noexcept;

		allocator_type			m_allocator;
		size_t					m_current;
		record_table			m_records;
		util::test_resource *	m_resource;

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
			return memory_manager::allocate(size, 1 << 0);
		}

		ML_NODISCARD inline void * operator new[](size_t size) noexcept
		{
			return memory_manager::allocate(size, 1 << 1);
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