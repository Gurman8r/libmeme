#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/Python.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Engine/Script.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Renderer/RenderStates.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct engine_plugins
	{
		ds::flat_set<fs::path> files;

		ds::flat_map<shared_library, plugin *> libs;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static engine::config	s_engine_config		{};
	static engine::IO		s_engine_io			{};
	static timer			s_main_timer		{};
	static timer			s_loop_timer		{};
	static engine_plugins	s_plugins			{};
	static render_window	s_window			{};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::startup(bool install_callbacks)
	{
		if (running())
			return false;

		s_main_timer.start();

		if (!lua::startup())
			return debug::log_error("Failed initializing lua");

		if (!python::startup(s_engine_config.program_name, s_engine_config.library_path))
			return debug::log_error("Failed initializing python");

		//for (auto const & filename : s.boot_scripts)
		//{
		//	script{ filename }();
		//}

		if (!s_window.create(
			s_engine_config.window_title, 
			s_engine_config.window_video, 
			s_engine_config.window_context, 
			s_engine_config.window_flags))
		{
			return false;
		}

		if (install_callbacks)
		{
			s_window.set_char_callback([](auto, auto ch)
			{
				event_system::fire_event<char_event>(ch);
			});

			s_window.set_cursor_enter_callback([](auto, auto entered)
			{
				event_system::fire_event<cursor_enter_event>(entered);
			});

			s_window.set_cursor_pos_callback([](auto, auto x, auto y)
			{
				event_system::fire_event<cursor_pos_event>(x, y);
			});

			s_window.set_error_callback([](auto code, auto desc)
			{
				event_system::fire_event<window_error_event>(code, desc);
			});

			s_window.set_frame_size_callback([](auto, auto w, auto h)
			{
				event_system::fire_event<frame_size_event>(w, h);
			});

			s_window.set_key_callback([](auto, auto button, auto scan, auto action, auto mods)
			{
				event_system::fire_event<key_event>(button, scan, action, mods);
			});

			s_window.set_mouse_callback([](auto, auto button, auto action, auto mods)
			{
				event_system::fire_event<mouse_event>(button, action, mods);
			});

			s_window.set_scroll_callback([](auto, auto x, auto y)
			{
				event_system::fire_event<scroll_event>(x, y);
			});

			s_window.set_window_close_callback([](auto)
			{
				event_system::fire_event<window_close_event>();
			});

			s_window.set_window_focus_callback([](auto, auto focused)
			{
				event_system::fire_event<window_focus_event>(focused);
			});

			s_window.set_window_pos_callback([](auto, auto x, auto y)
			{
				event_system::fire_event<window_pos_event>(x, y);
			});

			s_window.set_window_size_callback([](auto, auto w, auto h)
			{
				event_system::fire_event<window_size_event>(w, h);
			});
		}
		
		return true;
	}

	void engine::shutdown()
	{
		s_plugins.files.clear();
		s_plugins.libs.for_each([](auto const &, auto & p) { delete p; });
		s_plugins.libs.clear();
		s_window.destroy();
		window::terminate();
		python::shutdown();
		lua::shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::running()
	{
		return s_window.is_open();
	}

	void engine::begin_loop()
	{
		get_io().delta_time = s_loop_timer.elapsed().count();
		s_loop_timer.stop().start();
		window::poll_events();
	}

	void engine::begin_draw()
	{
		s_window.clear_color(colors::black);
		s_window.viewport({ {}, s_window.get_frame_size() });
		constexpr render_states states{
		}; states(); // default states
	}

	void engine::end_loop()
	{
		if (s_window.get_flags() & WindowFlags_DoubleBuffered)
		{
			s_window.swap_buffers();
		}
		else
		{
			GL::flush();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::load_plugin(fs::path const & filename)
	{
		if (auto const file{ s_plugins.files.insert(filename) }; file.second)
		{
			if (auto lib{ make_shared_library(*file.first) })
			{
				if (auto const inst{ lib.call_function<plugin *>("ml_plugin_main") })
				{
					return (*s_plugins.libs.insert(std::move(lib), inst).first.second);
				}
			}
			s_plugins.files.erase(file.first);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD engine::config & engine::get_config() noexcept
	{
		return s_engine_config;
	}

	ML_NODISCARD engine::IO & engine::get_io() noexcept
	{
		return s_engine_io;
	}

	ML_NODISCARD duration const & engine::get_time() noexcept
	{
		return s_main_timer.elapsed();
	}

	ML_NODISCARD render_window & engine::get_window() noexcept
	{
		return s_window;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}