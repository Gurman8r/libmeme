#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_event(load_event)
	{
	};

	ML_event(unload_event)
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_event(pre_loop_event)
	{
	};

	ML_event(post_loop_event)
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_event(update_event)
	{
	};

	ML_event(late_update_event)
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_event(pre_draw_event)
	{
	};

	ML_event(draw_event)
	{
	};

	ML_event(post_draw_event)
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_event(pre_gui_event)
	{
	};

	ML_event(gui_dock_event)
	{
	};

	ML_event(gui_draw_event)
	{
	};

	ML_event(post_gui_event)
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_