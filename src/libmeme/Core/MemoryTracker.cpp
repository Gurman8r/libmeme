#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory_tracker::memory_tracker() noexcept
		: m_index{}, m_records{}, m_allocator{}
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
					<< std::setw(indx_size) << std::get<ID_Index>(*rec)
					<< std::setw(size_size) << std::get<ID_Size>(*rec)
					<< std::setw(addr_size) << std::get<ID_Data>(*rec)
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
		if (size <= 0 || flags < 0)
			return nullptr;

		// allocate the requested bytes
		byte_t * const data{ inst.m_allocator.allocate(size) };

		// create the allocation record
		return (*inst.m_records.insert(
			data, 
			::new (inst.m_allocator.allocate(sizeof(record))) record
			{
				++inst.m_index, size, flags, data
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
				std::get<ID_Data>(**it->second), std::get<ID_Size>(**it->second)
			);

			// free the record
			inst.m_allocator.deallocate(
				reinterpret_cast<byte_t *>(*it->second), sizeof(record)
			);
			
			// erase the entry
			inst.m_records.erase(it->first);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}