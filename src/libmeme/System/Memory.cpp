#include <libmeme/System/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory * memory::s_instance{};

	memory::memory(test_resource * res) noexcept : m_testres{ res }
	{
		ML_assert("memory manager instance already exists" && !s_instance);

		s_instance = this;

		pmr::set_default_resource(res);

		ML_assert(testres() == pmr::get_default_resource());
	}

	memory::~memory() noexcept
	{
#if (ML_is_debug)
		if (!m_records.empty())
		{
			debug::error("final allocations follow:");

			debug::pause();

			enum : std::streamsize { W = 20 };

			std::cout << std::left
				<< std::setw(W) << "address"
				<< std::setw(W) << "index"
				<< std::setw(W) << "size"
				<< '\n';

			m_records.for_each([&](auto, auto const & rec)
			{
				std::cout << std::left
					<< std::setw(W) << rec.addr
					<< std::setw(W) << rec.index
					<< std::setw(W) << rec.size
					<< '\n';
			});

			debug::pause();
		}
#endif
		ML_assert("MEMORY LEAKS DETECTED" && m_records.empty());
		s_instance = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}