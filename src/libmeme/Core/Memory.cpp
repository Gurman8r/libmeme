#include <libmeme/Core/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory * memory::g_memory{};

	memory::memory(passthrough_resource * res) noexcept
		: m_resource	{ res }
		, m_allocator	{}
		, m_counter		{}
		, m_records		{}
	{
		ML_assert(!g_memory && (g_memory = this));

		ML_assert(m_resource == pmr::get_default_resource());
	}

	memory::~memory() noexcept
	{
		ML_assert(g_memory == this && !(g_memory = nullptr));

		ML_assert("MEMORY LEAKS DETECTED" && m_records.empty());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}