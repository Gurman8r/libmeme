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

	editor::context const * editor::create_context()
	{
		if (g_editor) return nullptr;

		return (g_editor = new editor::context{});
	}
	
	bool editor::initialized() noexcept
	{
		return g_editor;
	}
	
	bool editor::startup(bool install_callbacks)
	{
		if (!g_editor) return false;

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
		if (!g_editor) return;

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
#ifdef ML_RENDERER_OPENGL
		ImGui_ImplOpenGL3_NewFrame();
#endif
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void editor::render_frame()
	{
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
		ImGui::ShowAboutWindow(p_open);
	}

	void editor::show_imgui_demo(bool * p_open)
	{
		ImGui::ShowDemoWindow(p_open);
	}

	void editor::show_user_guide()
	{
		ImGui::ShowUserGuide();
	}

	void editor::show_style_editor(void * ref)
	{
		ImGui::ShowStyleEditor(static_cast<ImGuiStyle *>(ref));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	editor::context const * editor::get_context() noexcept
	{
		return g_editor;
	}

	editor::config & editor::get_config() noexcept
	{
		return g_editor->config;
	}

	editor::io & editor::get_io() noexcept
	{
		return g_editor->io;
	}

	editor_dockspace & editor::get_dockspace() noexcept
	{
		return g_editor->dockspace;
	}

	editor_main_menu & editor::get_main_menu() noexcept
	{
		return g_editor->main_menu;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void editor::draw_texture_preview(texture const & value, vec2 const & maxSize)
	{
		auto & io{ ImGui::GetIO() };

		void * const tex_id{ value.address() };

		vec2 const tex_sz{ util::scale_to_fit((vec2)value.size(),
		{
			maxSize[0] == 0.0f ? ImGui::GetContentRegionAvail()[0] : maxSize[0],
			maxSize[1] == 0.0f ? ImGui::GetContentRegionAvail()[1] : maxSize[1]
		}) };

		float_t const tex_w{ tex_sz[0] };
		float_t const tex_h{ tex_sz[1] };

		ImGui::Text("%u: %ux%u (%.0fx%.0f)",
			value.handle(), value.width(), value.height(), tex_w, tex_h
		);

		auto const pos{ ImGui::GetCursorScreenPos() };

		ImGui::Image(tex_id,
			{ tex_w, tex_h },
			{ 0, 0 },
			{ 1, 1 },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 0.5f }
		);

		if (ImGui::IsItemHovered())
		{
			float_t const region_zoom{ 4.0f };
			float_t const region_size{ 32.0f };

			ImGui::BeginTooltip();

			float_t region_x{ io.MousePos.x - pos.x - region_size * 0.5f };
			if (region_x < 0.0f) region_x = 0.0f;
			else if (region_x > (tex_w - region_size)) region_x = (tex_w - region_size);

			float_t region_y{ io.MousePos.y - pos.y - region_size * 0.5f };
			if (region_y < 0.0f) region_y = 0.0f;
			else if (region_y > (tex_h - region_size)) region_y = (tex_h - region_size);

			ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
			ImGui::Text("Max: (%.2f, %.2f)", region_x + region_size, region_y + region_size);
			ImGui::Image(tex_id,
				{ region_size * region_zoom, region_size * region_zoom },
				{ region_x / tex_w, region_y / tex_h },
				{ (region_x + region_size) / tex_w, (region_y + region_size) / tex_h },
				{ 1.0f, 1.0f, 1.0f, 1.0f },
				{ 1.0f, 1.0f, 1.0f, 0.5f }
			);

			ImGui::EndTooltip();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}