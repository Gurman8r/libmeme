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

	engine::context const * engine::create_context()
	{
		if (g_engine) return nullptr;

		return (g_engine = new engine::context{});
	}

	bool engine::initialized() noexcept
	{
		return g_engine;
	}

	bool engine::startup(bool install_callbacks)
	{
		if (!g_engine) return false;

		// start lua
		if (!lua::startup())
		{
			return debug::log_error("engine failed initializing lua");
		}

		// start python
		if (!python::startup(
			g_engine->config.program_name,
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
		if (!g_engine->window.create(
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
			g_engine->window.set_char_callback([](auto, auto ch)
			{
				event_system::fire_event<char_event>(ch);
			});

			g_engine->window.set_cursor_enter_callback([](auto, auto entered)
			{
				event_system::fire_event<cursor_enter_event>(entered);
			});

			g_engine->window.set_cursor_pos_callback([](auto, auto x, auto y)
			{
				event_system::fire_event<cursor_pos_event>(x, y);
			});

			g_engine->window.set_error_callback([](auto code, auto desc)
			{
				event_system::fire_event<window_error_event>(code, desc);
			});

			g_engine->window.set_frame_size_callback([](auto, auto w, auto h)
			{
				event_system::fire_event<frame_size_event>(w, h);
			});

			g_engine->window.set_key_callback([](auto, auto button, auto scan, auto action, auto mods)
			{
				event_system::fire_event<key_event>(button, scan, action, mods);
			});

			g_engine->window.set_mouse_callback([](auto, auto button, auto action, auto mods)
			{
				event_system::fire_event<mouse_event>(button, action, mods);
			});

			g_engine->window.set_scroll_callback([](auto, auto x, auto y)
			{
				event_system::fire_event<scroll_event>(x, y);
			});

			g_engine->window.set_window_close_callback([](auto)
			{
				event_system::fire_event<window_close_event>();
			});

			g_engine->window.set_window_focus_callback([](auto, auto focused)
			{
				event_system::fire_event<window_focus_event>(focused);
			});

			g_engine->window.set_window_pos_callback([](auto, auto x, auto y)
			{
				event_system::fire_event<window_pos_event>(x, y);
			});

			g_engine->window.set_window_size_callback([](auto, auto w, auto h)
			{
				event_system::fire_event<window_size_event>(w, h);
			});
		}
		
		return true;
	}

	void engine::shutdown()
	{
		if (!g_engine) return;
		
		g_engine->lib_filenames.clear();
		g_engine->lib_instances.for_each([](auto const &, auto & p) { delete p; });
		g_engine->lib_instances.clear();
		
		if (g_engine->window.is_open())
		{
			g_engine->window.destroy();

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
		return g_engine && (g_engine->window.is_open());
	}

	void engine::begin_loop()
	{
		if (!g_engine) return;

		g_engine->io.delta_time = g_engine->loop_timer.elapsed().count();

		g_engine->loop_timer.stop().start();

		window::poll_events();
	}

	void engine::begin_draw()
	{
		if (!g_engine) return;

		g_engine->window.clear_color(colors::black);

		g_engine->window.viewport({ {}, g_engine->window.get_frame_size() });

		constexpr render_states states{
		}; states();
	}

	void engine::end_loop()
	{
		if (!g_engine) return;

		if (g_engine->window.get_flags() & WindowFlags_DoubleBuffered)
		{
			g_engine->window.swap_buffers();
		}
		else
		{
			GL::flush();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::load_plugin(fs::path const & path)
	{
		if (!g_engine) return false;

		// get existing paths
		auto & files{ g_engine->lib_filenames };

		// check file name already loaded
		if (auto const f{ files.insert(path.filename()) }; f.second)
		{
			// load library
			if (auto l{ make_shared_library(*f.first) })
			{
				// load plugin
				if (auto const p{ l.call_function<plugin *>("ml_plugin_main") })
				{
					return (*g_engine->lib_instances.insert(std::move(l), *p).second);
				}
			}
			files.erase(f.first);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	engine::context const * engine::get_context() noexcept
	{
		return g_engine;
	}

	engine::config & engine::get_config() noexcept
	{
		return g_engine->config;
	}

	engine::io & engine::get_io() noexcept
	{
		return g_engine->io;
	}

	duration const & engine::get_time() noexcept
	{
		return g_engine->main_timer.elapsed();
	}

	render_window & engine::get_window() noexcept
	{
		return g_engine->window;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}