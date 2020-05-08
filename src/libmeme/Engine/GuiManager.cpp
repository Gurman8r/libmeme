#include <libmeme/Engine/GuiManager.hpp>
#include <libmeme/Engine/ImGui.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/FileUtility.hpp>
#include <libmeme/Core/Input.hpp>
#include <libmeme/Platform/Window.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	gui_manager::gui_manager(json const & j, allocator_type alloc) noexcept
		: m_gui_context	{}
		, m_main_menu	{ alloc }
		, m_dockspace	{ alloc }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool gui_manager::is_initialized() noexcept { return (m_gui_context != nullptr); }

	bool gui_manager::initialize(window const & win, cstring version)
	{
		if (is_initialized()) { return false; }

		// check imgui version
		IMGUI_CHECKVERSION();

		// set allocator functions
		ImGui::SetAllocatorFunctions
		(
			[](size_t s, auto) noexcept { return memory_manager::allocate(s); },
			[](void * p, auto) noexcept { return memory_manager::deallocate(p); },
			nullptr
		);

		// create editor_context
		m_gui_context = ImGui::CreateContext();

		auto & im_io{ ImGui::GetIO() };
		auto & im_style{ ImGui::GetStyle() };

		// imgui paths
		im_io.LogFilename = nullptr;
		im_io.IniFilename = nullptr;

		// config flags
		im_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		im_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		im_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// backend
#if defined(ML_RENDERER_OPENGL) && defined(ML_PLATFORM_GLFW)
		
		if (!ImGui_ImplGlfw_InitForOpenGL((struct GLFWwindow *)win.get_handle(), true))
		{
			return debug::error("Failed initializing ImGui platform");
		}

		if (!ImGui_ImplOpenGL3_Init(version))
		{
			return debug::error("Failed initializing ImGui renderer");
		}
#else
#endif
		return is_initialized();
	}

	bool gui_manager::finalize()
	{
		if (!is_initialized()) { return false; }

#if defined(ML_RENDERER_OPENGL)
		ImGui_ImplOpenGL3_Shutdown();
#else
#endif

#if defined(ML_PLATFORM_GLFW)
		ImGui_ImplGlfw_Shutdown();
#else
#endif

		ImGui::DestroyContext();

		m_gui_context = nullptr;

		return !is_initialized();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void gui_manager::begin_frame()
	{
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

	void gui_manager::draw_builtin()
	{
		ML_imgui_scope_id(ML_addressof(this));

		// DOCKSPACE
		if (auto & d{ m_dockspace }; d.visible)
		{
			ML_imgui_scope_id(ML_addressof(&d));
			
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				// viewport
				ImGuiViewport const * v{ ImGui::GetMainViewport() };
				ImGui::SetNextWindowPos(v->Pos);
				ImGui::SetNextWindowSize(v->Size);
				ImGui::SetNextWindowViewport(v->ID);

				// style
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, d.rounding);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, d.border);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, d.padding);
				ImGui::SetNextWindowBgAlpha(d.alpha);

				// begin
				if (ImGui::Begin(d.title, &d.visible,
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoBringToFrontOnFocus |
					ImGuiWindowFlags_NoNavFocus |
					ImGuiWindowFlags_NoDocking |
					ImGuiWindowFlags_NoBackground |
					(m_main_menu.visible ? ImGuiWindowFlags_MenuBar : 0)
				))
				{
					ImGui::PopStyleVar(3);

					// fire docking event if nodes are empty
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
		}

		// MAIN MENU
		if (auto & m{ m_main_menu }; m.visible)
		{
			ML_imgui_scope_id(ML_addressof(&m));

			if (ImGui::BeginMainMenuBar())
			{
				for (auto const & pair : m.menus)
				{
					if (!pair.second.empty() && ImGui::BeginMenu(pair.first))
					{
						for (auto const & fn : pair.second)
						{
							if (fn)
							{
								std::invoke(fn);
							}
						}
						ImGui::EndMenu();
					}
				}
				ImGui::EndMainMenuBar();
			}
		}
	}

	void gui_manager::end_frame()
	{
		ImGui::Render();

#if defined(ML_RENDERER_OPENGL)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#else
#endif
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			auto backup_context{ window::get_current_context() };
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			window::set_current_context(backup_context);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool gui_manager::load_style(fs::path const & path)
	{
		using namespace util;

		// empty
		if (path.empty()) return false;

		// builtins
		if (!fs::exists(path))
		{
			switch (hash(to_lower(path.string())))
			{
			default: return false;
			case hash("classic")	: ImGui::StyleColorsClassic();
			case hash("dark")		: ImGui::StyleColorsDark();
			case hash("light")	: ImGui::StyleColorsLight(); 
			}
			return true;
		}

		// open file
		std::ifstream file{ path };
		ML_defer{ file.close(); };
		if (!file) return false;

		// style ref
		auto & style{ ImGui::GetStyle() };

		// scan file
		pmr::string line;
		while (std::getline(file, line))
		{
			// trim line
			line = trim(line);
			
			// skip line
			if (line.empty() || line.front() == '#') { continue; }

			// scan line
			pmr::stringstream ss{}; ss << line;
			switch (hash(input<pmr::string>(ss)))
			{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			case hash("ImGuiStyle"): { switch (hash(input<pmr::string>(ss)))
			{
			case hash("Alpha")					: style.Alpha = input<float_t>(ss); break;
			case hash("WindowPadding")			: style.WindowPadding = input<vec2>(ss); break;
			case hash("WindowRounding")			: style.WindowRounding = input<float_t>(ss); break;
			case hash("WindowBorderSize")		: style.WindowBorderSize = input<float_t>(ss); break;
			case hash("WindowMinSize")			: style.WindowMinSize = input<vec2>(ss); break;
			case hash("WindowTitleAlign")		: style.WindowTitleAlign = input<vec2>(ss); break;
			case hash("ChildRounding")			: style.ChildRounding = input<float_t>(ss); break;
			case hash("ChildBorderSize")		: style.ChildBorderSize = input<float_t>(ss); break;
			case hash("PopupRounding")			: style.PopupRounding = input<float_t>(ss); break;
			case hash("PopupBorderSize")		: style.PopupBorderSize = input<float_t>(ss); break;
			case hash("FramePadding")			: style.FramePadding = input<vec2>(ss); break;
			case hash("FrameRounding")			: style.FrameRounding = input<float_t>(ss); break;
			case hash("FrameBorderSize")		: style.FrameBorderSize = input<float_t>(ss); break;
			case hash("ItemSpacing")			: style.ItemInnerSpacing = input<vec2>(ss); break;
			case hash("ItemInnerSpacing")		: style.ItemInnerSpacing = input<vec2>(ss); break;
			case hash("TouchExtraPadding")		: style.TouchExtraPadding = input<vec2>(ss); break;
			case hash("IndentSpacing")			: style.IndentSpacing = input<float_t>(ss); break;
			case hash("ColumnsMinSpacing")		: style.ColumnsMinSpacing = input<float_t>(ss); break;
			case hash("ScrollbarSize")			: style.ScrollbarSize = input<float_t>(ss); break;
			case hash("ScrollbarRounding")		: style.ScrollbarRounding = input<float_t>(ss); break;
			case hash("GrabMinSize")			: style.GrabMinSize = input<float_t>(ss); break;
			case hash("GrabRounding")			: style.GrabRounding = input<float_t>(ss); break;
			case hash("TabRounding")			: style.TabRounding = input<float_t>(ss); break;
			case hash("TabBorderSize")			: style.TabBorderSize = input<float_t>(ss); break;
			case hash("ButtonTextAlign")		: style.ButtonTextAlign = input<vec2>(ss); break;
			case hash("SelectableTextAlign")	: style.SelectableTextAlign = input<vec2>(ss); break;
			case hash("DisplayWindowPadding")	: style.DisplayWindowPadding = input<vec2>(ss); break;
			case hash("DisplaySafeAreaPadding")	: style.DisplaySafeAreaPadding = input<vec2>(ss); break;
			case hash("MouseCursorScale")		: style.MouseCursorScale = input<float_t>(ss); break;
			case hash("AntiAliasedLines")		: style.AntiAliasedLines = input<bool>(ss); break;
			case hash("AntiAliasedFill")		: style.AntiAliasedFill = input<bool>(ss); break;
			case hash("CurveTessellationTol")	: style.CurveTessellationTol = input<float_t>(ss); break;
			} } break;
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			case hash("ImGuiCol"): { switch (hash(input<pmr::string>(ss)))
			{
			case hash("Text")					: style.Colors[ImGuiCol_Text] = input<vec4>(ss); break;
			case hash("TextDisabled")			: style.Colors[ImGuiCol_TextDisabled] = input<vec4>(ss); break;
			case hash("WindowBg")				: style.Colors[ImGuiCol_WindowBg] = input<vec4>(ss); break;
			case hash("ChildBg")				: style.Colors[ImGuiCol_ChildBg] = input<vec4>(ss); break;
			case hash("PopupBg")				: style.Colors[ImGuiCol_PopupBg] = input<vec4>(ss); break;
			case hash("Border")					: style.Colors[ImGuiCol_Border] = input<vec4>(ss); break;
			case hash("BorderShadow")			: style.Colors[ImGuiCol_BorderShadow] = input<vec4>(ss); break;
			case hash("FrameBg")				: style.Colors[ImGuiCol_FrameBg] = input<vec4>(ss); break;
			case hash("FrameBgHovered")			: style.Colors[ImGuiCol_FrameBgHovered] = input<vec4>(ss); break;
			case hash("FrameBgActive")			: style.Colors[ImGuiCol_FrameBgActive] = input<vec4>(ss); break;
			case hash("TitleBg")				: style.Colors[ImGuiCol_TitleBg] = input<vec4>(ss); break;
			case hash("TitleBgActive")			: style.Colors[ImGuiCol_TitleBgActive] = input<vec4>(ss); break;
			case hash("TitleBgCollapsed")		: style.Colors[ImGuiCol_TitleBgCollapsed] = input<vec4>(ss); break;
			case hash("MenuBarBg")				: style.Colors[ImGuiCol_MenuBarBg] = input<vec4>(ss); break;
			case hash("ScrollbarBg")			: style.Colors[ImGuiCol_ScrollbarBg] = input<vec4>(ss); break;
			case hash("ScrollbarGrab")			: style.Colors[ImGuiCol_ScrollbarGrab] = input<vec4>(ss); break;
			case hash("ScrollbarGrabHovered")	: style.Colors[ImGuiCol_ScrollbarGrabHovered] = input<vec4>(ss); break;
			case hash("ScrollbarGrabActive")	: style.Colors[ImGuiCol_ScrollbarGrabActive] = input<vec4>(ss); break;
			case hash("CheckMark")				: style.Colors[ImGuiCol_CheckMark] = input<vec4>(ss); break;
			case hash("SliderGrab")				: style.Colors[ImGuiCol_SliderGrab] = input<vec4>(ss); break;
			case hash("SliderGrabActive")		: style.Colors[ImGuiCol_SliderGrabActive] = input<vec4>(ss); break;
			case hash("Button")					: style.Colors[ImGuiCol_Button] = input<vec4>(ss); break;
			case hash("ButtonHovered")			: style.Colors[ImGuiCol_ButtonHovered] = input<vec4>(ss); break;
			case hash("ButtonActive")			: style.Colors[ImGuiCol_ButtonActive] = input<vec4>(ss); break;
			case hash("Header")					: style.Colors[ImGuiCol_Header] = input<vec4>(ss); break;
			case hash("HeaderHovered")			: style.Colors[ImGuiCol_HeaderHovered] = input<vec4>(ss); break;
			case hash("HeaderActive")			: style.Colors[ImGuiCol_HeaderActive] = input<vec4>(ss); break;
			case hash("Separator")				: style.Colors[ImGuiCol_Separator] = input<vec4>(ss); break;
			case hash("SeparatorHovered")		: style.Colors[ImGuiCol_SeparatorHovered] = input<vec4>(ss); break;
			case hash("SeparatorActive")		: style.Colors[ImGuiCol_SeparatorActive] = input<vec4>(ss); break;
			case hash("ResizeGrip")				: style.Colors[ImGuiCol_ResizeGrip] = input<vec4>(ss); break;
			case hash("ResizeGripHovered")		: style.Colors[ImGuiCol_ResizeGripHovered] = input<vec4>(ss); break;
			case hash("ResizeGripActive")		: style.Colors[ImGuiCol_ResizeGripActive] = input<vec4>(ss); break;
			case hash("Tab")					: style.Colors[ImGuiCol_Tab] = input<vec4>(ss); break;
			case hash("TabHovered")				: style.Colors[ImGuiCol_TabHovered] = input<vec4>(ss); break;
			case hash("TabActive")				: style.Colors[ImGuiCol_TabActive] = input<vec4>(ss); break;
			case hash("TabUnfocused")			: style.Colors[ImGuiCol_TabUnfocused] = input<vec4>(ss); break;
			case hash("TabUnfocusedActive")		: style.Colors[ImGuiCol_TabUnfocusedActive] = input<vec4>(ss); break;
			case hash("DockingPreview")			: style.Colors[ImGuiCol_DockingPreview] = input<vec4>(ss); break;
			case hash("DockingEmptyBg")			: style.Colors[ImGuiCol_DockingEmptyBg] = input<vec4>(ss); break;
			case hash("PlotLines")				: style.Colors[ImGuiCol_PlotLines] = input<vec4>(ss); break;
			case hash("PlotLinesHovered")		: style.Colors[ImGuiCol_PlotLinesHovered] = input<vec4>(ss); break;
			case hash("PlotHistogram")			: style.Colors[ImGuiCol_PlotHistogram] = input<vec4>(ss); break;
			case hash("PlotHistogramHovered")	: style.Colors[ImGuiCol_PlotHistogramHovered] = input<vec4>(ss); break;
			case hash("TextSelectedBg")			: style.Colors[ImGuiCol_TextSelectedBg] = input<vec4>(ss); break;
			case hash("DragDropTarget")			: style.Colors[ImGuiCol_DragDropTarget] = input<vec4>(ss); break;
			case hash("NavHighlight")			: style.Colors[ImGuiCol_NavHighlight] = input<vec4>(ss); break;
			case hash("NavWindowingHighlight")	: style.Colors[ImGuiCol_NavWindowingHighlight] = input<vec4>(ss); break;
			case hash("NavWindowingDimBg")		: style.Colors[ImGuiCol_NavWindowingDimBg] = input<vec4>(ss); break;
			case hash("ModalWindowDimBg")		: style.Colors[ImGuiCol_ModalWindowDimBg] = input<vec4>(ss); break;
			} } break;
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			}
		}
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void gui_manager::show_imgui_demo(bool * p_open)
	{
		ImGui::ShowDemoWindow(p_open);
	}

	void gui_manager::show_imgui_metrics(bool * p_open)
	{
		ImGui::ShowMetricsWindow(p_open);
	}

	void gui_manager::show_imgui_about(bool * p_open)
	{
		ImGui::ShowAboutWindow(p_open);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t gui_manager::dockspace_data::begin_builder(int32_t flags)
	{
		if (uint32_t root{ ImGui::GetID(this->title) }; !ImGui::DockBuilderGetNode(root))
		{
			ImGui::DockBuilderRemoveNode(root);

			ImGui::DockBuilderAddNode(root, flags);

			return root;
		}
		return NULL;
	}

	uint32_t gui_manager::dockspace_data::end_builder(uint32_t root)
	{
		if (root)
		{
			ImGui::DockBuilderFinish(root);
		}
		return root;
	}

	uint32_t gui_manager::dockspace_data::dock(cstring name, uint32_t id)
	{
		if (name && id)
		{
			ImGui::DockBuilderDockWindow(name, id);
			return id;
		}
		return NULL;
	}

	uint32_t gui_manager::dockspace_data::split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * value)
	{
		return this->nodes[(size_t)i] = split(id, dir, ratio, value);
	}

	uint32_t gui_manager::dockspace_data::split(uint32_t id, int32_t dir, float_t ratio, uint32_t * value)
	{
		return split(id, dir, ratio, nullptr, value);
	}

	uint32_t gui_manager::dockspace_data::split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * value)
	{
		return ImGui::DockBuilderSplitNode(id, dir, ratio, out, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}