#include <libmeme/Core/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory_manager::~memory_manager()
	{
#if (ML_is_debug)
		if (!m_records.empty())
		{
			debug::error("final allocations follow");

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
}