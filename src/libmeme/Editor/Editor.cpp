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
		return !g_editor && (g_editor = new editor::context{});
	}

	bool editor::initialized() noexcept
	{
		return g_editor;
	}

	editor::context * const editor::get_context() noexcept
	{
		return g_editor;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool editor::startup(bool install_callbacks)
	{
		ML_ASSERT(initialized());

		// set allocator functions
		ImGui::SetAllocatorFunctions(
			[](size_t const size, auto) { return memory_manager::allocate(size); },
			[](void * const addr, auto) { return memory_manager::deallocate(addr); },
			nullptr
		);

		// create context
		IMGUI_CHECKVERSION();
		get_io().imgui_context = ImGui::CreateContext();

		auto & im_io{ ImGui::GetIO() };
		auto & im_style{ ImGui::GetStyle() };

		// config flags
		im_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		im_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		im_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		
		// viewports
		if (im_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			im_style.WindowRounding = 0.0f;
			im_style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// paths
		im_io.LogFilename = get_config().ini_file;
		im_io.IniFilename = get_config().log_file;

		// style
		switch (util::hash(util::to_lower(get_config().style)))
		{
		case util::hash("light"): ImGui::StyleColorsLight(); break;
		case util::hash("dark"): ImGui::StyleColorsDark(); break;
		case util::hash("classic"): ImGui::StyleColorsClassic(); break;
		default:
			if (fs::path const path{ get_config().style }; fs::exists(path))
			{
				style_loader{}(path);
			}
			break;
		}

		// backend
#ifdef ML_RENDERER_OPENGL

		if (!ImGui_ImplGlfw_InitForOpenGL(
			(struct GLFWwindow *)get_config().window_handle,
			install_callbacks
		))
		{
			return debug::log_error("Failed initializing ImGui platform");
		}

		if (!ImGui_ImplOpenGL3_Init(get_config().api_version.c_str()))
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

		get_main_menu().menus().clear();

#ifdef ML_RENDERER_OPENGL
		ImGui_ImplOpenGL3_Shutdown();
#else
#endif
		ImGui_ImplGlfw_Shutdown();

		ImGui::DestroyContext();

		delete g_editor;
		g_editor = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void editor::new_frame()
	{
		ML_ASSERT(initialized());

#ifdef ML_RENDERER_OPENGL
		ImGui_ImplOpenGL3_NewFrame();
#else
#endif
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void editor::render()
	{
		get_dockspace().render();

		get_main_menu().render();
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
}