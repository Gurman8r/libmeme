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

	static engine::context * g_engine{ nullptr };
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::create_context()
	{
		return !initialized() && (g_engine = new engine::context{});
	}

	bool engine::startup(bool install_callbacks)
	{
		ML_ASSERT(initialized());

		// start lua
		if (!lua::startup())
		{
			return debug::log_error("engine failed initializing lua");
		}

		// start python
		if (!python::startup(
			g_engine->config.command_line.front(),
			g_engine->config.library_path
		))
		{
			return debug::log_error("engine failed initializing python");
		}

		// run scripts
		for (auto const & path : g_engine->config.script_list)
		{
			script{ path }();
		}

		// load plugins
		for (auto const & path : g_engine->config.plugin_list)
		{
			if (!load_plugin(path))
			{
				debug::log_error("engine failed loading plugin: {0}", path);
			}
		}

		// create window
		if (!g_engine->io.window.create(
			g_engine->config.window_title,
			g_engine->config.window_video,
			g_engine->config.window_context,
			g_engine->config.window_flags
		))
		{
			return debug::log_error("engine failed creating window");
		}

		// install window callbacks
		if (install_callbacks)
		{
			g_engine->io.window.set_char_callback([](auto, auto ... args)
			{
				event_system::fire_event<char_event>(ML_FWD(args)...);
			});

			g_engine->io.window.set_cursor_enter_callback([](auto, auto ... args)
			{
				event_system::fire_event<cursor_enter_event>(ML_FWD(args)...);
			});

			g_engine->io.window.set_cursor_pos_callback([](auto, auto ... args)
			{
				event_system::fire_event<cursor_pos_event>(ML_FWD(args)...);
			});

			g_engine->io.window.set_error_callback([](auto ... args)
			{
				event_system::fire_event<window_error_event>(ML_FWD(args)...);
			});

			g_engine->io.window.set_frame_size_callback([](auto, auto ... args)
			{
				event_system::fire_event<frame_size_event>(ML_FWD(args)...);
			});

			g_engine->io.window.set_key_callback([](auto, auto ... args)
			{
				event_system::fire_event<key_event>(ML_FWD(args)...);
			});

			g_engine->io.window.set_mouse_callback([](auto, auto ... args)
			{
				event_system::fire_event<mouse_event>(ML_FWD(args)...);
			});

			g_engine->io.window.set_scroll_callback([](auto, auto ... args)
			{
				event_system::fire_event<scroll_event>(ML_FWD(args)...);
			});

			g_engine->io.window.set_window_close_callback([](auto)
			{
				event_system::fire_event<window_close_event>();
			});

			g_engine->io.window.set_window_focus_callback([](auto, auto ... args)
			{
				event_system::fire_event<window_focus_event>(ML_FWD(args)...);
			});

			g_engine->io.window.set_window_pos_callback([](auto, auto ... args)
			{
				event_system::fire_event<window_pos_event>(ML_FWD(args)...);
			});

			g_engine->io.window.set_window_size_callback([](auto, auto ... args)
			{
				event_system::fire_event<window_size_event>(ML_FWD(args)...);
			});
		}
		
		return true;
	}

	void engine::shutdown()
	{
		ML_ASSERT(initialized());
		
		g_engine->io.plugin_files.clear();
		g_engine->io.plugin_libs.for_each([](auto const &, auto & p) { delete p; });
		g_engine->io.plugin_libs.clear();
		
		if (g_engine->io.window.is_open())
		{
			g_engine->io.window.destroy();

			window::terminate();
		}
		
		if (python::initialized())
		{
			python::shutdown();
		}
		
		if (lua::initialized())
		{
			lua::shutdown();
		}
		
		delete g_engine;
		g_engine = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::running()
	{
		return initialized() && g_engine->io.window.is_open();
	}

	void engine::begin_loop()
	{
		ML_ASSERT(initialized());

		g_engine->io.delta_time = g_engine->io.loop_timer.elapsed().count<float_t>();

		g_engine->io.loop_timer.restart();

		window::poll_events();
	}

	void engine::begin_draw()
	{
		ML_ASSERT(initialized());

		g_engine->io.window.clear_color(colors::black);

		g_engine->io.window.viewport({ {}, g_engine->io.window.get_frame_size() });

		constexpr render_states states{
		}; states();
	}

	void engine::end_draw()
	{
		ML_ASSERT(initialized());

		if (g_engine->io.window.get_flags() & WindowFlags_DoubleBuffered)
		{
			g_engine->io.window.swap_buffers();
		}
		else
		{
			GL::flush();
		}
	}

	void engine::end_loop()
	{
		ML_ASSERT(initialized());

		++g_engine->io.frame_count;

		g_engine->io.frame_rate = g_engine->io.fps_tracker(g_engine->io.delta_time);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::load_plugin(fs::path const & path)
	{
		ML_ASSERT(initialized());

		// check file name already loaded
		if (auto const file{ g_engine->io.plugin_files.insert(path.filename()) }; file.second)
		{
			// load library
			if (auto lib{ make_shared_library(*file.first) })
			{
				// load plugin
				if (auto const inst{ lib.call_function<plugin *>("ml_plugin_main") })
				{
					return (*g_engine->io.plugin_libs.insert(std::move(lib), *inst).second);
				}
			}
			g_engine->io.plugin_files.erase(file.first);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::initialized() noexcept
	{
		return g_engine;
	}

	engine::context const * const engine::get_context() noexcept
	{
		ML_ASSERT(initialized());
		return g_engine;
	}

	engine::config & engine::get_config() noexcept
	{
		ML_ASSERT(initialized());
		return g_engine->config;
	}

	engine::io & engine::get_io() noexcept
	{
		ML_ASSERT(initialized());
		return g_engine->io;
	}

	duration const & engine::get_time() noexcept
	{
		ML_ASSERT(initialized());
		return g_engine->io.main_timer.elapsed();
	}

	render_window & engine::get_window() noexcept
	{
		ML_ASSERT(initialized());
		return g_engine->io.window;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}