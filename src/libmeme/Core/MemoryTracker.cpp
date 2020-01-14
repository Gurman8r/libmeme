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

	AllocationRecord::AllocationRecord(storage_type && storage) noexcept
		: m_storage{ std::move(storage) }
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

			m_records.for_each([&](auto, auto const & rec)
			{
				std::cerr << std::left
					<< std::setw(indx_size) << rec->index()
					<< std::setw(size_size) << rec->size()
					<< std::setw(addr_size) << rec->data()
					<< '\n';
			});

			Debug::pause(1);
		}
#endif
		ML_ASSERT("MEMORY LEAKS DETECTED" && m_records.empty());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Trackable * MemoryTracker::make_allocation(size_t size, int32_t flags)
	{
		return ([&, this, temp = ::new (ML_IMPL_NEW(size)) Trackable]() {
			return (*m_records.try_emplace(temp, ::new AllocationRecord{
				std::make_tuple(m_current++, size, flags, temp) }
			).first.second)->data();
		})();
	}

	void MemoryTracker::free_allocation(Trackable * value, int32_t flags)
	{
		if (auto const it{ m_records.find(value) })
		{
			ML_IMPL_DELETE(value);

			::delete (**it);

			m_records.erase(m_records.get_key(*it));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}