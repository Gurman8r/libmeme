#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/Application.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin::plugin(application * app, void * user) noexcept
		: system_object	{ app->getsys() }
		, event_listener{ app->getbus() }
		, m_app			{ app }
		, m_user		{ user }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}