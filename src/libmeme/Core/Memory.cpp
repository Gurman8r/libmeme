#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory_manager::memory_manager() noexcept
		: m_allocator	{}
		, m_current		{}
		, m_records		{}
		, m_resource	{ nullptr }
	{
	}

	memory_manager::~memory_manager() noexcept
	{
#if ML_DEBUG
		if (!m_records.empty())
		{
			debug::log_error("MEMORY LEAKS DETECTED");

			static constexpr std::streamsize
				indx_size{ 8 },
				size_size{ sizeof(size_t) },
				addr_size{ sizeof(size_t) * 3 };

			std::cerr << std::left
				<< std::setw(indx_size) << "Index"
				<< std::setw(size_size) << "Size"
				<< std::setw(addr_size) << "Address"
				<< '\n';

			m_records.for_each([&](auto, auto const & rec)
			{
				std::cerr << std::left
					<< std::setw(indx_size) << rec->index()
					<< std::setw(size_size) << rec->size()
					<< std::setw(addr_size) << rec->data()
					<< '\n';
			});

			debug::pause(1);
		}
#endif
		ML_ASSERT("MEMORY LEAKS DETECTED" && m_records.empty());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * memory_manager::allocate(size_t const size, int32_t const flags) noexcept
	{
		static memory_manager & inst{ get_instance() };

		if (size <= 0) return nullptr;

		// allocate the requested bytes
		byte_t * const data{ inst.m_allocator.allocate(size) };

		// create the record
		return (*inst.m_records.insert(
			data,
			::new (inst.m_allocator.allocate(sizeof(memory_record))) memory_record
			{
				++inst.m_current, size, flags, data
			}
		).first);
	}

	void memory_manager::deallocate(void * value) noexcept
	{
		static memory_manager & inst{ get_instance() };

		if (inst.m_records.empty() || !value) return;

		// find the entry
		if (auto const it{ inst.m_records.find(value) })
		{
			// free the allocation
			inst.m_allocator.deallocate(
				(*it->second)->data(),
				(*it->second)->size()
			);

			// free the record
			inst.m_allocator.deallocate(
				reinterpret_cast<byte_t *>(*it->second),
				sizeof(memory_record)
			);

			// erase the entry
			inst.m_records.erase(it->first);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}