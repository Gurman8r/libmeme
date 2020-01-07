#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/Debug.hpp>

#ifndef ML_IMPL_NEW
#define ML_IMPL_NEW std::malloc
#endif

#ifndef ML_IMPL_DELETE
#define ML_IMPL_DELETE std::free
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	AllocationRecord::AllocationRecord(storage_type && storage) noexcept
		: m_storage{ std::move(storage) }
	{
	}

	AllocationRecord::~AllocationRecord() noexcept
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MemoryTracker::MemoryTracker() noexcept
		: m_current{ 0 }
		, m_records{}
	{
	}

	MemoryTracker::~MemoryTracker()
	{
#if (ML_DEBUG)
		if (!m_records.empty())
		{
			Debug::logError("MEMORY LEAKS DETECTED");

			static constexpr std::streamsize
				indx_size{ 6 },
				size_size{ sizeof(size_t) },
				addr_size{ sizeof(size_t) * 3 };

			std::cerr << std::left
				<< std::setw(indx_size) << "Index"
				<< std::setw(size_size) << "Size"
				<< std::setw(addr_size) << "Address"
				<< '\n';

			for (auto const & pair : m_records)
			{
				std::cerr << std::left
					<< std::setw(indx_size) << pair.second->index()
					<< std::setw(size_size) << pair.second->size()
					<< std::setw(addr_size) << pair.second->data()
					<< '\n';
			}

			Debug::pause(1);
		}
#endif
		ML_ASSERT("MEMORY LEAKS DETECTED" && m_records.empty());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Trackable * MemoryTracker::create_allocation(size_t size)
	{
		auto * const ptr{ static_cast<Trackable *>(ML_IMPL_NEW(size)) };
		return m_records.insert(std::make_pair(
			ptr, ::new AllocationRecord{ std::make_tuple(m_current++, size, ptr) }
		)).first->second->data();
	}

	void MemoryTracker::destroy_allocation(Trackable * value)
	{
		if (auto it{ m_records.find(value) }; it != m_records.end())
		{
			if (auto ptr{ it->second->data() })
			{
				ML_IMPL_DELETE(ptr);
			}
			::delete it->second;
			m_records.erase(it);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}