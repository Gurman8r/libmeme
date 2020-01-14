#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

#define ML_MemoryTracker ::ml::MemoryTracker::getInstance()

namespace ml
{
	// Allocation Record
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_CORE_API AllocationRecord final : public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Index, ID_Size, ID_Flags, ID_Data };

		using storage_type = typename std::tuple<
			size_t,				// index
			size_t,				// size
			int32_t,			// flags
			struct Trackable *	// data
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline decltype(auto) index() const noexcept { return std::get<ID_Index>(m_storage); }

		inline decltype(auto) size() const noexcept { return std::get<ID_Size>(m_storage); }

		inline decltype(auto) flags() const noexcept { return std::get<ID_Flags>(m_storage); }

		inline decltype(auto) data() const noexcept { return std::get<ID_Data>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct MemoryTracker;

		explicit AllocationRecord(storage_type && storage) noexcept;

		union { storage_type m_storage; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Memory Tracker
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_CORE_API MemoryTracker final : public Singleton<MemoryTracker>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_type = typename ds::flat_map<
			struct Trackable *, AllocationRecord *
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto current() const -> size_t { return m_current; }

		inline auto records() const -> storage_type const & { return m_records; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<MemoryTracker>;

		friend struct Trackable;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MemoryTracker() noexcept;

		~MemoryTracker();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		size_t m_current;

		storage_type m_records;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Trackable * make_allocation(size_t size, int32_t flags);

		void free_allocation(struct Trackable * value, int32_t flags);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <int32_t Flags> inline auto make_allocation(size_t size)
		{
			return make_allocation(size, Flags);
		}

		template <int32_t Flags> inline auto free_allocation(struct Trackable * value)
		{
			return free_allocation(value, Flags);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	// Trackable
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_CORE_API Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~Trackable() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::type_info const & rtti() const noexcept { return typeid(*this); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * operator new(size_t size)
		{
			return ML_MemoryTracker.make_allocation<0>(size);
		}

		inline void * operator new[](size_t size)
		{
			return ML_MemoryTracker.make_allocation<1>(size);
		}
		
		inline void operator delete(void * value)
		{
			return ML_MemoryTracker.free_allocation<0>(static_cast<Trackable *>(value));
		}

		inline void operator delete[](void * value)
		{
			return ML_MemoryTracker.free_allocation<1>(static_cast<Trackable *>(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_