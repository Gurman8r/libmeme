#include <libmeme/Engine/Editor.hpp>
#include <libmeme/Engine/ImGuiExt.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Core/Events.hpp>
#include <libmeme/Core/FileUtility.hpp>
#include <libmeme/Core/ParserUtil.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>

// GLFW / OpenGL3
#if defined(ML_IMPL_WINDOW_GLFW) && defined(ML_IMPL_RENDERER_OPENGL)
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#define ML_ImGui_Init_Platform(wh, ic)	ImGui_ImplGlfw_InitForOpenGL((struct GLFWwindow *)wh, ic)
#define ML_ImGui_Init_Renderer()		ImGui_ImplOpenGL3_Init("#version 130")
#define ML_ImGui_Shutdown()				ML_scope(){ ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplGlfw_Shutdown(); }
#define ML_ImGui_NewFrame()				ML_scope(){ ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame(); }
#define ML_ImGui_RenderDrawData(x)		ImGui_ImplOpenGL3_RenderDrawData(x)

#else
#	error "imgui is unavailable"
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t editor_dockspace::begin_builder(int32_t flags)
	{
		if (uint32_t root{ ImGui::GetID(this->title) }; !ImGui::DockBuilderGetNode(root))
		{
			ImGui::DockBuilderRemoveNode(root);

			ImGui::DockBuilderAddNode(root, flags);

			return root;
		}
		return NULL;
	}

	uint32_t editor_dockspace::end_builder(uint32_t root)
	{
		if (root)
		{
			ImGui::DockBuilderFinish(root);
		}
		return root;
	}

	uint32_t editor_dockspace::dock(cstring name, uint32_t id)
	{
		if (name && id)
		{
			ImGui::DockBuilderDockWindow(name, id);
			return id;
		}
		return NULL;
	}

	uint32_t editor_dockspace::dock(gui::widget const & w, uint32_t id)
	{
		return dock(w.title, id);
	}

	uint32_t editor_dockspace::split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * value)
	{
		return this->nodes[(size_t)i] = split(id, dir, ratio, value);
	}

	uint32_t editor_dockspace::split(uint32_t id, int32_t dir, float_t ratio, uint32_t * value)
	{
		return split(id, dir, ratio, nullptr, value);
	}

	uint32_t editor_dockspace::split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * value)
	{
		return ImGui::DockBuilderSplitNode(id, dir, ratio, out, value);
	}

	void editor_dockspace::render(event_bus * bus)
	{
		if (!bus		||
			!visible	||
			!(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
		) { return; }

		ML_ImGui_ScopeID(this);

		// viewport
		ImGuiViewport const * v{ ImGui::GetMainViewport() };
		ImGui::SetNextWindowPos(v->Pos);
		ImGui::SetNextWindowSize(v->Size);
		ImGui::SetNextWindowViewport(v->ID);

		// style
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, rounding);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, border);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);
		ImGui::SetNextWindowBgAlpha(alpha);

		// begin
		if (ImGui::Begin(title, &visible,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoBackground |
			(menubar ? ImGuiWindowFlags_MenuBar : 0)
		))
		{
			ImGui::PopStyleVar(3);

			// fire docking event if nodes are empty
			if (nodes.empty()) { bus->fire<dockspace_event>(this); }

			ImGui::DockSpace(
				ImGui::GetID(title),
				size,
				ImGuiDockNodeFlags_PassthruCentralNode |
				ImGuiDockNodeFlags_AutoHideTabBar);

			ImGui::End();
		}

		// main menu bar
		if (menubar && ImGui::BeginMainMenuBar())
		{
			bus->fire<main_menu_bar_event>(this);

			ImGui::EndMainMenuBar();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	editor_context::editor_context(event_bus * bus, render_window * win, allocator_type alloc)
		: m_bus		{ bus }
		, m_win		{ win }
		, m_dock	{ alloc }
		, m_imgui	{}
	{
		ML_assert(m_bus && m_win);

		// check version
		ML_assert("failed validating imgui version" && IMGUI_CHECKVERSION());

		// set allocators
		ImGui::SetAllocatorFunctions
		(
			[](size_t s, auto) noexcept { return ml_malloc(s); },
			[](void * p, auto) noexcept { return ml_free(p); },
			nullptr
		);

		// create context
		ML_assert("failed creating imgui context" && (m_imgui = ImGui::CreateContext()));

		// configure context
		auto & im_io{ ImGui::GetIO() };
		im_io.LogFilename = nullptr;
		im_io.IniFilename = nullptr;
		im_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		im_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		im_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		if (win->is_open())
		{
			ML_assert(startup());
		}
	}

	editor_context::~editor_context()
	{
		shutdown();

		ImGui::DestroyContext((ImGuiContext *)m_imgui);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool editor_context::startup()
	{
		return
			(ML_ImGui_Init_Platform(m_win->get_handle(), true)
				|| debug::error("failed initializing imgui platform")) &&
			
			(ML_ImGui_Init_Renderer()
				|| debug::error("failed initializing imgui renderer"))
			;
	}

	void editor_context::shutdown()
	{
		ML_ImGui_Shutdown();
	}

	void editor_context::new_frame()
	{
		ML_assert(m_imgui && m_win);

		ML_ImGui_NewFrame();

		ImGui::NewFrame();

		m_dock.render(m_bus);
	}

	void editor_context::render_frame()
	{
		ML_assert(m_imgui && m_win);

		ImGui::Render();

		for (auto const & cmd : {
			gfx::command::set_viewport(m_win->get_framebuffer_size()),
			gfx::command::set_clear_color(colors::black),
			gfx::command::clear(gfx::clear_color),
		})	gfx::execute(cmd, m_win->get_render_context());

		ML_ImGui_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			auto backup_context{ window::get_context_current() };
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			window::make_context_current(backup_context);
		}

		if (m_win->has_hints(window_hints_doublebuffer))
		{
			window::swap_buffers(m_win->get_handle());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void editor_context::show_imgui_demo(bool * p_open) const
	{
		ImGui::ShowDemoWindow(p_open);
	}

	void editor_context::show_imgui_metrics(bool * p_open) const
	{
		ImGui::ShowMetricsWindow(p_open);
	}

	void editor_context::show_imgui_about(bool * p_open) const
	{
		ImGui::ShowAboutWindow(p_open);
	}

	void editor_context::show_imgui_style_editor(ImGuiStyle * ref) const
	{
		ImGui::ShowStyleEditor(ref);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool editor_context::load_style(fs::path const & path)
	{
		using namespace util;
	
		// empty
		if (path.empty()) return false;

		// builtin
		switch (hash(to_lower(path.string())))
		{
		case hash("classic"): ImGui::StyleColorsClassic()	; return true;
		case hash("dark")	: ImGui::StyleColorsDark()		; return true;
		case hash("light")	: ImGui::StyleColorsLight()		; return true;
		}
	
		// open file
		std::ifstream file{ path }; ML_defer(&file) { file.close(); };
		if (!file) return false;
	
		// style ref
		auto & style{ ImGui::GetStyle() };
	
		// scan file
		pmr::string line;
		while (std::getline(file, line))
		{
			if ((line = trim(line)).empty() || line.front() == '#') { continue; }
	
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
}