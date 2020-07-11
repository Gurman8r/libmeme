#include <libmeme/Engine/GuiManager.hpp>
#include <libmeme/Engine/ImGui.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/System/EventBus.hpp>
#include <libmeme/Core/FileUtility.hpp>
#include <libmeme/Core/ParserUtil.hpp>
#include <libmeme/Window/Window.hpp>

// GLFW / OpenGL3
#if defined(ML_IMPL_WINDOW_GLFW) && defined(ML_IMPL_RENDERER_OPENGL)
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#define ML_ImGui_Init_Platform(wh, ic)	ImGui_ImplGlfw_InitForOpenGL((struct GLFWwindow *)wh, ic)
#define ML_ImGui_Init_Renderer()		ImGui_ImplOpenGL3_Init("#version 130")
#define ML_ImGui_Shutdown()				ML_scope(&){ ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplGlfw_Shutdown(); }
#define ML_ImGui_NewFrame()				ML_scope(&){ ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame(); }
#define ML_ImGui_RenderDrawData(x)		ImGui_ImplOpenGL3_RenderDrawData(x)

#else
#	error "imgui is unavailable"
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	gui_manager::gui_manager(json const & j, allocator_type alloc) noexcept
		: m_gui_context	{}
		, m_main_menu	{ alloc }
		, m_dockspace	{ alloc }
	{
		IMGUI_CHECKVERSION();

		ML_assert(!ImGui::GetCurrentContext());
	}

	gui_manager::~gui_manager() noexcept
	{
		ML_assert(finalize());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool gui_manager::initialize(window const & wnd)
	{
		if (ImGui::GetCurrentContext()) { return false; }

		// allocators
		ImGui::SetAllocatorFunctions
		(
			[](size_t s, auto) noexcept { return memory::allocate(s); },
			[](void * p, auto) noexcept { return memory::deallocate(p); },
			nullptr
		);

		// create context
		if (!(m_gui_context = ImGui::CreateContext()))
		{
			return debug::error("failed creating ImGuiContext");
		}

		// config
		auto & im_io{ ImGui::GetIO() };
		im_io.LogFilename = nullptr;
		im_io.IniFilename = nullptr;
		im_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		im_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		im_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// init platform
		if (!ML_ImGui_Init_Platform(wnd.get_handle(), true))
		{
			return debug::error("failed initializing ImGui platform");
		}

		// init renderer
		if (!ML_ImGui_Init_Renderer())
		{
			return debug::error("failed initializing ImGui renderer");
		}

		return ImGui::GetCurrentContext();
	}

	bool gui_manager::finalize()
	{
		if (!ImGui::GetCurrentContext()) { return false; }

		ML_ImGui_Shutdown();
		
		ImGui::DestroyContext();
		
		return !(m_gui_context = nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void gui_manager::begin_frame()
	{
		ML_ImGui_NewFrame();

		ImGui::NewFrame();
	}

	void gui_manager::draw_default()
	{
		ML_ImGui_ScopeID(this);

		// DOCKSPACE
		if (auto & d{ m_dockspace }; d.visible)
		{
			ML_ImGui_ScopeID(&d);
			
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
						event_bus::fire_event<dock_gui_event>();
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

		// MAIN MENU BAR
		if (auto & m{ m_main_menu }; m.visible)
		{
			ML_ImGui_ScopeID(&m);

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

		ML_ImGui_RenderDrawData(ImGui::GetDrawData());

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
			default				: return false;
			case hash("classic"): ImGui::StyleColorsClassic();
			case hash("dark")	: ImGui::StyleColorsDark();
			case hash("light")	: ImGui::StyleColorsLight(); 
			}
			return true;
		}

		// open file
		std::ifstream file{ path };
		ML_defer(&){ file.close(); };
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
			switch (hash(parse<pmr::string>(ss)))
			{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			case hash("ImGuiStyle"): { switch (hash(parse<pmr::string>(ss)))
			{
			case hash("Alpha")					: style.Alpha = parse<float_t>(ss); break;
			case hash("WindowPadding")			: style.WindowPadding = parse<vec2>(ss); break;
			case hash("WindowRounding")			: style.WindowRounding = parse<float_t>(ss); break;
			case hash("WindowBorderSize")		: style.WindowBorderSize = parse<float_t>(ss); break;
			case hash("WindowMinSize")			: style.WindowMinSize = parse<vec2>(ss); break;
			case hash("WindowTitleAlign")		: style.WindowTitleAlign = parse<vec2>(ss); break;
			case hash("ChildRounding")			: style.ChildRounding = parse<float_t>(ss); break;
			case hash("ChildBorderSize")		: style.ChildBorderSize = parse<float_t>(ss); break;
			case hash("PopupRounding")			: style.PopupRounding = parse<float_t>(ss); break;
			case hash("PopupBorderSize")		: style.PopupBorderSize = parse<float_t>(ss); break;
			case hash("FramePadding")			: style.FramePadding = parse<vec2>(ss); break;
			case hash("FrameRounding")			: style.FrameRounding = parse<float_t>(ss); break;
			case hash("FrameBorderSize")		: style.FrameBorderSize = parse<float_t>(ss); break;
			case hash("ItemSpacing")			: style.ItemInnerSpacing = parse<vec2>(ss); break;
			case hash("ItemInnerSpacing")		: style.ItemInnerSpacing = parse<vec2>(ss); break;
			case hash("TouchExtraPadding")		: style.TouchExtraPadding = parse<vec2>(ss); break;
			case hash("IndentSpacing")			: style.IndentSpacing = parse<float_t>(ss); break;
			case hash("ColumnsMinSpacing")		: style.ColumnsMinSpacing = parse<float_t>(ss); break;
			case hash("ScrollbarSize")			: style.ScrollbarSize = parse<float_t>(ss); break;
			case hash("ScrollbarRounding")		: style.ScrollbarRounding = parse<float_t>(ss); break;
			case hash("GrabMinSize")			: style.GrabMinSize = parse<float_t>(ss); break;
			case hash("GrabRounding")			: style.GrabRounding = parse<float_t>(ss); break;
			case hash("TabRounding")			: style.TabRounding = parse<float_t>(ss); break;
			case hash("TabBorderSize")			: style.TabBorderSize = parse<float_t>(ss); break;
			case hash("ButtonTextAlign")		: style.ButtonTextAlign = parse<vec2>(ss); break;
			case hash("SelectableTextAlign")	: style.SelectableTextAlign = parse<vec2>(ss); break;
			case hash("DisplayWindowPadding")	: style.DisplayWindowPadding = parse<vec2>(ss); break;
			case hash("DisplaySafeAreaPadding")	: style.DisplaySafeAreaPadding = parse<vec2>(ss); break;
			case hash("MouseCursorScale")		: style.MouseCursorScale = parse<float_t>(ss); break;
			case hash("AntiAliasedLines")		: style.AntiAliasedLines = parse<bool>(ss); break;
			case hash("AntiAliasedFill")		: style.AntiAliasedFill = parse<bool>(ss); break;
			case hash("CurveTessellationTol")	: style.CurveTessellationTol = parse<float_t>(ss); break;
			} } break;
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			case hash("ImGuiCol"): { switch (hash(parse<pmr::string>(ss)))
			{
			case hash("Text")					: style.Colors[ImGuiCol_Text] = parse<vec4>(ss); break;
			case hash("TextDisabled")			: style.Colors[ImGuiCol_TextDisabled] = parse<vec4>(ss); break;
			case hash("WindowBg")				: style.Colors[ImGuiCol_WindowBg] = parse<vec4>(ss); break;
			case hash("ChildBg")				: style.Colors[ImGuiCol_ChildBg] = parse<vec4>(ss); break;
			case hash("PopupBg")				: style.Colors[ImGuiCol_PopupBg] = parse<vec4>(ss); break;
			case hash("Border")					: style.Colors[ImGuiCol_Border] = parse<vec4>(ss); break;
			case hash("BorderShadow")			: style.Colors[ImGuiCol_BorderShadow] = parse<vec4>(ss); break;
			case hash("FrameBg")				: style.Colors[ImGuiCol_FrameBg] = parse<vec4>(ss); break;
			case hash("FrameBgHovered")			: style.Colors[ImGuiCol_FrameBgHovered] = parse<vec4>(ss); break;
			case hash("FrameBgActive")			: style.Colors[ImGuiCol_FrameBgActive] = parse<vec4>(ss); break;
			case hash("TitleBg")				: style.Colors[ImGuiCol_TitleBg] = parse<vec4>(ss); break;
			case hash("TitleBgActive")			: style.Colors[ImGuiCol_TitleBgActive] = parse<vec4>(ss); break;
			case hash("TitleBgCollapsed")		: style.Colors[ImGuiCol_TitleBgCollapsed] = parse<vec4>(ss); break;
			case hash("MenuBarBg")				: style.Colors[ImGuiCol_MenuBarBg] = parse<vec4>(ss); break;
			case hash("ScrollbarBg")			: style.Colors[ImGuiCol_ScrollbarBg] = parse<vec4>(ss); break;
			case hash("ScrollbarGrab")			: style.Colors[ImGuiCol_ScrollbarGrab] = parse<vec4>(ss); break;
			case hash("ScrollbarGrabHovered")	: style.Colors[ImGuiCol_ScrollbarGrabHovered] = parse<vec4>(ss); break;
			case hash("ScrollbarGrabActive")	: style.Colors[ImGuiCol_ScrollbarGrabActive] = parse<vec4>(ss); break;
			case hash("CheckMark")				: style.Colors[ImGuiCol_CheckMark] = parse<vec4>(ss); break;
			case hash("SliderGrab")				: style.Colors[ImGuiCol_SliderGrab] = parse<vec4>(ss); break;
			case hash("SliderGrabActive")		: style.Colors[ImGuiCol_SliderGrabActive] = parse<vec4>(ss); break;
			case hash("Button")					: style.Colors[ImGuiCol_Button] = parse<vec4>(ss); break;
			case hash("ButtonHovered")			: style.Colors[ImGuiCol_ButtonHovered] = parse<vec4>(ss); break;
			case hash("ButtonActive")			: style.Colors[ImGuiCol_ButtonActive] = parse<vec4>(ss); break;
			case hash("Header")					: style.Colors[ImGuiCol_Header] = parse<vec4>(ss); break;
			case hash("HeaderHovered")			: style.Colors[ImGuiCol_HeaderHovered] = parse<vec4>(ss); break;
			case hash("HeaderActive")			: style.Colors[ImGuiCol_HeaderActive] = parse<vec4>(ss); break;
			case hash("Separator")				: style.Colors[ImGuiCol_Separator] = parse<vec4>(ss); break;
			case hash("SeparatorHovered")		: style.Colors[ImGuiCol_SeparatorHovered] = parse<vec4>(ss); break;
			case hash("SeparatorActive")		: style.Colors[ImGuiCol_SeparatorActive] = parse<vec4>(ss); break;
			case hash("ResizeGrip")				: style.Colors[ImGuiCol_ResizeGrip] = parse<vec4>(ss); break;
			case hash("ResizeGripHovered")		: style.Colors[ImGuiCol_ResizeGripHovered] = parse<vec4>(ss); break;
			case hash("ResizeGripActive")		: style.Colors[ImGuiCol_ResizeGripActive] = parse<vec4>(ss); break;
			case hash("Tab")					: style.Colors[ImGuiCol_Tab] = parse<vec4>(ss); break;
			case hash("TabHovered")				: style.Colors[ImGuiCol_TabHovered] = parse<vec4>(ss); break;
			case hash("TabActive")				: style.Colors[ImGuiCol_TabActive] = parse<vec4>(ss); break;
			case hash("TabUnfocused")			: style.Colors[ImGuiCol_TabUnfocused] = parse<vec4>(ss); break;
			case hash("TabUnfocusedActive")		: style.Colors[ImGuiCol_TabUnfocusedActive] = parse<vec4>(ss); break;
			case hash("DockingPreview")			: style.Colors[ImGuiCol_DockingPreview] = parse<vec4>(ss); break;
			case hash("DockingEmptyBg")			: style.Colors[ImGuiCol_DockingEmptyBg] = parse<vec4>(ss); break;
			case hash("PlotLines")				: style.Colors[ImGuiCol_PlotLines] = parse<vec4>(ss); break;
			case hash("PlotLinesHovered")		: style.Colors[ImGuiCol_PlotLinesHovered] = parse<vec4>(ss); break;
			case hash("PlotHistogram")			: style.Colors[ImGuiCol_PlotHistogram] = parse<vec4>(ss); break;
			case hash("PlotHistogramHovered")	: style.Colors[ImGuiCol_PlotHistogramHovered] = parse<vec4>(ss); break;
			case hash("TextSelectedBg")			: style.Colors[ImGuiCol_TextSelectedBg] = parse<vec4>(ss); break;
			case hash("DragDropTarget")			: style.Colors[ImGuiCol_DragDropTarget] = parse<vec4>(ss); break;
			case hash("NavHighlight")			: style.Colors[ImGuiCol_NavHighlight] = parse<vec4>(ss); break;
			case hash("NavWindowingHighlight")	: style.Colors[ImGuiCol_NavWindowingHighlight] = parse<vec4>(ss); break;
			case hash("NavWindowingDimBg")		: style.Colors[ImGuiCol_NavWindowingDimBg] = parse<vec4>(ss); break;
			case hash("ModalWindowDimBg")		: style.Colors[ImGuiCol_ModalWindowDimBg] = parse<vec4>(ss); break;
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

	void gui_manager::show_imgui_style_editor(void * ref)
	{
		ImGui::ShowStyleEditor((ImGuiStyle *)ref);
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