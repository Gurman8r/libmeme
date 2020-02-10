#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct enter_event final : T_event<enter_event>
	{
		int32_t const argc;
		cstring const * argv;

		constexpr enter_event(int32_t argc, cstring const * argv) noexcept
			: argc{ argc }
			, argv{ argv }
		{
		}
	};

	struct frame_begin_event final : T_event<frame_begin_event>
	{
	};

	struct update_event final : T_event<update_event>
	{
	};

	struct draw_event final : T_event<draw_event>
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