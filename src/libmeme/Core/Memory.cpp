#include <libmeme/Core/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory * memory::s_instance{};

	memory::memory(arena_test_resource * res) noexcept
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
		for (auto addr : ds::set<void *>{ m_records.keys() })
		{
			deallocate(addr);
		}

		ML_assert("MEMORY LEAKS DETECTED" && m_records.empty());

		s_instance = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}