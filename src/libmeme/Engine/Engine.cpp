#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Python.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Platform/WindowEvents.hpp>
#include <libmeme/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static void install_event_callbacks(Window & window)
	{
		window.set_char_callback([](auto, auto ch)
		{
			EventSystem::fire_event<CharEvent>(ch);
		});
		window.set_cursor_enter_callback([](auto, auto entered)
		{
			EventSystem::fire_event<CursorEnterEvent>(entered);
		});
		window.set_cursor_pos_callback([](auto, auto x, auto y)
		{
			EventSystem::fire_event<CursorPosEvent>(x, y);
		});
		window.set_error_callback([](auto code, auto desc)
		{
			EventSystem::fire_event<WindowErrorEvent>(code, desc);
		});
		window.set_frame_size_callback([](auto, auto w, auto h)
		{
			EventSystem::fire_event<FrameSizeEvent>(w, h);
		});
		window.set_key_callback([](auto, auto button, auto scan, auto action, auto mods)
		{
			EventSystem::fire_event<KeyEvent>(button, scan, action, mask8_t{ {
				(mods & ML_MOD_SHIFT),
				(mods & ML_MOD_CTRL),
				(mods & ML_MOD_ALT),
				(mods & ML_MOD_SUPER),
				(mods & ML_MOD_CAPSLOCK),
				(mods & ML_MOD_NUMLOCK)
			} });
		});
		window.set_mouse_callback([](auto, auto button, auto action, auto mods)
		{
			EventSystem::fire_event<MouseEvent>(button, action, mods);
		});
		window.set_scroll_callback([](auto, auto x, auto y)
		{
			EventSystem::fire_event<ScrollEvent>(x, y);
		});
		window.set_window_close_callback([](auto)
		{
			EventSystem::fire_event<WindowCloseEvent>();
		});
		window.set_window_focus_callback([](auto, auto focused)
		{
			EventSystem::fire_event<WindowFocusEvent>(focused);
		});
		window.set_window_pos_callback([](auto, auto x, auto y)
		{
			EventSystem::fire_event<WindowPosEvent>(x, y);
		});
		window.set_window_size_callback([](auto, auto w, auto h)
		{
			EventSystem::fire_event<WindowSizeEvent>(w, h);
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderWindow Engine::s_window{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Engine::startup(path_t const & name, path_t const & home)
	{
		return Lua::startup() && Python::startup(name, home);
	}

	bool Engine::create_window(CreateWindowSettings const & settings)
	{
		if (s_window.create(settings.title, settings.display, settings.style, settings.context))
		{
			install_event_callbacks(s_window);

			return true;
		}
		return false;
	}

	bool Engine::running()
	{
		return s_window.is_open();
	}

	void Engine::begin_loop()
	{
		s_window.poll_events();
	}

	void Engine::end_loop()
	{
		s_window.swap_buffers();
	}

	void Engine::shutdown()
	{
		s_window.destroy();

		s_window.terminate();

		Python::shutdown();

		Lua::shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}