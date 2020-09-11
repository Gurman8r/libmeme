#include <libmeme/Engine/Application.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Engine/EngineEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	application * application::g_app{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	application::application(system_context * sys)
		: system_object	{ sys }
		, m_plugins		{ sys->mem->allocator() }
	{
		ML_assert(!g_app && (g_app = this));

		// systems
		auto & mem	{ *sys->mem	};
		auto & io	{ *sys->io	};
		auto & bus	{ *sys->bus	};
		auto & win	{ *sys->win	};
		auto & ed	{ *sys->ed	};
		auto & scr	{ *sys->scr	};

		// setup window
		ML_assert(win.open(io.conf["window"]));
		win.set_char_callback([](auto ... x)			{ g_app->get_bus()->fire<window_char_event>(x...); });
		win.set_char_mods_callback([](auto ... x)		{ g_app->get_bus()->fire<window_char_mods_event>(x...); });
		win.set_close_callback([](auto ... x)			{ g_app->get_bus()->fire<window_close_event>(x...); });
		win.set_cursor_enter_callback([](auto ... x)	{ g_app->get_bus()->fire<window_cursor_enter_event>(x...); });
		win.set_cursor_position_callback([](auto ... x) { g_app->get_bus()->fire<window_cursor_pos_event>(x...); });
		win.set_content_scale_callback([](auto ... x)	{ g_app->get_bus()->fire<window_content_scale_event>(x...); });
		win.set_drop_callback([](auto ... x)			{ g_app->get_bus()->fire<window_drop_event>(x...); });
		win.set_error_callback([](auto ... x)			{ g_app->get_bus()->fire<window_error_event>(x...); });
		win.set_focus_callback([](auto ... x)			{ g_app->get_bus()->fire<window_focus_event>(x...); });
		win.set_framebuffer_size_callback([](auto ... x){ g_app->get_bus()->fire<window_framebuffer_size_event>(x...); });
		win.set_iconify_callback([](auto ... x)			{ g_app->get_bus()->fire<window_iconify_event>(x...); });
		win.set_key_callback([](auto ... x)				{ g_app->get_bus()->fire<window_key_event>(x...); });
		win.set_maximize_callback([](auto ... x)		{ g_app->get_bus()->fire<window_maximize_event>(x...);  });
		win.set_mouse_callback([](auto ... x)			{ g_app->get_bus()->fire<window_mouse_event>(x...); });
		win.set_position_callback([](auto ... x)		{ g_app->get_bus()->fire<window_pos_event>(x...); });
		win.set_refresh_callback([](auto ... x)			{ g_app->get_bus()->fire<window_refresh_event>(x...); });
		win.set_scroll_callback([](auto ... x)			{ g_app->get_bus()->fire<window_scroll_event>(x...); });
		win.set_size_callback([](auto ... x)			{ g_app->get_bus()->fire<window_size_event>(x...); });

		// setup editor
		ML_assert(ed.startup());
		ed.load_style(io.path2(io.conf["editor"]["style"]));
		io.conf["editor"]["dockspace"]["visible"].get_to(ed.get_dockspace().visible);
		io.conf["editor"]["dockspace"]["menubar"].get_to(ed.get_dockspace().menubar);

		// install plugins
		for (auto const & path : io.conf["plugins"]["files"])
		{
			install_plugin(path);
		}

		// execute scripts
		for (auto const & path : io.conf["scripts"]["files"])
		{
			scr.do_file(io.path2(path));
		}
	}

	application::~application() noexcept
	{
		ML_assert(g_app == this && !(g_app = nullptr));

		while (!m_plugins.get<plugin_id>().empty())
		{
			uninstall_plugin(m_plugins.get<plugin_id>().back());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t application::run()
	{
		// systems
		auto const	sys	{ get_system()	};
		auto &		mem	{ *sys->mem		};
		auto &		io	{ *sys->io		};
		auto &		bus	{ *sys->bus		};
		auto &		win	{ *sys->win		};
		auto &		ed	{ *sys->ed		};
		auto &		scr	{ *sys->scr		};

		// run check
		if (!m_running && win.is_open()) { m_running = true; }
		else { return EXIT_FAILURE; } ML_defer(&) { m_running = false; };

		// loading
		bus.fire<load_event>(this); ML_defer(&) { bus.fire<unload_event>(this); };

		// loop
		while (win.is_open())
		{
			// timers
			io.loop_timer.restart();
			auto const dt{ (float_t)io.delta_time.count() };
			io.fps_accum += dt - io.fps_times[io.fps_index];
			io.fps_times[io.fps_index] = dt;
			io.fps_index = (io.fps_index + 1) % io.fps_times.size();
			io.frame_rate = (0.f < io.fps_accum) ? 1.f / (io.fps_accum / (float_t)io.fps_times.size()) : FLT_MAX;
			ML_defer(&io)
			{
				performance::refresh_samples();
				io.delta_time = io.loop_timer.elapsed();
			};
			
			// update
			window::poll_events();
			bus.fire<update_event>(this);

			// gui
			ed.new_frame();
			bus.fire<gui_event>(&ed);
			ed.render_frame();

			// end frame
			if (win.has_hints(window_hints_doublebuffer))
			{
				window::swap_buffers(win.get_handle());
			}
		}

		return EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin_id application::install_plugin(fs::path const & path, void * user)
	{
		// load library
		if (auto const id{ std::invoke([&, &lib = shared_library{ path }]() -> plugin_id
		{
			return (!lib || m_plugins.contains<shared_library>(lib)) ? nullptr :
				std::get<plugin_id &>(m_plugins.push_back
				(
					ML_handle(plugin_id, lib.hash()), lib.path(), std::move(lib), nullptr,
					plugin_api
					{
						lib.proc<plugin *, application *, void *>("ml_plugin_attach"),
						lib.proc<void, application *, plugin *>("ml_plugin_detach"),
					}
				));
		}) })
		// load plugin
		{
			if (plugin * ptr{ m_plugins.back<plugin_api>().attach(this, user) })
			{
				m_plugins.back<manual<plugin>>().reset(ptr);

				return id;
			}
		}
		// failed
		return nullptr;
	}

	bool application::uninstall_plugin(plugin_id value)
	{
		if (!value) { return false; }
		else if (auto const it{ m_plugins.find<plugin_id>(value) }
		; it == m_plugins.end<plugin_id>()) { return false; }
		else
		{
			auto const i{ m_plugins.index_of<plugin_id>(it) };

			plugin * ptr{ m_plugins.at<manual<plugin>>(i).release() };

			m_plugins.at<plugin_api>(i).detach(this, ptr);

			m_plugins.erase(i);

			return true;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}