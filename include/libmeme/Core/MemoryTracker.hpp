#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

#define ML_MemoryTracker ::ml::MemoryTracker::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Trackable;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Allocation Record
	struct ML_CORE_API AllocationRecord final : public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Index, ID_Size, ID_Flags, ID_Data };

		using storage_type = typename std::tuple<
			size_t,		// index
			size_t,		// size
			int32_t,	// flags
			Trackable *	// data
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
		friend struct MemoryTracker;

		explicit AllocationRecord(storage_type && storage) noexcept;

		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// Memory Tracker
	struct ML_CORE_API MemoryTracker final : public Singleton<MemoryTracker>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_type = typename ds::flat_map<Trackable *, AllocationRecord *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline size_t current() { return m_current; }

		ML_NODISCARD inline auto const & records() { return m_records; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<MemoryTracker>;

		friend struct Trackable;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MemoryTracker() noexcept;

		~MemoryTracker();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		size_t m_current;

		ds::flat_map<Trackable *, AllocationRecord *> m_records;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Trackable * make_allocation(size_t size, int32_t flags);

		void free_allocation(void * value, int32_t flags);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Trackable
	struct ML_CORE_API Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~Trackable() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * operator new(size_t size)
		{
			return ML_MemoryTracker.make_allocation(size, 0);
		}

		inline void * operator new[](size_t size)
		{
			return ML_MemoryTracker.make_allocation(size, 1);
		}
		
		inline void operator delete(void * value)
		{
			return ML_MemoryTracker.free_allocation(value, 0);
		}

		inline void operator delete[](void * value)
		{
			return ML_MemoryTracker.free_allocation(value, 1);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_