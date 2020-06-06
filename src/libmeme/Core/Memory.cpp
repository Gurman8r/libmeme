#include <libmeme/Core/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory::~memory() noexcept
	{
#if (ML_is_debug)
		if (!m_records.empty())
		{
			debug::error("final allocations follow:");

			debug::pause();

			enum : std::streamsize { W = 20 };

			debug::std_out << std::left
				<< std::setw(W) << "index"
				<< std::setw(W) << "size"
				<< std::setw(W) << "address"
				<< '\n';

			m_records.for_each([&](auto, auto const & rec)
			{
				debug::std_out << std::left
					<< std::setw(W) << rec.index
					<< std::setw(W) << rec.size
					<< std::setw(W) << rec.data
					<< '\n';
			});

			debug::pause();
		}
#endif
		ML_assert("MEMORY LEAKS DETECTED" && m_records.empty());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}