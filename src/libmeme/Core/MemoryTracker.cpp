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

	allocation_record::allocation_record(storage_type && storage) noexcept
		: m_storage{ std::move(storage) }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory_tracker::memory_tracker() noexcept : m_current{ 0 }, m_records{} {}

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

	trackable * memory_tracker::make_allocation(size_t size, int32_t flags)
	{
		auto const temp{ static_cast<trackable *>(ML_IMPL_NEW(size)) };
		return (*m_records.insert(temp, ::new allocation_record{
			std::make_tuple(m_current++, size, flags, temp) }
		).first.second)->data();
	}

	void memory_tracker::free_allocation(void * value, int32_t flags)
	{
		if (auto const it{ m_records.find(static_cast<trackable *>(value)) })
		{
			ML_IMPL_DELETE(value); // free the allocation

			::delete (*it->second); // delete the record
			
			m_records.erase(it->first); // erase the entry
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}