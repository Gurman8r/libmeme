#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Editor/StyleLoader.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Editor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// editor editor_context
	class editor::editor_context final : non_copyable, trackable
	{
		friend class		editor								;
		editor_config		m_config		{}					; // public startup variables
		editor_io			m_io			{}					; // public runtime variables
		ImGuiContext *		m_imgui			{}					; // imgui context
	};

	static editor::editor_context * g_editor{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool editor::is_initialized() noexcept { return g_editor; }

	bool editor::create_context(json const & j)
	{
		if (g_editor)
		{
			return debug::log::error("editor is already initialized");
		}
		else if (!(g_editor = new editor_context{}))
		{
			return debug::log::error("failed initializing editor editor_context");
		}
		else
		{
			auto & cfg{ config() };

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
		delete g_editor;
		return !(g_editor = nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool editor::startup()
	{
		ML_assert(g_editor);

		// check imgui version
		IMGUI_CHECKVERSION();

		// set allocator functions
		ImGui::SetAllocatorFunctions
		(
			[](size_t s, auto) { return memory_manager::allocate(s); },
			[](void * p, auto) { return memory_manager::deallocate(p); },
			nullptr
		);

		// create editor_context
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
#if defined(ML_RENDERER_OPENGL) && defined(ML_PLATFORM_GLFW)
		
		if (!ImGui_ImplGlfw_InitForOpenGL((struct GLFWwindow *)engine::window().get_handle(), true))
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
		ML_assert(g_editor);

		g_editor->m_io.main_menus.clear();

#if defined(ML_RENDERER_OPENGL)
		ImGui_ImplOpenGL3_Shutdown();
#else
#endif

#if defined(ML_PLATFORM_GLFW)
		ImGui_ImplGlfw_Shutdown();
#else
#endif

		ImGui::DestroyContext();

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void editor::new_frame()
	{
		ML_assert(g_editor);

#if defined(ML_RENDERER_OPENGL)
		ImGui_ImplOpenGL3_NewFrame();
#else
#endif

#if defined(ML_PLATFORM_GLFW)
		ImGui_ImplGlfw_NewFrame();
#else
#endif

		ImGui::NewFrame();
	}

	void editor::render()
	{
		ML_assert(g_editor);

		ML_ImGui_ScopeID(ML_addressof(g_editor));

		// DOCKSPACE
		if (auto & io{ g_editor->m_io }; io.show_dockspace)
		{
			ML_ImGui_ScopeID(dockspace_title);
			
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				// viewport
				ImGuiViewport const * v{ ImGui::GetMainViewport() };
				ImGui::SetNextWindowPos(v->Pos);
				ImGui::SetNextWindowSize(v->Size);
				ImGui::SetNextWindowViewport(v->ID);

				// style
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, io.dockspace_rounding);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, io.dockspace_border);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, io.dockspace_padding);
				ImGui::SetNextWindowBgAlpha(io.dockspace_alpha);

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

					if (io.dockspace_nodes.empty())
					{
						event_system::fire_event<gui_dock_event>();
					}

					ImGui::DockSpace(
						ImGui::GetID(dockspace_title),
						io.dockspace_size,
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
		ML_assert(g_editor);

		ImGui::Render();

#if defined(ML_RENDERER_OPENGL)
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
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	editor::editor_config & editor::config() noexcept
	{
		ML_assert(g_editor);
		return (*g_editor).m_config;
	}

	editor::editor_io & editor::io() noexcept
	{
		ML_assert(g_editor);
		return (*g_editor).m_io;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void editor::show_imgui_demo(bool * p_open)
	{
		ML_assert(g_editor);
		ImGui::ShowDemoWindow(p_open);
	}

	void editor::show_imgui_metrics(bool * p_open)
	{
		ML_assert(g_editor);
		ImGui::ShowMetricsWindow(p_open);
	}

	void editor::show_imgui_about(bool * p_open)
	{
		ML_assert(g_editor);
		ImGui::ShowAboutWindow(p_open);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void editor::add_menu(cstring label, menu_t && fn)
	{
		ML_assert(g_editor);
		
		auto & menus{ g_editor->m_io.main_menus };
		
		auto it{ std::find_if(menus.begin(), menus.end(), [&](auto & e)
		{
			return (e.first == label); })
		};
		
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

	uint32_t editor::begin_dockspace_builder(int32_t flags)
	{
		ML_assert(g_editor);
		
		if (uint32_t root{ ImGui::GetID(dockspace_title) }; !ImGui::DockBuilderGetNode(root))
		{
			ImGui::DockBuilderRemoveNode(root);

			ImGui::DockBuilderAddNode(root, flags);

			return root;
		}
		return NULL;
	}

	uint32_t editor::end_dockspace_builder(uint32_t root)
	{
		ML_assert(g_editor);
		if (root)
		{
			ImGui::DockBuilderFinish(root);
		}
		return root;
	}

	uint32_t editor::dock(cstring name, uint32_t id)
	{
		ML_assert(g_editor);
		if (name && id)
		{
			ImGui::DockBuilderDockWindow(name, id);
			return id;
		}
		return NULL;
	}

	uint32_t editor::split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * value)
	{
		ML_assert(g_editor);
		return (*g_editor).m_io.dockspace_nodes[(size_t)i] = split(id, dir, ratio, value);
	}

	uint32_t editor::split(uint32_t id, int32_t dir, float_t ratio, uint32_t * value)
	{
		ML_assert(g_editor);
		return split(id, dir, ratio, nullptr, value);
	}

	uint32_t editor::split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * value)
	{
		ML_assert(g_editor);
		return ImGui::DockBuilderSplitNode(id, dir, ratio, out, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}