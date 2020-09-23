#include <libmeme/Client/Application.hpp>
#include <libmeme/Client/ClientEvents.hpp>
#include <libmeme/Window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	application * application::g_app{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	application::application(system_context * sys)
		: system_object	{ sys }
		, m_running		{}
		, m_mods		{ sys->mem->get_allocator() }
	{
		// singleton
		ML_assert(!g_app && (g_app = this));

		// systems
		auto & io	{ *get_io() };
		auto & bus	{ *get_bus() };
		auto & win	{ *get_win() };
		auto & gui	{ *get_gui() };

		// events
		subscribe<	window_cursor_pos_event	>();
		subscribe<	window_key_event		>();
		subscribe<	window_mouse_event		>();

		// setup window
		ML_assert(win.open(io.prefs["window"]));
		win.set_char_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_char_event>(x...); });
		win.set_char_mods_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_char_mods_event>(x...); });
		win.set_close_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_close_event>(x...); });
		win.set_cursor_enter_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_cursor_enter_event>(x...); });
		win.set_cursor_position_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_cursor_pos_event>(x...); });
		win.set_content_scale_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_content_scale_event>(x...); });
		win.set_drop_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_drop_event>(x...); });
		win.set_error_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_error_event>(x...); });
		win.set_focus_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_focus_event>(x...); });
		win.set_framebuffer_size_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_framebuffer_size_event>(x...); });
		win.set_iconify_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_iconify_event>(x...); });
		win.set_key_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_key_event>(x...); });
		win.set_maximize_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_maximize_event>(x...); });
		win.set_mouse_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_mouse_event>(x...); });
		win.set_position_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_pos_event>(x...); });
		win.set_refresh_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_refresh_event>(x...); });
		win.set_scroll_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_scroll_event>(x...); });
		win.set_size_callback([](auto ... x) noexcept { g_app->get_bus()->fire<window_size_event>(x...); });

		// setup editor
		ML_assert(gui.startup(io.prefs["gui"]["instclbk"]));
		gui.load_style(io.path2(io.prefs["gui"]["style"]));
		io.prefs["gui"]["menubar"]["enabled"].get_to(gui.get_menubar().enabled);
		io.prefs["gui"]["dockspace"]["enabled"].get_to(gui.get_docker().enabled);
	}

	application::~application() noexcept
	{
		// singleton
		ML_assert(g_app == this && !(g_app = nullptr));

		// uninstall plugins
		while (!m_mods.get<plugin_id>().empty())
		{
			uninstall_plugin(m_mods.get<plugin_id>().back());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void application::on_event(event const & value)
	{
		switch (auto const sys{ get_sys() }; value)
		{
		case window_cursor_pos_event::ID: {
			auto const & ev{ (window_cursor_pos_event const &)value };
			sys->io->cursor = { ev.x, ev.y };
		} break;

		case window_key_event::ID: {
			auto const & ev{ (window_key_event const &)value };
			sys->io->keys[ev.key] = ev.action;
		} break;

		case window_mouse_event::ID: {
			auto const & ev{ (window_mouse_event const &)value };
			sys->io->mouse[ev.button] = ev.action;
		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t application::run()
	{
		// systems
		auto const	sys	{ get_sys()	}; ML_assert(sys);
		auto &		mem	{ *sys->mem	};
		auto &		io	{ *sys->io	};
		auto &		bus	{ *sys->bus	};
		auto &		win	{ *sys->win	};
		auto &		gui	{ *sys->gui };

		// running
		if (m_running || !win.is_open()) { return EXIT_FAILURE; }
		else { m_running = true; } ML_defer(&) { m_running = false; };

		// load/unload
		bus.fire<app_load_event>(this); ML_defer(&) { bus.fire<app_unload_event>(this); };

		// main loop
		if (!win.is_open()) { return EXIT_FAILURE; }
		do
		{
			// timers
			io.loop_timer.restart();
			auto const dt{ (float_t)io.frame_time.count() };
			io.fps_accum += dt - io.fps_times[io.fps_index];
			io.fps_times[io.fps_index] = dt;
			io.fps_index = (io.fps_index + 1) % io.fps_times.size();
			io.frame_rate = (0.f < io.fps_accum) ? 1.f / (io.fps_accum / (float_t)io.fps_times.size()) : FLT_MAX;
			ML_defer(&io) { io.frame_time = io.loop_timer.elapsed(); };

			// poll events
			window::poll_events();

			// update
			bus.fire<app_update_event>(this);

			// gui
			gui.new_frame();
			gui.render_frame();

			// swap buffers
			if (win.has_hints(window_hints_doublebuffer))
			{
				window::swap_buffers(win.get_handle());
			}
		}
		while (win.is_open());
		return EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin_id application::install_plugin(fs::path const & path, void * user)
	{
		// check exists
		if (has_plugin(path))
		{
			return nullptr;
		}
		// load library
		else if (auto const id{ std::invoke([&, &lib = shared_library{ path }]() -> plugin_id
		{
			return (!lib || m_mods.contains<shared_library>(lib)) ? nullptr :
				std::get<plugin_id &>(m_mods.push_back
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
			if (auto const p{ m_mods.back<plugin_api>().attach(this, user) })
			{
				m_mods.back<manual<plugin>>().reset(p);

				return id;
			}
		}
		// failed
		return nullptr;
	}

	bool application::uninstall_plugin(plugin_id value)
	{
		if (!value) { return false; }
		else if (auto const it{ m_mods.find<plugin_id>(value) }
		; it == m_mods.end<plugin_id>()) { return false; }
		else
		{
			auto const i{ m_mods.index_of<plugin_id>(it) };

			auto const p{ m_mods.at<manual<plugin>>(i).release() };

			m_mods.at<plugin_api>(i).detach(this, p);

			m_mods.erase(i);

			return true;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}