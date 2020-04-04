#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Editor/StyleLoader.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Editor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// editor context
	class editor::context final : trackable, non_copyable
	{
		friend class		editor						;
		editor::config		m_config		{}			; // public startup variables
		editor::io			m_io			{}			; // public io variables
		void *				m_imgui			{}			; // active imgui context
	};

	static editor::context * g_editor{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool editor::is_initialized() noexcept
	{
		return g_editor;
	}

	bool editor::create_context(json const & j)
	{
		if (is_initialized() || !(g_editor = new editor::context{}))
		{
			return debug::log::error("editor is already initialized");
		}
		else
		{
			auto & cfg{ get_config() };

			j["imgui_style"].get_to(cfg.style_config);

			j["imgui_api_version"].get_to(cfg.api_version);

			cfg.ini_filename = j["imgui_enable_ini"].get<bool>() ? "imgui.ini" : nullptr;

			cfg.log_filename = j["imgui_enable_log"].get<bool>() ? "imgui.log" : nullptr;

			return g_editor;
		}
	}

	bool editor::destroy_context()
	{
		if (!g_editor) { return false; }
		else
		{
			delete g_editor;
			return !(g_editor = nullptr);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	editor::config & editor::get_config() noexcept
	{
		ML_assert(is_initialized());
		return (*g_editor).m_config;
	}

	editor::io & editor::get_io() noexcept
	{
		ML_assert(is_initialized());
		return (*g_editor).m_io;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool editor::startup()
	{
		if (!is_initialized()) { return false; }

		// set allocator functions
		ImGui::SetAllocatorFunctions
		(
			[](size_t s, auto) { return memory_manager::allocate(s); },
			[](void * p, auto) { return memory_manager::deallocate(p); },
			nullptr
		);

		// create context
		IMGUI_CHECKVERSION();
		g_editor->m_imgui = ImGui::CreateContext();

		auto & im_io{ ImGui::GetIO() };
		auto & im_style{ ImGui::GetStyle() };

		// paths
		im_io.LogFilename = g_editor->m_config.ini_filename;
		im_io.IniFilename = g_editor->m_config.log_filename;

		// config flags
		im_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		im_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		im_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// style
		style_loader::load_from_file(engine::path_to(g_editor->m_config.style_config));

		// backend
#ifdef ML_RENDERER_OPENGL
		if (!ImGui_ImplGlfw_InitForOpenGL((struct GLFWwindow *)engine::get_window().get_handle(), true))
		{
			return debug::log::error("Failed initializing ImGui platform");
		}

		if (!ImGui_ImplOpenGL3_Init(g_editor->m_config.api_version.c_str()))
		{
			return debug::log::error("Failed initializing ImGui renderer");
		}
#else
#endif
		return true;
	}

	bool editor::shutdown()
	{
		if (!is_initialized()) { return false; }

		g_editor->m_io.main_menus.clear();

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
		ML_assert(is_initialized());

#ifdef ML_RENDERER_OPENGL
		ImGui_ImplOpenGL3_NewFrame();
#else
#endif
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
	}

	void editor::render()
	{
		ML_assert(is_initialized());

		ML_ImGui_ScopeID(ML_addressof(g_editor));

		// DOCKSPACE
		if (auto & io{ g_editor->m_io }; io.show_dockspace)
		{
			ML_ImGui_ScopeID(dockspace_title);
			
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				// bounds
				ImGuiViewport const * viewport{ ImGui::GetMainViewport() };
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);

				// style
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, io.dock_rounding);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, io.dock_border);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, io.dock_padding);
				ImGui::SetNextWindowBgAlpha(io.dock_alpha);

				// begin
				if (ImGui::Begin(dockspace_title, &io.show_dockspace,
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoBringToFrontOnFocus |
					ImGuiWindowFlags_NoNavFocus |
					ImGuiWindowFlags_NoDocking |
					ImGuiWindowFlags_NoBackground |
					(io.show_main_menu ? ImGuiWindowFlags_MenuBar : 0)
				))
				{
					ImGui::PopStyleVar(3);

					if (io.dock_nodes.empty())
					{
						event_system::fire_event<gui_dock_event>();
					}

					ImGui::DockSpace(
						ImGui::GetID(dockspace_title),
						io.dock_size,
						ImGuiDockNodeFlags_PassthruCentralNode |
						ImGuiDockNodeFlags_AutoHideTabBar
					);

					ImGui::End();
				}
			}
		}

		// MAIN MENU
		if (auto & io{ g_editor->m_io }; io.show_main_menu)
		{
			if (ImGui::BeginMainMenuBar())
			{
				for (auto const & pair : io.main_menus)
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
		ML_assert(is_initialized());

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

	void editor::show_imgui_demo(bool * p_open)
	{
		ML_assert(is_initialized());
		ImGui::ShowDemoWindow(p_open);
	}

	void editor::show_imgui_metrics(bool * p_open)
	{
		ML_assert(is_initialized());
		ImGui::ShowMetricsWindow(p_open);
	}

	void editor::show_imgui_about(bool * p_open)
	{
		ML_assert(is_initialized());
		ImGui::ShowAboutWindow(p_open);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void editor::add_menu(cstring label, menu_t && fn)
	{
		ML_assert(is_initialized());
		
		auto & menus{ g_editor->m_io.main_menus };
		
		auto it{ std::find_if(menus.begin(), menus.end(), [&](auto & e) { return (e.first == label); }) };
		
		if (it == menus.end())
		{
			menus.push_back({ label, {} });
			it = (menus.end() - 1);
		}
		
		if (fn)
		{
			it->second.emplace_back(ML_forward(fn));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t editor::begin_dock_builder(int32_t flags)
	{
		ML_assert(is_initialized());
		
		if (uint32_t root{ ImGui::GetID(dockspace_title) }; !ImGui::DockBuilderGetNode(root))
		{
			ImGui::DockBuilderRemoveNode(root);

			ImGui::DockBuilderAddNode(root, flags);

			return root;
		}
		return NULL;
	}

	uint32_t editor::end_dock_builder(uint32_t root)
	{
		ML_assert(is_initialized());
		if (root)
		{
			ImGui::DockBuilderFinish(root);
		}
		return root;
	}

	uint32_t editor::dock(cstring name, uint32_t id)
	{
		ML_assert(is_initialized());
		if (name && id)
		{
			ImGui::DockBuilderDockWindow(name, id);
			return id;
		}
		return NULL;
	}

	uint32_t editor::split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * other)
	{
		ML_assert(is_initialized());
		return (*g_editor).m_io.dock_nodes[(size_t)i] = split(id, dir, ratio, other);
	}

	uint32_t editor::split(uint32_t id, int32_t dir, float_t ratio, uint32_t * other)
	{
		ML_assert(is_initialized());
		return split(id, dir, ratio, nullptr, other);
	}

	uint32_t editor::split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * other)
	{
		ML_assert(is_initialized());
		return ImGui::DockBuilderSplitNode(id, dir, ratio, out, other);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}