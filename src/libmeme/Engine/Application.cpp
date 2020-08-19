#include <libmeme/Engine/Application.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/ImGui.hpp>
#include <libmeme/Engine/API_Embed.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	application * application::g_app{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	application::application(json const & j, allocator_type alloc) noexcept
		: m_running	{}
		, m_sys		{}
		, m_time	{}
		, m_config	{ json{ j } }
		, m_path	{ fs::current_path(), j["path"]["content"] }
		, m_window	{}
		, m_gui		{ alloc }
		, m_input	{}
		, m_plugins	{ alloc }
	{
		ML_assert("another application is already running" && (!g_app && (g_app = this)));

		// EVENTS
		event_bus::add_listener< draw_begin_event		>(this);
		event_bus::add_listener< gui_begin_event		>(this);
		event_bus::add_listener< gui_draw_event			>(this);
		event_bus::add_listener< gui_end_event			>(this);
		event_bus::add_listener< draw_end_event			>(this);
		event_bus::add_listener< key_event				>(this);
		event_bus::add_listener< mouse_event			>(this);
		event_bus::add_listener< cursor_position_event	>(this);

		// WINDOW
		ML_assert(m_window.open(j["window"]));
		{
			m_window.set_char_callback([
			](auto ... x) noexcept { event_bus::fire<char_event>(ML_forward(x)...); });

			m_window.set_char_mods_callback([
			](auto ... x) noexcept { event_bus::fire<char_mods_event>(ML_forward(x)...); });

			m_window.set_close_callback([
			](auto ... x) noexcept { event_bus::fire<close_event>(ML_forward(x)...); });

			m_window.set_cursor_enter_callback([
			](auto ... x) noexcept { event_bus::fire<cursor_enter_event>(ML_forward(x)...); });

			m_window.set_cursor_position_callback([
			](auto ... x) noexcept { event_bus::fire<cursor_position_event>(ML_forward(x)...); });

			m_window.set_content_scale_callback([
			](auto ... x) noexcept { event_bus::fire<content_scale_event>(ML_forward(x)...); });

			m_window.set_drop_callback([
			](auto ... x) noexcept { event_bus::fire<drop_event>(ML_forward(x)...); });

			m_window.set_error_callback([
			](auto ... x) noexcept { event_bus::fire<error_event>(ML_forward(x)...); });

			m_window.set_focus_callback([
			](auto ... x) noexcept { event_bus::fire<focus_event>(ML_forward(x)...); });

			m_window.set_framebuffer_size_callback([
			](auto ... x) noexcept { event_bus::fire<framebuffer_size_event>(ML_forward(x)...); });

			m_window.set_iconify_callback([
			](auto ... x) noexcept { event_bus::fire<iconify_event>(ML_forward(x)...); });

			m_window.set_key_callback([
			](auto ... x) noexcept { event_bus::fire<key_event>(ML_forward(x)...); });

			m_window.set_maximize_callback([
			](auto ... x) noexcept { event_bus::fire<maximize_event>(ML_forward(x)...);  });

			m_window.set_mouse_callback([
			](auto ... x) noexcept { event_bus::fire<mouse_event>(ML_forward(x)...); });

			m_window.set_position_callback([
			](auto ... x) noexcept { event_bus::fire<position_event>(ML_forward(x)...); });

			m_window.set_refresh_callback([
			](auto ... x) noexcept { event_bus::fire<refresh_event>(ML_forward(x)...); });

			m_window.set_scroll_callback([
			](auto ... x) noexcept { event_bus::fire<scroll_event>(ML_forward(x)...); });

			m_window.set_size_callback([
			](auto ... x) noexcept { event_bus::fire<size_event>(ML_forward(x)...); });
		}

		// GUI
		ML_assert(m_gui.startup(m_window));
		if (j["path"].contains("guistyle"))
		{
			m_gui.load_style(path2(j["path"]["guistyle"]));
		}

		// PYTHON
		{
			PyObject_SetArenaAllocator(std::invoke([&temp = PyObjectArenaAllocator{}]() noexcept
			{
				temp.alloc = [](auto, size_t s) noexcept
				{
					return pmr::get_default_resource()->allocate(s);
				};
				temp.free = [](auto, void * p, size_t s) noexcept
				{
					return pmr::get_default_resource()->deallocate(p, s);
				};
				return &temp;
			}));
			Py_SetProgramName(fs::path{ __argv[0] }.filename().c_str());
			Py_SetPythonHome(j["path"]["library"].get<fs::path>().c_str());
			Py_InitializeEx(1);
			ML_assert(Py_IsInitialized());
		}

		// PLUGINS
		for (auto const & e : j["plugins"]["files"])
		{
			install_plugin(e.get<fs::path>());
		}

		// SCRIPTS
		for (auto const & e : j["scripts"]["files"])
		{
			do_file(path2(e.get<fs::path>()));
		}
	}

	application::~application() noexcept
	{
		ML_assert(Py_FinalizeEx() == EXIT_SUCCESS);

		m_gui.shutdown();

		m_window.close();

		g_app = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t application::run(std::function<void()> const & callback)
	{
		if (m_running || !m_window.is_open()) { return EXIT_FAILURE; }

		m_running = true;

		do
		{
			m_time.loop_timer.restart();

			m_time.frame_rate = std::invoke([&, dt = (float_t)m_time.delta_time.count()]() noexcept
			{
				m_time.fps_accum += dt - m_time.fps_times[m_time.fps_index];
				m_time.fps_times[m_time.fps_index] = dt;
				m_time.fps_index = (m_time.fps_index + 1) % m_time.fps_times.size();
				return (0.f < m_time.fps_accum) ? 1.f / (m_time.fps_accum / (float_t)m_time.fps_times.size()) : FLT_MAX;
			});

			window::poll_events();

			std::invoke(callback);

			++m_time.frame_count;

			performance::refresh_samples();

			m_time.delta_time = m_time.loop_timer.elapsed();
		}
		while (m_window.is_open());

		m_running = false;

		return EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void application::on_event(event const & ev)
	{
		switch (ev)
		{
		case draw_begin_event::ID: {
			for (auto const & cmd :
			{
				gfx::render_command::set_clear_color(colors::black),
				gfx::render_command::clear(gfx::clear_color),
				gfx::render_command::set_viewport(m_window.get_framebuffer_size()),
			})
			{
				std::invoke(cmd, m_window.get_render_context().get());
			}
		} break;

		case gui_begin_event::ID: {
			m_gui.begin_frame();
		} break;

		case gui_draw_event::ID: {
			m_gui.draw_default();
		} break;

		case gui_end_event::ID: {
			m_gui.end_frame();
		} break;

		case draw_end_event::ID: {
			if (m_window.get_hints() & window_hints_doublebuffer)
			{
				window::swap_buffers(m_window.get_handle());
			}
		} break;

		case key_event::ID: {
			auto const & k{ (key_event const &)ev };
			m_input.keyboard[k.key] = k.action;
		} break;

		case mouse_event::ID: {
			auto const & m{ (mouse_event const &)ev };
			m_input.mouse[m.button] = m.action;
		} break;

		case cursor_position_event::ID: {
			auto const & c{ (cursor_position_event const &)ev };
			m_input.cursor = { c.x, c.y };
		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t application::do_file(fs::path const & value)
	{
		ML_assert(Py_IsInitialized());

		return PyRun_SimpleFileExFlags(
			std::fopen(value.string().c_str(), "r"),
			value.string().c_str(),
			true,
			nullptr);
	}

	int32_t application::do_string(pmr::string const & value)
	{
		ML_assert(Py_IsInitialized());

		return PyRun_SimpleStringFlags(value.c_str(), nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin_handle application::install_plugin(fs::path const & path)
	{
		// path empty
		if (path.empty()) { return 0; }

		// lookup file
		if (auto const code{ util::hash(path.filename().string()) }
		; m_plugins.find<hash_t>(code) == m_plugins.end<hash_t>())
		{
			// load library
			if (shared_library lib{ path })
			{
				// load plugin
				if (auto const p{ lib.call<plugin *>(ML_PLUGIN_MAIN, this) })
				{
					ML_assert((intptr_t)this < (intptr_t)*p);

					m_plugins.push_back(code, path, std::move(lib), *p);

					return ML_handle(plugin_handle, code);
				}
			}
		}

		return nullptr;
	}

	bool application::uninstall_plugin(plugin_handle value)
	{
		if (!value) { return false; }

		if (auto const it{ m_plugins.find<hash_t>(ML_handle(hash_t, value)) }
		; it != m_plugins.end<hash_t>())
		{
			m_plugins.erase(m_plugins.index_of<hash_t>(it));

			return true;
		}
		else
		{
			return false;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}