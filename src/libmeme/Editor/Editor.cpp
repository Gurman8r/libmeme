#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Platform/Window.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Editor/StyleLoader.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/examples/imgui_impl_glfw.h>

#ifdef ML_RENDERER_OPENGL
#	include <imgui/examples/imgui_impl_opengl3.h>
#else
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static editor::context * g_editor{ nullptr };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool editor::create_context()
	{
		return !initialized() && (g_editor = new editor::context{});
	}
	
	bool editor::startup(bool install_callbacks)
	{
		ML_ASSERT(initialized());

		// Create ImGui Context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		auto & io{ ImGui::GetIO() };
		auto & style{ ImGui::GetStyle() };

		// Config Flags
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		
		// Viewports
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Paths
		io.LogFilename = g_editor->config.ini_file;
		io.IniFilename = g_editor->config.log_file;

		// Style
		switch (util::hash(util::to_lower(g_editor->config.style)))
		{
		case util::hash("light"): ImGui::StyleColorsLight(); break;
		case util::hash("dark"): ImGui::StyleColorsDark(); break;
		case util::hash("classic"): ImGui::StyleColorsClassic(); break;
		default:
			if (fs::path const path{ g_editor->config.style }; fs::exists(path))
			{
				style_loader{}(path);
			}
			break;
		}

		// Initialize Backend
#ifdef ML_RENDERER_OPENGL

		if (!ImGui_ImplGlfw_InitForOpenGL(
			(struct GLFWwindow *)g_editor->config.window_handle,
			install_callbacks
		))
		{
			return debug::log_error("Failed initializing ImGui platform");
		}

		if (!ImGui_ImplOpenGL3_Init(g_editor->config.api_version.c_str()))
		{
			return debug::log_error("Failed initializing ImGui renderer");
		}
#else
#endif
		return true;
	}

	void editor::shutdown()
	{
		ML_ASSERT(initialized());

		get_main_menu().clear();

#ifdef ML_RENDERER_OPENGL
		ImGui_ImplOpenGL3_Shutdown();
#endif
		ImGui_ImplGlfw_Shutdown();

		delete g_editor;
		g_editor = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void editor::new_frame()
	{
		ML_ASSERT(initialized());

#ifdef ML_RENDERER_OPENGL
		ImGui_ImplOpenGL3_NewFrame();
#endif
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void editor::render_frame()
	{
		ML_ASSERT(initialized());

		ImGui::Render();

#ifdef ML_RENDERER_OPENGL
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#else
#endif
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			auto backup_context{ window::get_context_current() };
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			window::make_context_current(backup_context);
		}

		GL::flush();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void editor::show_about_window(bool * p_open)
	{
		ML_ASSERT(initialized());
		ImGui::ShowAboutWindow(p_open);
	}

	void editor::show_imgui_demo(bool * p_open)
	{
		ML_ASSERT(initialized());
		ImGui::ShowDemoWindow(p_open);
	}

	void editor::show_user_guide()
	{
		ML_ASSERT(initialized());
		ImGui::ShowUserGuide();
	}

	void editor::show_style_editor(void * ref)
	{
		ML_ASSERT(initialized());
		ImGui::ShowStyleEditor(static_cast<ImGuiStyle *>(ref));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool editor::initialized() noexcept
	{
		return g_editor;
	}

	editor::context const * const editor::get_context() noexcept
	{
		ML_ASSERT(initialized());
		return g_editor;
	}

	editor::config & editor::get_config() noexcept
	{
		ML_ASSERT(initialized());
		return g_editor->config;
	}

	editor::io & editor::get_io() noexcept
	{
		ML_ASSERT(initialized());
		return g_editor->io;
	}

	editor_dockspace & editor::get_dockspace() noexcept
	{
		ML_ASSERT(initialized());
		return g_editor->io.dockspace;
	}

	editor_main_menu & editor::get_main_menu() noexcept
	{
		ML_ASSERT(initialized());
		return g_editor->io.main_menu;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}