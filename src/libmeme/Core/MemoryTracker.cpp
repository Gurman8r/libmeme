#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory_tracker::memory_tracker() noexcept
		: m_allocator{}, m_current{}, m_records{}
	{
	}

	memory_tracker::~memory_tracker() noexcept
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

	void * memory_tracker::make_allocation(size_t size, int32_t flags) noexcept
	{
		static auto & inst{ get_instance() };
		if (size <= 0)
			return nullptr;

		// allocate the requested bytes
		byte_t * const data{ inst.m_allocator.allocate(size) };

		// create the record
		return (*inst.m_records.insert(
			data,
			::new (inst.m_allocator.allocate(sizeof(allocation_record))) allocation_record
			{
				++inst.m_current, size, flags, data
			}
		).first);
	}

	void memory_tracker::free_allocation(void * value) noexcept
	{
		static auto & inst{ get_instance() };
		if (inst.m_records.empty() || !value)
			return;

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
				sizeof(allocation_record)
			);
			
			// erase the entry
			inst.m_records.erase(it->first);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}