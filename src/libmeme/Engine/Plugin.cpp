#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/Application.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin::plugin(application * app, void * user) noexcept
		: system_object	{ app->get_system() }
		, event_listener{ app->get_bus() }
		, m_app			{ app }
		, m_user		{ user }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}