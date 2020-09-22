#include <libmeme/Client/Plugin.hpp>
#include <libmeme/Client/Application.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin::plugin(application * app, void * user)
		: system_object	{ app->get_sys() }
		, m_app			{ app }
		, m_user		{ user }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}