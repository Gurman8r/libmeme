#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

#define ML_memory_tracker ::ml::memory_tracker::get_instance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct trackable;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Allocation Record
	struct ML_CORE_API allocation_record final : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Index, ID_Size, ID_Flags, ID_Data };

		using storage_type = typename std::tuple<
			size_t,		// index
			size_t,		// size
			int32_t,	// flags
			trackable *	// data
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline decltype(auto) index() const noexcept
		{
			return std::get<ID_Index>(m_storage);
		}

		ML_NODISCARD inline decltype(auto) size() const noexcept
		{
			return std::get<ID_Size>(m_storage);
		}

		ML_NODISCARD inline decltype(auto) flags() const noexcept
		{
			return std::get<ID_Flags>(m_storage);
		}

		ML_NODISCARD inline decltype(auto) data() const noexcept
		{
			return std::get<ID_Data>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct memory_tracker;

		explicit allocation_record(storage_type && storage) noexcept;

		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// Memory Tracker
	struct ML_CORE_API memory_tracker final : singleton<memory_tracker>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_type = typename ds::flat_map<trackable *, allocation_record *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline size_t current() { return m_current; }

		ML_NODISCARD inline auto const & records() { return m_records; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct singleton<memory_tracker>;

		friend struct trackable;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		memory_tracker() noexcept;

		~memory_tracker();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		size_t m_current;

		ds::flat_map<trackable *, allocation_record *> m_records;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		trackable * make_allocation(size_t size, int32_t flags);

		void free_allocation(void * value, int32_t flags);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Trackable
	struct ML_CORE_API trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~trackable() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * operator new(size_t size)
		{
			return ML_memory_tracker.make_allocation(size, 0);
		}

		inline void * operator new[](size_t size)
		{
			return ML_memory_tracker.make_allocation(size, 1);
		}
		
		inline void operator delete(void * value)
		{
			return ML_memory_tracker.free_allocation(value, 0);
		}

		inline void operator delete[](void * value)
		{
			return ML_memory_tracker.free_allocation(value, 1);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_