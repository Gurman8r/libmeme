#ifndef _ML_MEMORY_HPP_
#define _ML_MEMORY_HPP_

#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

namespace ml
{
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

		struct buffer_view final { byte_t * data; size_t size; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline void startup(buffer_view const & buf)
		{
			ML_ASSERT(buf.data);
			ML_ASSERT(buf.size);
			std::memcpy(&get_instance().m_buffer, &buf, sizeof(buffer_view));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static void * allocate(size_t size, int32_t flags) noexcept;

		static void deallocate(void * value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline auto const & get_buffer() noexcept
		{
			return ((memory_manager const &)get_instance()).m_buffer;
		}

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<memory_manager>;

		memory_manager() noexcept;
		~memory_manager() noexcept;

		allocator_type	m_allocator;
		size_t			m_current;
		record_table	m_records;
		buffer_view		m_buffer;

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