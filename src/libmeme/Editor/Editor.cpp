#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Platform/Window.hpp>
#include <libmeme/Renderer/GL.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#ifdef ML_IMPL_PLATFORM_GLFW
#	include <imgui/examples/imgui_impl_glfw.h>
#else
#endif

#ifdef ML_IMPL_RENDERER_OPENGL
#	include <imgui/examples/imgui_impl_opengl3.h>
#else
#endif

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor::Editor()
		: m_dockspace{}
		, m_mainMenuBar{}
	{
	}

	Editor::~Editor()
	{
		m_mainMenuBar.m_menus.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Editor::startup(void * window)
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
		switch (Hash(util::to_lower(imgui_style)))
		{
		case Hash("light"): ImGui::StyleColorsLight(); break;
		case Hash("dark"): ImGui::StyleColorsDark(); break;
		case Hash("classic"): ImGui::StyleColorsClassic(); break;
		}
		
		// Startup
#ifdef ML_IMPL_PLATFORM_GLFW
#	ifdef ML_IMPL_RENDERER_OPENGL
		if (!ImGui_ImplGlfw_InitForOpenGL(static_cast<struct GLFWwindow *>(window), true))
		{
			return Debug::logError("Failed initializing ImGui Platform");
		}

		if (!ImGui_ImplOpenGL3_Init("#version 130"))
		{
			return Debug::logError("Failed initializing ImGui Renderer");
		}
#	else
		// 
#	endif
#else
		// 
#endif
		return true;
	}

	void Editor::new_frame()
	{
#ifdef ML_IMPL_RENDERER_OPENGL
		ImGui_ImplOpenGL3_NewFrame();
#else
		// 
#endif

#ifdef ML_IMPL_PLATFORM_GLFW
		ImGui_ImplGlfw_NewFrame();
#else
		// 
#endif
		ImGui::NewFrame();
	}

	void Editor::render_frame()
	{
		ImGui::Render();

#ifdef ML_IMPL_RENDERER_OPENGL
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#else
		// 
#endif
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			auto backup_context{ Window::getContextCurrent() };
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			Window::makeContextCurrent(backup_context);
		}

		GL::flush();
	}

	void Editor::shutdown()
	{
		getInstance().~Editor();

#ifdef ML_IMPL_RENDERER_OPENGL
		ImGui_ImplOpenGL3_Shutdown();
#else
		// 
#endif

#ifdef ML_IMPL_PLATFORM_GLFW
		ImGui_ImplGlfw_Shutdown();
#else
		// 
#endif
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