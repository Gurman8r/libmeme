#include <libmeme/Engine/Application.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/ImGui.hpp>
#include <libmeme/Engine/API_Embed.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>

// GLFW / OpenGL3
#if defined(ML_IMPL_WINDOW_GLFW) && defined(ML_IMPL_RENDERER_OPENGL)
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#define ML_ImGui_Init_Platform(wh, ic)	ImGui_ImplGlfw_InitForOpenGL((struct GLFWwindow *)wh, ic)
#define ML_ImGui_Init_Renderer()		ImGui_ImplOpenGL3_Init("#version 130")
#define ML_ImGui_Shutdown()				ML_scope(&){ ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplGlfw_Shutdown(); }
#define ML_ImGui_NewFrame()				ML_scope(&){ ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame(); }
#define ML_ImGui_RenderDrawData(x)		ImGui_ImplOpenGL3_RenderDrawData(x)

#else
#	error "imgui is unavailable"
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	application::application(json const & j, allocator_type alloc) noexcept
		: m_sys		{ memory::get_singleton(), event_bus::get_singleton(), performance::get_singleton() }
		, m_time	{}
		, m_config	{ json{ j } }
		, m_path	{ fs::current_path(), j["path"]["content"].get<fs::path>() }
		, m_window	{ j["window"].get<window_settings>() }
		, m_gui		{ m_window, alloc }
		, m_plugins	{ alloc }
	{
		// EVENTS
		event_bus::add_listener< begin_loop_event	>(this);
		event_bus::add_listener< begin_draw_event	>(this);
		event_bus::add_listener< begin_gui_event	>(this);
		event_bus::add_listener< draw_gui_event		>(this);
		event_bus::add_listener< end_gui_event		>(this);
		event_bus::add_listener< end_draw_event		>(this);
		event_bus::add_listener< end_loop_event		>(this);

		// PYTHON
		{
			ML_assert(!Py_IsInitialized());

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

			Py_SetPythonHome(j["scripts"]["library"].get<fs::path>().c_str());

			Py_InitializeEx(1);

			ML_assert(Py_IsInitialized());
		}

		// STYLE
		if (j.contains("path") && j["path"].contains("style"))
		{
			m_gui.load_style(path2(j["path"]["style"].get<fs::path>()));
		}

		// PLUGINS
		for (auto const & e : j["plugins"]["files"])
		{
			install_plugin(e.get<fs::path>());
		}

		// SCRIPTS
		for (auto const & e : j["scripts"]["files"])
		{
			execute_file(path2(e.get<fs::path>()));
		}
	}

	application::~application() noexcept
	{
		ML_assert(Py_FinalizeEx() == EXIT_SUCCESS);

		ML_assert(!Py_IsInitialized());

		m_gui.main_menu_bar().menus.clear();

		m_gui.shutdown();

		m_window.close();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void application::on_event(event const & ev)
	{
		switch (ev.ID)
		{
		case hashof_v<begin_loop_event>: {
			m_time.loop_timer.restart();

			m_time.frame_rate = std::invoke([&, dt = (float_t)m_time.delta_time.count()]() noexcept
			{
				m_time.fps_accum += dt - m_time.fps_times[m_time.fps_index];
				m_time.fps_times[m_time.fps_index] = dt;
				m_time.fps_index = (m_time.fps_index + 1) % m_time.fps_times.size();
				return (0.f < m_time.fps_accum) ? 1.f / (m_time.fps_accum / (float_t)m_time.fps_times.size()) : FLT_MAX;
			});

			window::poll_events();
		}break;

		case hashof_v<begin_draw_event>: {
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

		case hashof_v<begin_gui_event>: {
			m_gui.begin_frame();
		} break;

		case hashof_v<draw_gui_event>: {
			m_gui.draw_default();
		} break;

		case hashof_v<end_gui_event>: {
			m_gui.end_frame();
		} break;

		case hashof_v<end_draw_event>: {
			if (m_window.get_hints() & window_hints_doublebuffer)
			{
				window::swap_buffers(m_window.get_handle());
			}
		} break;

		case hashof_v<end_loop_event>: {
			++m_time.frame_count;
			performance::refresh_samples();
			m_time.delta_time = m_time.loop_timer.elapsed();
		}break;
		}
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
				if (auto const optl{ lib.call<plugin *>(ML_PLUGIN_MAIN, this) })
				{
					m_plugins.push_back(code, path, std::move(lib), optl.value());

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

	int32_t application::execute_file(fs::path const & value)
	{
		ML_assert(Py_IsInitialized());

		return PyRun_SimpleFileExFlags(
			std::fopen(value.string().c_str(), "r"),
			value.string().c_str(),
			true,
			nullptr);
	}

	int32_t application::execute_string(pmr::string const & value)
	{
		ML_assert(Py_IsInitialized());

		return PyRun_SimpleStringFlags(value.c_str(), nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}