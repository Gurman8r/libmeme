#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory_tracker::memory_tracker() noexcept
		: m_alloc	{}
		, m_index	{}
		, m_records	{}
		, m_testres	{ nullptr }
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
					<< std::setw(indx_size) << rec->index
					<< std::setw(size_size) << rec->size
					<< std::setw(addr_size) << rec->data
					<< '\n';
			});

			debug::pause(1);
		}
#endif
		ML_ASSERT("MEMORY LEAKS DETECTED" && m_records.empty());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * memory_tracker::allocate(size_t const size) noexcept
	{
		static auto & inst{ ref() };

		ML_ASSERT(0 < size);

		// allocate the record
		byte_t * const rec{ inst.m_alloc.allocate(sizeof(record)) };

		// allocate the requested bytes
		byte_t * const ptr{ inst.m_alloc.allocate(size) };

		// insert the entry
		return (*inst.m_records.insert(ptr, new (rec)
			record{ ++inst.m_index, size, ptr }
		).first);
	}

	void memory_tracker::deallocate(void * value) noexcept
	{
		static auto & inst{ ref() };

		if (!value || inst.m_records.empty()) return;

		// find the entry
		if (auto const it{ inst.m_records.find(reinterpret_cast<byte_t *>(value)) })
		{
			// free the allocation
			inst.m_alloc.deallocate((*it->second)->data, (*it->second)->size);

			// free the record
			inst.m_alloc.deallocate(reinterpret_cast<byte_t *>(*it->second), sizeof(record));

			// erase the entry
			inst.m_records.erase(it->first);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}