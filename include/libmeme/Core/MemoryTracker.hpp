#ifndef _ML_MEMORY_HPP_
#define _ML_MEMORY_HPP_

// system for tracking manual allocations

#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// allocation record
	struct allocation_record final : non_copyable
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
		friend struct memory_tracker;

		template <class ... Args
		> explicit allocation_record(Args && ... args) noexcept
			: m_storage{ std::make_tuple(ML_FWD(args)...) }
		{
		}

		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// memory tracker
	struct ML_CORE_API memory_tracker final : singleton<memory_tracker>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using storage_type = typename ds::flat_map<void *, allocation_record *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline auto const & get_buffer() noexcept
		{
			return get_instance().m_buffer;
		}

		static inline void set_buffer(byte_t * buffer, size_t size) noexcept
		{
			get_instance().m_buffer.first = buffer;
			get_instance().m_buffer.second = size;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline allocator_type const & get_allocator() noexcept
		{
			return get_instance().m_allocator;
		}

		ML_NODISCARD static inline size_t const & get_current_index() noexcept
		{
			return get_instance().m_current;
		}

		ML_NODISCARD static inline storage_type const & get_records() noexcept
		{
			return get_instance().m_records;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static void * make_allocation(size_t size, int32_t flags) noexcept;

		static void free_allocation(void * value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<memory_tracker>;

		memory_tracker() noexcept;
		~memory_tracker() noexcept;

		allocator_type	m_allocator;
		size_t			m_current;
		storage_type	m_records;

		std::pair<byte_t *, size_t> m_buffer;

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
			return memory_tracker::make_allocation(size, 1 << 0);
		}

		ML_NODISCARD inline void * operator new[](size_t size) noexcept
		{
			return memory_tracker::make_allocation(size, 1 << 1);
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

#endif // !_ML_MEMORY_HPP_