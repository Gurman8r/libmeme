#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/Debug.hpp>

#ifndef ML_IMPL_NEW
#define ML_IMPL_NEW malloc
#endif

#ifndef ML_IMPL_DELETE
#define ML_IMPL_DELETE free
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory_tracker::memory_tracker() noexcept
		: m_alloc	{}
		, m_index	{ 0 }
		, m_records	{}
	{
	}

	memory_tracker::~memory_tracker()
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
		return ([this, size, flags](byte_t * data)
		{
			return (*m_records.insert(
				data,
				::new (m_alloc.allocate(sizeof(allocation_record)))
				allocation_record{ m_index++, size, flags, data }
			).first.first);
		}
		)(m_alloc.allocate(size));
	}

	void memory_tracker::free_allocation(void * value) noexcept
	{
		// find the entry
		if (auto const it{ m_records.find(value) })
		{
			// get the record
			auto const & record{ (*it->second) };

			// free the allocation
			m_alloc.deallocate(record->data(), record->size());

			// free the record
			m_alloc.deallocate(reinterpret_cast<byte_t *>(record), sizeof(allocation_record));
			
			// erase the entry
			m_records.erase(it->first);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}