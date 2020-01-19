#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	// Startup
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct enter_event final : I_event<enter_event>
	{
		int32_t const argc;
		C_string const * argv;

		constexpr enter_event(int32_t argc, C_string const * argv) noexcept
			: argc{ argc }
			, argv{ argv }
		{
		}
	};

	struct load_event final : I_event<load_event>
	{
		constexpr load_event() noexcept = default;
	};


	// Loop
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct begin_loop_event final : I_event<begin_loop_event>
	{
		constexpr begin_loop_event() noexcept = default;
	};

	struct update_event final : I_event<update_event>
	{
		constexpr update_event() noexcept = default;
	};

	struct draw_event final : I_event<draw_event>
	{
		constexpr draw_event() noexcept = default;
	};

	struct end_loop_event final : I_event<end_loop_event>
	{
		constexpr end_loop_event() noexcept = default;
	};


	// Shutdown
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct unload_event final : I_event<unload_event>
	{
		constexpr unload_event() noexcept = default;
	};

	struct exit_event final : I_event<exit_event>
	{
		constexpr exit_event() noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_