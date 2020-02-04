#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct enter_event final : type_event<enter_event>
	{
		int32_t const argc;
		cstring const * argv;

		constexpr enter_event(int32_t argc, cstring const * argv) noexcept
			: argc{ argc }
			, argv{ argv }
		{
		}
	};

	struct begin_loop_event final : type_event<begin_loop_event>
	{
	};

	struct update_event final : type_event<update_event>
	{
	};

	struct draw_event final : type_event<draw_event>
	{
	};

	struct end_loop_event final : type_event<end_loop_event>
	{
	};

	struct exit_event final : type_event<exit_event>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_