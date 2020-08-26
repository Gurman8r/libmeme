#include <libmeme/Core/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory * memory::s_instance{};

	memory::memory(test_resource * res) noexcept
		: m_resource	{ res }
		, m_allocator	{}
		, m_counter		{}
		, m_records		{}
	{
		ML_assert("memory manager already exists" && !s_instance);

		ML_assert(m_resource == pmr::get_default_resource());

		s_instance = this;
	}

	memory::~memory() noexcept
	{
		ML_assert(this == s_instance);
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