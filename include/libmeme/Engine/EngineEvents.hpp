#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_event(load_event)
	{
	};

	ML_event(loop_begin_event)
	{
	};

	ML_event(update_event)
	{
	};

	ML_event(fixed_update_event)
	{
	};

	ML_event(begin_draw_event)
	{
	};

	ML_event(draw_event)
	{
	};

	ML_event(end_draw_event)
	{
	};

	ML_event(loop_end_event)
	{
	};

	ML_event(unload_event)
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_