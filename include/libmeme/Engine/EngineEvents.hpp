#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <libmeme/Core/EventBus.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(load_event) {};

	ML_decl_event(unload_event) {};

	ML_decl_event(update_event) {};

	ML_decl_event(dockspace_event) {};

	ML_decl_event(main_menu_bar_event) {};

	ML_decl_event(gui_event) {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_