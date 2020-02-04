#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

#define ML_Memory ::ml::memory_tracker::get_instance()

namespace ml
{
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
			byte_t *	// data
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline decltype(auto) index() const noexcept { return std::get<ID_Index>(m_storage); }

		ML_NODISCARD inline decltype(auto) size() const noexcept { return std::get<ID_Size>(m_storage); }

		ML_NODISCARD inline decltype(auto) flags() const noexcept { return std::get<ID_Flags>(m_storage); }

		ML_NODISCARD inline decltype(auto) data() const noexcept { return std::get<ID_Data>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct memory_tracker;

		template <class ... Args
		> explicit allocation_record(Args && ... args) noexcept
			: m_storage{ std::make_tuple(std::forward<Args>(args)...) }
		{
		}

		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// Memory Tracker
	struct ML_CORE_API memory_tracker final : singleton<memory_tracker>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using storage_type = typename ds::flat_map<void *, allocation_record *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto get_allocator() const noexcept -> allocator_type const & { return m_alloc; }

		ML_NODISCARD inline auto get_index() const noexcept -> size_t { return m_index; }

		ML_NODISCARD inline auto get_records() const noexcept -> storage_type const & { return m_records; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct singleton<memory_tracker>;

		friend struct trackable;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		memory_tracker() noexcept;

		~memory_tracker();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void * make_allocation(size_t size, int32_t flags) noexcept;

		void free_allocation(void * value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		allocator_type	m_alloc;
		size_t			m_index;
		storage_type	m_records;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Trackable
	struct ML_CORE_API trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~trackable() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline void * operator new(size_t size) noexcept
		{
			return ML_Memory.make_allocation(size, 1);
		}

		ML_NODISCARD inline void * operator new[](size_t size) noexcept
		{
			return ML_Memory.make_allocation(size, 2);
		}
		
		inline void operator delete(void * value) noexcept
		{
			return ML_Memory.free_allocation(value);
		}

		inline void operator delete[](void * value) noexcept
		{
			return ML_Memory.free_allocation(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_