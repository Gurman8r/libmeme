#include <libmeme/Core/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory * memory::g_mem{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory::memory(passthrough_resource & res)
		: m_resource{ std::addressof(res) }
		, m_alloc	{ m_resource }
		, m_records	{ m_alloc }
		, m_counter	{}
	{
		ML_assert(!g_mem && (g_mem = this));

		ML_assert(m_resource == pmr::get_default_resource());
	}

	memory::~memory() noexcept
	{
		ML_assert(g_mem == this && !(g_mem = nullptr));

		ML_assert("MEMORY LEAKS DETECTED" && m_records.empty());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}