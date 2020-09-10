#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <libmeme/Core/Events.hpp>

namespace ml
{
	struct application;
	struct editor_context;
	struct editor_dockspace;
	struct system_context;
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// LOAD
	ML_decl_event(load_event)
	{
		application * const app;

		auto operator->() const noexcept { return app; }

		constexpr load_event(application * app) noexcept : app{ app }
		{
		}
	};

	// UNLOAD
	ML_decl_event(unload_event)
	{
		application * const app;

		auto operator->() const noexcept { return app; }

		constexpr unload_event(application * app) noexcept : app{ app }
		{
		}
	};

	// UPDATE
	ML_decl_event(update_event)
	{
		application * const app;

		auto operator->() const noexcept { return app; }

		constexpr update_event(application * app) noexcept : app{ app }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// GUI
	ML_decl_event(gui_event)
	{
		editor_context * const context;

		auto operator->() const noexcept { return context; }

		constexpr gui_event(editor_context * const context) noexcept
			: context{ context }
		{
		}
	};

	// DOCKSPACE
	ML_decl_event(dockspace_event)
	{
		editor_dockspace * const dockspace;

		auto operator->() const noexcept { return dockspace; }

		constexpr dockspace_event(editor_dockspace * dockspace) noexcept
			: dockspace{ dockspace }
		{
		}
	};

	// MAIN MENU BAR
	ML_decl_event(main_menu_bar_event)
	{
		editor_dockspace * const dockspace;

		auto operator->() const noexcept { return dockspace; }

		constexpr main_menu_bar_event(editor_dockspace * dockspace) noexcept
			: dockspace{ dockspace }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_