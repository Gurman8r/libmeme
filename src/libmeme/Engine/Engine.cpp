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
		return !g_engine && (g_engine = new engine::context{});
	}

	bool engine::initialized() noexcept
	{
		return g_engine;
	}

	engine::context * const engine::get_context() noexcept
	{
		return g_engine;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::startup(bool install_callbacks)
	{
		ML_ASSERT(initialized());

		// start main timer
		ref().main_timer.start();

		// start lua
		if (!lua::startup())
		{
			return debug::log_error("engine failed initializing lua");
		}

		// start python
		if (!python::startup(get_config().program_name, get_config().library_path))
		{
			return debug::log_error("engine failed initializing python");
		}

		// run scripts
		for (auto const & path : get_config().script_list)
		{
			script{ path }();
		}

		// load plugins
		for (auto const & path : get_config().plugin_list)
		{
			if (!load_plugin(path))
			{
				debug::log_error("engine failed loading plugin: {0}", path);
			}
		}

		// create window
		if (!get_window().create(
			get_config().window_title,
			get_config().window_video,
			get_config().window_context,
			get_config().window_flags
		))
		{
			return debug::log_error("engine failed creating window");
		}

		// install window callbacks
		if (install_callbacks)
		{
			get_window().set_char_callback([](auto, auto ... args)
			{
				event_system::fire_event<char_event>(ML_FWD(args)...);
			});

			get_window().set_cursor_enter_callback([](auto, auto ... args)
			{
				event_system::fire_event<cursor_enter_event>(ML_FWD(args)...);
			});

			get_window().set_cursor_pos_callback([](auto, auto ... args)
			{
				event_system::fire_event<cursor_pos_event>(ML_FWD(args)...);
			});

			get_window().set_error_callback([](auto ... args)
			{
				event_system::fire_event<window_error_event>(ML_FWD(args)...);
			});

			get_window().set_frame_size_callback([](auto, auto ... args)
			{
				event_system::fire_event<frame_size_event>(ML_FWD(args)...);
			});

			get_window().set_key_callback([](auto, auto ... args)
			{
				event_system::fire_event<key_event>(ML_FWD(args)...);
			});

			get_window().set_mouse_callback([](auto, auto ... args)
			{
				event_system::fire_event<mouse_event>(ML_FWD(args)...);
			});

			get_window().set_scroll_callback([](auto, auto ... args)
			{
				event_system::fire_event<scroll_event>(ML_FWD(args)...);
			});

			get_window().set_window_close_callback([](auto)
			{
				event_system::fire_event<window_close_event>();
			});

			get_window().set_window_focus_callback([](auto, auto ... args)
			{
				event_system::fire_event<window_focus_event>(ML_FWD(args)...);
			});

			get_window().set_window_pos_callback([](auto, auto ... args)
			{
				event_system::fire_event<window_pos_event>(ML_FWD(args)...);
			});

			get_window().set_window_size_callback([](auto, auto ... args)
			{
				event_system::fire_event<window_size_event>(ML_FWD(args)...);
			});
		}
		
		return true;
	}

	void engine::shutdown()
	{
		ML_ASSERT(initialized());
		
		ref().plugin_files.clear();
		ref().plugin_libs.for_each([](auto const &, plugin * p)
		{
			memory_manager::deallocate(p);

			//if (p) delete p;
		});
		ref().plugin_libs.clear();
		
		if (get_window().is_open())
		{
			get_window().destroy();

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

	void engine::begin_loop()
	{
		ML_ASSERT(initialized());

		ref().io.delta_time = ref().loop_timer.elapsed().count<float_t>();

		ref().loop_timer.stop().start();

		window::poll_events();
	}

	void engine::end_loop()
	{
		ML_ASSERT(initialized());

		++ref().io.frame_count;

		ref().io.frame_rate = ref().fps_tracker(ref().io.delta_time);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void engine::begin_draw()
	{
		ML_ASSERT(initialized());

		get_window().clear_color(colors::black);

		get_window().viewport({ {}, get_window().get_frame_size() });

		constexpr render_states states{
		}; states();
	}

	void engine::end_draw()
	{
		ML_ASSERT(initialized());

		if ML_UNLIKELY(get_window() & WindowFlags_DoubleBuffered)
		{
			get_window().swap_buffers();
		}
		else
		{
			GL::flush();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::load_plugin(fs::path const & path)
	{
		ML_ASSERT(initialized());

		// check file name already loaded
		if (auto const file{ ref().plugin_files.insert(path.filename()) }; file.second)
		{
			// load library
			if (auto lib{ make_shared_library(*file.first) })
			{
				// load plugin
				if (auto const inst{ lib.call_function<plugin *>("ml_plugin_main") })
				{
					return (*ref().plugin_libs.insert(std::move(lib), *inst).second);
				}
			}
			ref().plugin_files.erase(file.first);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}