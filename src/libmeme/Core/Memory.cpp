#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory_tracker::memory_tracker() noexcept
		: m_alloc	{}
		, m_index		{}
		, m_records		{}
		, m_testres		{ nullptr }
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

			for (auto & rec : m_records)
			{
				std::cerr << std::left
					<< std::setw(indx_size) << rec.index
					<< std::setw(size_size) << rec.size
					<< std::setw(addr_size) << rec.data
					<< '\n';
			};

			debug::pause(1);
		}
#endif
		ML_ASSERT("MEMORY LEAKS DETECTED" && m_records.empty());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * memory_tracker::allocate(size_t const size) noexcept
	{
		static auto & inst{ ref() };
		if (size <= 0) return nullptr;

		return inst.m_records.emplace_back(memory_record{
			++inst.m_index,
			size,
			inst.m_alloc.allocate(size)
		}).data;
	}

	void memory_tracker::deallocate(void * value) noexcept
	{
		static auto & inst{ ref() };
		if (!value || inst.m_records.empty()) return;

		// find the entry
		auto const rec{ std::find_if(inst.m_records.begin(), inst.m_records.end(),
			[value](auto && e) { return (e.data == value); }
		) };

		if ((rec != inst.m_records.end()) && (rec->data && rec->size))
		{
			// free the allocation
			inst.m_alloc.deallocate(rec->data, rec->size);

			// erase the entry
			inst.m_records.erase(rec);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}