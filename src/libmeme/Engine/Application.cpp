#include <libmeme/Engine/Application.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Platform/WindowEvents.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>

#if defined(ML_IMPL_WINDOW_GLFW) && defined(ML_IMPL_RENDERER_OPENGL)
// GLFW / OpenGL3
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

	application::application(allocator_type alloc) noexcept : render_window{}
	{
		IMGUI_CHECKVERSION();

		event_system::add_listener<	begin_loop_event>(this);
		event_system::add_listener<	begin_draw_event>(this);
		event_system::add_listener<	begin_gui_event	>(this);
		event_system::add_listener<	draw_gui_event	>(this);
		event_system::add_listener<	end_gui_event	>(this);
		event_system::add_listener<	end_draw_event	>(this);
		event_system::add_listener<	end_loop_event	>(this);
	}

	application::application(window_settings const & ws, allocator_type alloc) noexcept : application{}
	{
		ML_assert(open(ws));
	}

	application::~application() noexcept
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void application::on_event(event const & ev)
	{
		switch (ev.ID)
		{
		case hashof_v<begin_loop_event>: // BEGIN LOOP
		{
			m_perf.loop_timer.restart();

			m_perf.frame_rate = std::invoke([&, dt = m_perf.delta_time.count<float_t>()]() noexcept
			{
				m_perf.fps_accum += dt - m_perf.fps_times[m_perf.fps_index];
				m_perf.fps_times[m_perf.fps_index] = dt;
				m_perf.fps_index = (m_perf.fps_index + 1) % m_perf.fps_times.size();
				return (0.f < m_perf.fps_accum) ? 1.f / (m_perf.fps_accum / (float_t)m_perf.fps_times.size()) : FLT_MAX;
			});

			poll_events();
		}
		break;

		case hashof_v<begin_draw_event>: // BEGIN DRAW
		{
			for (auto const & cmd :
			{
				gfx::render_command::set_clear_color(colors::black),

				gfx::render_command::clear(gfx::clear_color),
					
				gfx::render_command::set_viewport(get_framebuffer_size()),
			})
			{
				std::invoke(cmd, get_render_context().get());
			}
		}
		break;

		case hashof_v<begin_gui_event>: // BEGIN GUI
		{
			ML_ImGui_NewFrame();

			ImGui::NewFrame();
		}
		break;

		case hashof_v<draw_gui_event>: // DRAW GUI
		{
		}
		break;

		case hashof_v<end_gui_event>: // END GUI
		{
			ImGui::Render();
			ML_ImGui_RenderDrawData(ImGui::GetDrawData());
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				auto backup_context{ get_current_context() };
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				set_current_context(backup_context);
			}
		}
		break;

		case hashof_v<end_draw_event>: // END DRAW
		{
			if (has_hint(window_hints_double_buffer))
			{
				swap_buffers();
			}
		}
		break;

		case hashof_v<end_loop_event>: // END LOOP
		{
			++m_perf.frame_count;

			performance::refresh_samples();

			m_perf.delta_time = m_perf.loop_timer.elapsed();
		}
		break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool application::open(window_settings const & ws)
	{
		// check already open
		if (is_open()) { return debug::error("application is already open"); }

		// open render_window
		if (!render_window::open(ws)) { return debug::error("failed opening application"); }

		// install callbacks
		{
			set_char_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_char_event>(ML_forward(x)...); });

			set_char_mods_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_char_mods_event>(ML_forward(x)...); });

			set_close_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_close_event>(ML_forward(x)...); });

			set_cursor_enter_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_cursor_enter_event>(ML_forward(x)...); });

			set_cursor_position_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_cursor_position_event>(ML_forward(x)...); });

			set_content_scale_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_content_scale_event>(ML_forward(x)...); });

			set_drop_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_drop_event>(ML_forward(x)...); });

			set_focus_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_focus_event>(ML_forward(x)...); });

			set_framebuffer_size_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_framebuffer_size_event>(ML_forward(x)...); });

			set_iconify_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_iconify_event>(ML_forward(x)...); });

			set_key_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_key_event>(ML_forward(x)...); });

			set_maximize_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_maximize_event>(ML_forward(x)...);  });

			set_mouse_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_mouse_event>(ML_forward(x)...); });

			set_position_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_position_event>(ML_forward(x)...); });

			set_refresh_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_refresh_event>(ML_forward(x)...); });

			set_scroll_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_scroll_event>(ML_forward(x)...); });

			set_size_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_size_event>(ML_forward(x)...); });
		}

		// setup imgui
		{
			// set imgui allocators
			ImGui::SetAllocatorFunctions
			(
				[](size_t s, auto) noexcept { return memory::allocate(s); },
				[](void * p, auto) noexcept { return memory::deallocate(p); },
				nullptr
			);

			// create imgui context
			if (!(m_imgui = ImGui::CreateContext()))
			{
				return debug::error("failed creating ImGuiContext");
			}

			// imgui config flags
			auto & im_io{ ImGui::GetIO() };
			im_io.LogFilename = nullptr;
			im_io.IniFilename = nullptr;
			im_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			im_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			im_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			// imgui platform init
			if (!ML_ImGui_Init_Platform(get_handle(), true))
			{
				return debug::error("failed initializing ImGui platform");
			}

			// imgui renderer init
			if (!ML_ImGui_Init_Renderer())
			{
				return debug::error("failed initializing ImGui renderer");
			}
		}

		return true;
	}

	void application::close()
	{
		ML_ImGui_Shutdown();

		ImGui::DestroyContext(m_imgui);

		render_window::close();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool application::free_plugin(plugin_handle value)
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

	plugin_handle application::load_plugin(fs::path const & path)
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
				if (auto const optl{ lib.call<plugin *>(ML_PLUGIN_MAIN) }
				; optl.has_value())
				{
					m_plugins.emplace_back(code, path, std::move(lib), optl.value());

					return ML_handle(plugin_handle, code);
				}
			}
		}

		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}