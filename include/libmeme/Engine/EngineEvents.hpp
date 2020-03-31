#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_EVENT(load_event)
	{
	};

	ML_EVENT(loop_begin_event)
	{
	};

	ML_EVENT(update_event)
	{
	};

	ML_EVENT(fixed_update_event)
	{
	};

	ML_EVENT(begin_draw_event)
	{
	};

	ML_EVENT(draw_event)
	{
	};

	ML_EVENT(end_draw_event)
	{
	};

	ML_EVENT(loop_end_event)
	{
	};

	ML_EVENT(unload_event)
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_