#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory_manager::memory_manager() noexcept
		: m_alloc	{}
		, m_index	{}
		, m_records	{}
		, m_testres	{}
	{
	}

	memory_manager::~memory_manager()
	{
#if ML_IS_DEBUG
		if (!m_records.empty())
		{
			debug::log::error("final allocations follow");

			debug::pause();
			
			std::cout << std::left
				<< std::setw(20) << "Index"
				<< std::setw(20) << "Size"
				<< std::setw(20) << "Address"
				<< '\n';

			m_records.for_each([&](auto, auto const & rec)
			{
				std::cout << std::left
					<< std::setw(20) << rec.index
					<< std::setw(20) << rec.size
					<< std::setw(20) << rec.data
					<< '\n';
			});

			debug::pause();
		}
#endif
		ML_assert("MEMORY LEAKS DETECTED" && m_records.empty());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// malloc
	void * memory_manager::allocate(size_t size)
	{
		static auto & inst{ memory_manager::get_instance() };

		// allocate the requested bytes
		return ([&, data = inst.m_alloc.allocate(size)]()
		{
			// create the record
			return (*inst.m_records.insert(data, record{ inst.m_index++, size, data }).first);
		})();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// calloc
	void * memory_manager::allocate(size_t count, size_t size)
	{
		return std::memset(allocate(count * size), 0, count * size);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// free
	void memory_manager::deallocate(void * addr)
	{
		static auto & inst{ memory_manager::get_instance() };

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

	// realloc
	void * memory_manager::reallocate(void * addr, size_t size)
	{
		return reallocate(addr, size, size);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// realloc (sized)
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
		else if (void * const temp{ allocate(newsz) })
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