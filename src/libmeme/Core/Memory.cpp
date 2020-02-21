#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory_manager::memory_manager() noexcept
		: m_alloc	{}
		, m_index	{}
		, m_records	{}
		, m_testres	{ nullptr }
	{
	}

	memory_manager::~memory_manager()
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
					<< std::setw(indx_size) << rec.index
					<< std::setw(size_size) << rec.size
					<< std::setw(addr_size) << rec.data
					<< '\n';
			});

			debug::pause(1);
		}
		ML_BREAK_IF(!m_records.empty());
#endif
		ML_ASSERT("MEMORY LEAKS DETECTED" && m_records.empty());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * memory_manager::allocate(size_t size)
	{
		static auto & inst{ memory_manager::ref() };

		// allocate the requested bytes
		return ([&, data = inst.m_alloc.allocate(size)]()
		{
			// create the record
			return (*inst.m_records.insert(data, record{ ++inst.m_index, size, data }).first);
		})();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void memory_manager::deallocate(void * addr)
	{
		static auto & inst{ memory_manager::ref() };

		// find the entry
		if (auto const it{ inst.m_records.find(addr) })
		{
			// free the allocation
			inst.m_alloc.deallocate(it->second->data, it->second->size);

			// erase the record
			inst.m_records.erase(it->first);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * memory_manager::reallocate(void * addr, size_t size)
	{
		return reallocate(addr, size, size);
	}

	void * memory_manager::reallocate(void * addr, size_t oldsz, size_t newsz)
	{
		if (newsz == 0)
		{
			deallocate(addr);
			return nullptr;
		}
		else if (!addr)
		{
			return allocate(newsz);
		}
		else if (newsz <= oldsz)
		{
			return addr;
		}
		else if (void * temp{ allocate(newsz) })
		{
			std::memcpy(temp, addr, oldsz);
			deallocate(addr);
			return temp;
		}
		else
		{
			return nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}