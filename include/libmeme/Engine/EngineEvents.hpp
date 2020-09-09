#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <libmeme/Core/Events.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(load_event) {};

	ML_decl_event(unload_event) {};

	ML_decl_event(update_event) {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(dockspace_event)
	{
		struct editor_dockspace * const dockspace;

		constexpr dockspace_event(struct editor_dockspace * dockspace) noexcept
			: dockspace{ dockspace }
		{
		}

	};

	ML_decl_event(main_menu_bar_event)
	{
		struct editor_dockspace * const dockspace;

		constexpr main_menu_bar_event(struct editor_dockspace * dockspace) noexcept
			: dockspace{ dockspace }
		{
		}
	};

	ML_decl_event(gui_event)
	{
		struct editor_context * const context;

		constexpr gui_event(struct editor_context * const context) noexcept
			: context{ context }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_