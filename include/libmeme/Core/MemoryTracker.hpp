#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

// system for tracking manual allocations

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// memory tracker
	struct ML_CORE_API memory_tracker final : singleton<memory_tracker>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		enum : size_t { ID_Index, ID_Size, ID_Flags, ID_Data };

		using record = typename std::tuple<
			size_t,		// index
			size_t,		// size
			int32_t,	// flags
			byte_t *	// data
		>;

		using storage_type = typename ds::flat_map<void *, record *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static void * make_allocation(size_t size, int32_t flags) noexcept;

		static void free_allocation(void * value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline auto get_allocator() noexcept -> allocator_type const &
		{
			return get_instance().m_allocator;
		}

		ML_NODISCARD static inline auto get_index() noexcept -> size_t const &
		{
			return get_instance().m_index;
		}

		ML_NODISCARD static inline auto get_records() noexcept -> storage_type const &
		{
			return get_instance().m_records;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<memory_tracker>;

		memory_tracker() noexcept;
		~memory_tracker() noexcept;

		allocator_type	m_allocator;
		size_t			m_index;
		storage_type	m_records;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// trackable
	struct ML_CORE_API trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~trackable() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline void * operator new(size_t size) noexcept
		{
			return memory_tracker::make_allocation(size, 1);
		}

		ML_NODISCARD inline void * operator new[](size_t size) noexcept
		{
			return memory_tracker::make_allocation(size, 2);
		}
		
		inline void operator delete(void * value) noexcept
		{
			return memory_tracker::free_allocation(value);
		}

		inline void operator delete[](void * value) noexcept
		{
			return memory_tracker::free_allocation(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_