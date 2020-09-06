#include <libmeme/Engine/Application.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	application * application::s_instance{};

	application::application(system_context * sys) : m_sys{ sys }, m_plugins{ sys }
	{
		ML_assert(!s_instance);
		s_instance = this;
	}

	application::~application()
	{
		ML_assert(s_instance == this);
		s_instance = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}