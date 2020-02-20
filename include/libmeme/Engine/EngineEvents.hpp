#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct enter_event final : T_event<enter_event>
	{
	};

	struct frame_begin_event final : T_event<frame_begin_event>
	{
	};

	struct update_event final : T_event<update_event>
	{
	};

	struct begin_draw_event final : T_event<begin_draw_event>
	{
	};

	struct draw_event final : T_event<draw_event>
	{
	};

	struct end_draw_event final : T_event<draw_event>
	{
	};

	struct frame_end_event final : T_event<frame_end_event>
	{
	};

	struct exit_event final : T_event<exit_event>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_