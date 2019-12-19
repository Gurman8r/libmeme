#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/Debug.hpp>

#ifndef ML_IMPL_NEW
#define ML_IMPL_NEW ::std::malloc
#endif

#ifndef ML_IMPL_DELETE
#define ML_IMPL_DELETE ::std::free
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	AllocationRecord::AllocationRecord(storage_t && storage) noexcept
		: m_storage{ std::forward<storage_t>(storage) }
	{
	}

	AllocationRecord::~AllocationRecord() noexcept {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MemoryTracker::MemoryTracker() noexcept : m_current{ 0 }, m_records{}
	{
	}

	MemoryTracker::~MemoryTracker()
	{
#if (ML_DEBUG)
		if (!m_records.empty())
		{
			Debug::logError("MEMORY LEAKS DETECTED");

			static constexpr std::streamsize
				size_size{ sizeof(size_t) * 2 },
				addr_size{ sizeof(size_t) * 3 };

			std::cerr << std::left
				<< std::setw(6) << "Index"
				<< std::setw(size_size) << "Size"
				<< std::setw(addr_size) << "Address"
				//<< std::setw(10) << "Type"
				<< '\n';

			for (auto const & [ ptr, rec ] : m_records)
			{
				std::cerr << std::left
					<< std::setw(6) << rec->index()
					<< std::setw(size_size) << rec->size()
					<< std::setw(addr_size) << rec->data()
					//<< std::setw(10) << rec->data()->rtti().name()
					<< '\n';
			}

			Debug::pause(1);
		}
#endif
		ML_ASSERT("MEMORY LEAKS DETECTED" && m_records.empty());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * MemoryTracker::allocate(size_t size)
	{
		void * ptr { ML_IMPL_NEW(size) };
		return m_records.insert({ ptr, ::new AllocationRecord{
			std::make_tuple(m_current++, size, static_cast<Trackable *>(ptr))
		} }).first->second->data();
	}

	void MemoryTracker::deallocate(void * value)
	{
		if (auto it{ m_records.find(value) }; it != m_records.end() && it->second)
		{
			ML_IMPL_DELETE(it->second->data());
			delete it->second;
			m_records.erase(it);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}