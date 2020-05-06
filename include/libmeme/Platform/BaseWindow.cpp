#include <libmeme/Platform/BaseWindow.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void base_window::install_default_callbacks()
	{
		if (!is_open()) { return; }

		set_char_callback([](auto, auto ... args)
		{
			event_system::fire_event<char_event>(ML_forward(args)...);
		});

		set_cursor_enter_callback([](auto, auto ... args)
		{
			event_system::fire_event<cursor_enter_event>(ML_forward(args)...);
		});

		set_cursor_pos_callback([](auto, auto ... args)
		{
			event_system::fire_event<cursor_position_event>(ML_forward(args)...);
		});

		set_error_callback([](auto ... args)
		{
			event_system::fire_event<window_error_event>(ML_forward(args)...);
		});

		set_frame_size_callback([](auto, auto ... args)
		{
			event_system::fire_event<frame_size_event>(ML_forward(args)...);
		});

		set_key_callback([](auto, auto ... args)
		{
			event_system::fire_event<key_event>(ML_forward(args)...);
		});

		set_mouse_callback([](auto, auto ... args)
		{
			event_system::fire_event<mouse_event>(ML_forward(args)...);
		});

		set_scroll_callback([](auto, auto ... args)
		{
			event_system::fire_event<scroll_event>(ML_forward(args)...);
		});

		set_window_close_callback([](auto)
		{
			event_system::fire_event<window_close_event>();
		});

		set_window_focus_callback([](auto, auto ... args)
		{
			event_system::fire_event<window_focus_event>(ML_forward(args)...);
		});

		set_window_pos_callback([](auto, auto ... args)
		{
			event_system::fire_event<window_position_event>(ML_forward(args)...);
		});

		set_window_size_callback([](auto, auto ... args)
		{
			event_system::fire_event<window_size_event>(ML_forward(args)...);
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}