#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Editor/StyleLoader.hpp>
#include <libmeme/Editor/ImGuiExt.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static editor::context * g_editor{};

	bool editor::initialized() noexcept
	{
		return g_editor;
	}

	bool editor::create_context()
	{
		return !g_editor && (g_editor = new editor::context{});
	}

	bool editor::destroy_context()
	{
		if (!g_editor) return false;
		delete g_editor;
		return !(g_editor = nullptr);
	}

	editor::context * const editor::get_context() noexcept
	{
		return g_editor;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool editor::startup(bool install_callbacks)
	{
		if (!initialized()) return false;

		// set allocator functions
		ImGui::SetAllocatorFunctions(
			[](auto size, auto) { return memory_manager::allocate(size); },
			[](auto addr, auto) { return memory_manager::deallocate(addr); },
			nullptr
		);

		// create context
		IMGUI_CHECKVERSION();
		g_editor->m_imgui_context = ImGui::CreateContext();

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
		im_io.LogFilename = g_editor->m_config.ini_file;
		im_io.IniFilename = g_editor->m_config.log_file;

		// style
		switch (util::hash(util::to_lower(g_editor->m_config.style)))
		{
		case util::hash("light"): ImGui::StyleColorsLight(); break;
		case util::hash("dark"): ImGui::StyleColorsDark(); break;
		case util::hash("classic"): ImGui::StyleColorsClassic(); break;
		default:
			if (fs::path const path{ g_editor->m_config.style }; fs::exists(path))
			{
				style_loader{}(path);
			}
			break;
		}

		// backend
#ifdef ML_RENDERER_OPENGL

		if (!ImGui_ImplGlfw_InitForOpenGL(
			(struct GLFWwindow *)engine::get_window().get_handle(),
			install_callbacks
		))
		{
			return debug::log_error("Failed initializing ImGui platform");
		}

		if (!ImGui_ImplOpenGL3_Init(g_editor->m_config.api_version.c_str()))
		{
			return debug::log_error("Failed initializing ImGui renderer");
		}
#else
#endif
		return true;
	}

	bool editor::shutdown()
	{
		if (!initialized()) return false;

		g_editor->m_main_menu.clear();

#ifdef ML_RENDERER_OPENGL
		ImGui_ImplOpenGL3_Shutdown();
#else
#endif
		ImGui_ImplGlfw_Shutdown();

		ImGui::DestroyContext();

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void editor::new_frame()
	{
#ifdef ML_RENDERER_OPENGL
		ImGui_ImplOpenGL3_NewFrame();
#else
#endif
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void editor::render()
	{
		// dockspace
		g_editor->m_dockspace.render();

		// main menu
		if (g_editor->m_io.show_main_menu)
		{
			if (ImGui::BeginMainMenuBar())
			{
				for (auto & pair : g_editor->m_main_menu)
				{
					if (!pair.second.empty() && ImGui::BeginMenu(pair.first))
					{
						for (auto const & fn : pair.second)
						{
							std::invoke(fn);
						}
						ImGui::EndMenu();
					}
				}

				ImGui::EndMainMenuBar();
			}
		}
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

	void editor::add_menu(cstring label, menu_t && value)
	{
		auto & m{ get_context()->m_main_menu };

		auto it{ std::find_if(m.begin(), m.end(), [&](auto elem)
		{
			return (elem.first == label);
		}) };
		if (it == m.end())
		{
			m.push_back({ label, {} });
			it = (m.end() - 1);
		}
		if (value)
		{
			it->second.emplace_back(ML_FWD(value));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void editor::show_imgui_demo(bool * p_open)
	{
		ImGui::ShowDemoWindow(p_open);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}