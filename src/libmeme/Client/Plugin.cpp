#include <libmeme/Client/Plugin.hpp>
#include <libmeme/Client/PluginManager.hpp>
#include <libmeme/Client/GuiManager.hpp>
#include <libmeme/Client/Python.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin::plugin(plugin_manager * manager, void * user) noexcept
		: client_object	{ ML_check(manager)->get_context() }
		, m_manager		{ manager }
		, m_userptr		{ user }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}