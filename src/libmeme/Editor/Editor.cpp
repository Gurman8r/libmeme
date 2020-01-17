#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Platform/Window.hpp>
#include <libmeme/Renderer/GL.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/examples/imgui_impl_glfw.h>

#ifdef ML_IMPL_RENDERER_OPENGL
#	include <imgui/examples/imgui_impl_opengl3.h>
#else
#endif

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Dockspace Editor::s_dockspace{};
	
	MainMenuBar Editor::s_mainMenuBar{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Editor::startup(EditorSettings const & es)
	{
		// Create ImGui Context
		/* * * * * * * * * * * * * * * * * * * * */
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
		io.LogFilename = nullptr;
		io.IniFilename = nullptr;

		// Style
		static const std::string imgui_style{ "Dark" };
		switch (util::hash(util::to_lower(imgui_style)))
		{
		case util::hash("light"): ImGui::StyleColorsLight(); break;
		case util::hash("dark"): ImGui::StyleColorsDark(); break;
		case util::hash("classic"): ImGui::StyleColorsClassic(); break;
		}

		// Init Platform/Renderer
#ifdef ML_IMPL_RENDERER_OPENGL
		if (!ImGui_ImplGlfw_InitForOpenGL((struct GLFWwindow *)es.window_handle, es.install_callbacks))
		{
			return debug::log_error("Failed initializing ImGui platform");
		}

		if (!ImGui_ImplOpenGL3_Init("#version 130"))
		{
			return debug::log_error("Failed initializing ImGui renderer");
		}
#else
#endif
		return true;
	}

	void Editor::new_frame()
	{
#ifdef ML_IMPL_RENDERER_OPENGL
		ImGui_ImplOpenGL3_NewFrame();
#else
#endif
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Editor::render_frame()
	{
		ImGui::Render();

#ifdef ML_IMPL_RENDERER_OPENGL
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#else
#endif
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			auto backup_context{ Window::get_context_current() };
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			Window::make_context_current(backup_context);
		}

		GL::flush();
	}

	void Editor::shutdown()
	{
		s_dockspace.dispose();
		s_mainMenuBar.dispose();

#ifdef ML_IMPL_RENDERER_OPENGL
		ImGui_ImplOpenGL3_Shutdown();
#else
#endif
		ImGui_ImplGlfw_Shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor::show_about_window(bool * p_open)
	{
		ImGui::ShowAboutWindow(p_open);
	}

	void Editor::show_imgui_demo(bool * p_open)
	{
		ImGui::ShowDemoWindow(p_open);
	}

	void Editor::show_user_guide()
	{
		ImGui::ShowUserGuide();
	}

	void Editor::show_style_editor(void * ref)
	{
		ImGui::ShowStyleEditor(static_cast<ImGuiStyle *>(ref));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}