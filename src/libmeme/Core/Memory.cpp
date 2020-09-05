#include <libmeme/Core/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	memory * memory::s_instance{};

	memory::memory(passthrough_resource * res) noexcept
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
		ML_assert(s_instance == this);

		while (!m_records.empty()) // cleanup remaining allocations
		{
			deallocate(m_records.back().first);
		}

		s_instance = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}