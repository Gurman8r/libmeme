#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Editor/StyleLoader.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Editor/EditorEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static editor::context * g_editor{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool editor::running() noexcept
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
	
	bool editor::startup()
	{
		if (!g_editor) return false;

		// set allocator functions
		ImGui::SetAllocatorFunctions(
			[](size_t s, auto)
			{
				return memory_manager::allocate(s);
			},
			[](void * p, auto)
			{
				return memory_manager::deallocate(p);
			},
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
		if (!g_editor) return false;

		g_editor->m_io.main_menu.menus.clear();

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
		ML_assert(running());
		ML_ImGui_ScopeID(ML_addressof(g_editor));

		auto do_render = [&](auto & x, auto && fn)
		{
			if (!x.open) return;
			ML_ImGui_ScopeID(ML_addressof(&x));
			ML_ImGui_ScopeID(x.title);
			std::invoke(ML_forward(fn), x);
		};

		// RENDER DOCKSPACE
		do_render(g_editor->m_io.dockspace, [&](auto & d)
		{
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				// bounds
				ImGuiViewport const * viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);

				// style
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, d.rounding);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, d.border);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, d.padding);
				ImGui::SetNextWindowBgAlpha(d.alpha);

				// begin
				if (ImGui::Begin(d.title, &d.open,
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoBringToFrontOnFocus |
					ImGuiWindowFlags_NoNavFocus |
					ImGuiWindowFlags_NoDocking |
					ImGuiWindowFlags_NoBackground |
					(g_editor->m_io.main_menu.open ? ImGuiWindowFlags_MenuBar : 0)
				))
				{
					ImGui::PopStyleVar(3);

					if (d.nodes.empty())
					{
						event_system::fire_event<gui_dock_event>();
					}

					ImGui::DockSpace(
						ImGui::GetID(d.title),
						d.size,
						ImGuiDockNodeFlags_PassthruCentralNode |
						ImGuiDockNodeFlags_AutoHideTabBar
					);

					ImGui::End();
				}
			}
		});

		// RENDER MAIN MENU BAR
		do_render(g_editor->m_io.main_menu, [&](auto & m)
		{
			if (ImGui::BeginMainMenuBar())
			{
				for (auto const & pair : m.menus)
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
		});
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

	void editor::runtime::main_menu_t::add_menu(cstring label, std::function<void()> && fn)
	{
		auto it{ std::find_if(menus.begin(), menus.end(), [&](auto elem)
		{
			return (elem.first == label);
		}) };
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

	uint32_t editor::runtime::dockspace_t::begin_builder(int32_t flags)
	{
		if (uint32_t root{ ImGui::GetID(this->title) })
		{
			if (!ImGui::DockBuilderGetNode(root))
			{
				ImGui::DockBuilderRemoveNode(root);

				ImGui::DockBuilderAddNode(root, flags);

				return root;
			}
		}
		return NULL;
	}

	uint32_t editor::runtime::dockspace_t::end_builder(uint32_t root)
	{
		if (root)
		{
			ImGui::DockBuilderFinish(root);
		}
		return root;
	}

	uint32_t editor::runtime::dockspace_t::dock(cstring name, uint32_t id)
	{
		if (name && id)
		{
			ImGui::DockBuilderDockWindow(name, id);
			return id;
		}
		return NULL;
	}

	uint32_t editor::runtime::dockspace_t::split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * other)
	{
		return this->nodes[(size_t)i] = split(id, dir, ratio, other);
	}

	uint32_t editor::runtime::dockspace_t::split(uint32_t id, int32_t dir, float_t ratio, uint32_t * other)
	{
		return split(id, dir, ratio, nullptr, other);
	}

	uint32_t editor::runtime::dockspace_t::split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * other)
	{
		return ImGui::DockBuilderSplitNode(id, dir, ratio, out, other);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void editor::show_imgui_demo(bool * p_open)
	{
		ImGui::ShowDemoWindow(p_open);
	}

	void editor::show_imgui_metrics(bool * p_open)
	{
		ImGui::ShowMetricsWindow(p_open);
	}

	void editor::show_imgui_about(bool * p_open)
	{
		ImGui::ShowAboutWindow(p_open);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}