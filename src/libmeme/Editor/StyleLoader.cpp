#include <libmeme/Editor/StyleLoader.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Core/FileUtility.hpp>
#include <libmeme/Core/Input.hpp>
#include <libmeme/Core/Hash.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool style_loader::load_from_file(fs::path const & path)
	{
		// empty
		if (path.empty()) return false;

		// builtins
		if (!fs::exists(path))
		{
			switch (util::hash(util::to_lower(path.string())))
			{
			default: return false;
			case util::hash("classic")	: ImGui::StyleColorsClassic();
			case util::hash("dark")		: ImGui::StyleColorsDark();
			case util::hash("light")	: ImGui::StyleColorsLight(); 
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
			line = util::trim(line);
			
			// skip line
			if (line.empty() || line.front() == '#') { continue; }

			// scan line
			std::stringstream ss{}; ss << line;
			switch (util::hash(input<pmr::string>(ss)))
			{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			case util::hash("ImGuiStyle"): { switch (util::hash(input<pmr::string>(ss)))
			{
			case util::hash("Alpha")					: style.Alpha = input<float_t>(ss); break;
			case util::hash("WindowPadding")			: style.WindowPadding = input<vec2>(ss); break;
			case util::hash("WindowRounding")			: style.WindowRounding = input<float_t>(ss); break;
			case util::hash("WindowBorderSize")			: style.WindowBorderSize = input<float_t>(ss); break;
			case util::hash("WindowMinSize")			: style.WindowMinSize = input<vec2>(ss); break;
			case util::hash("WindowTitleAlign")			: style.WindowTitleAlign = input<vec2>(ss); break;
			case util::hash("ChildRounding")			: style.ChildRounding = input<float_t>(ss); break;
			case util::hash("ChildBorderSize")			: style.ChildBorderSize = input<float_t>(ss); break;
			case util::hash("PopupRounding")			: style.PopupRounding = input<float_t>(ss); break;
			case util::hash("PopupBorderSize")			: style.PopupBorderSize = input<float_t>(ss); break;
			case util::hash("FramePadding")				: style.FramePadding = input<vec2>(ss); break;
			case util::hash("FrameRounding")			: style.FrameRounding = input<float_t>(ss); break;
			case util::hash("FrameBorderSize")			: style.FrameBorderSize = input<float_t>(ss); break;
			case util::hash("ItemSpacing")				: style.ItemInnerSpacing = input<vec2>(ss); break;
			case util::hash("ItemInnerSpacing")			: style.ItemInnerSpacing = input<vec2>(ss); break;
			case util::hash("TouchExtraPadding")		: style.TouchExtraPadding = input<vec2>(ss); break;
			case util::hash("IndentSpacing")			: style.IndentSpacing = input<float_t>(ss); break;
			case util::hash("ColumnsMinSpacing")		: style.ColumnsMinSpacing = input<float_t>(ss); break;
			case util::hash("ScrollbarSize")			: style.ScrollbarSize = input<float_t>(ss); break;
			case util::hash("ScrollbarRounding")		: style.ScrollbarRounding = input<float_t>(ss); break;
			case util::hash("GrabMinSize")				: style.GrabMinSize = input<float_t>(ss); break;
			case util::hash("GrabRounding")				: style.GrabRounding = input<float_t>(ss); break;
			case util::hash("TabRounding")				: style.TabRounding = input<float_t>(ss); break;
			case util::hash("TabBorderSize")			: style.TabBorderSize = input<float_t>(ss); break;
			case util::hash("ButtonTextAlign")			: style.ButtonTextAlign = input<vec2>(ss); break;
			case util::hash("SelectableTextAlign")		: style.SelectableTextAlign = input<vec2>(ss); break;
			case util::hash("DisplayWindowPadding")		: style.DisplayWindowPadding = input<vec2>(ss); break;
			case util::hash("DisplaySafeAreaPadding")	: style.DisplaySafeAreaPadding = input<vec2>(ss); break;
			case util::hash("MouseCursorScale")			: style.MouseCursorScale = input<float_t>(ss); break;
			case util::hash("AntiAliasedLines")			: style.AntiAliasedLines = input<bool>(ss); break;
			case util::hash("AntiAliasedFill")			: style.AntiAliasedFill = input<bool>(ss); break;
			case util::hash("CurveTessellationTol")		: style.CurveTessellationTol = input<float_t>(ss); break;
			} } break;
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			case util::hash("ImGuiCol"): { switch (util::hash(input<pmr::string>(ss)))
			{
			case util::hash("Text")					: style.Colors[ImGuiCol_Text] = input<vec4>(ss); break;
			case util::hash("TextDisabled")			: style.Colors[ImGuiCol_TextDisabled] = input<vec4>(ss); break;
			case util::hash("WindowBg")				: style.Colors[ImGuiCol_WindowBg] = input<vec4>(ss); break;
			case util::hash("ChildBg")				: style.Colors[ImGuiCol_ChildBg] = input<vec4>(ss); break;
			case util::hash("PopupBg")				: style.Colors[ImGuiCol_PopupBg] = input<vec4>(ss); break;
			case util::hash("Border")				: style.Colors[ImGuiCol_Border] = input<vec4>(ss); break;
			case util::hash("BorderShadow")			: style.Colors[ImGuiCol_BorderShadow] = input<vec4>(ss); break;
			case util::hash("FrameBg")				: style.Colors[ImGuiCol_FrameBg] = input<vec4>(ss); break;
			case util::hash("FrameBgHovered")		: style.Colors[ImGuiCol_FrameBgHovered] = input<vec4>(ss); break;
			case util::hash("FrameBgActive")		: style.Colors[ImGuiCol_FrameBgActive] = input<vec4>(ss); break;
			case util::hash("TitleBg")				: style.Colors[ImGuiCol_TitleBg] = input<vec4>(ss); break;
			case util::hash("TitleBgActive")		: style.Colors[ImGuiCol_TitleBgActive] = input<vec4>(ss); break;
			case util::hash("TitleBgCollapsed")		: style.Colors[ImGuiCol_TitleBgCollapsed] = input<vec4>(ss); break;
			case util::hash("MenuBarBg")			: style.Colors[ImGuiCol_MenuBarBg] = input<vec4>(ss); break;
			case util::hash("ScrollbarBg")			: style.Colors[ImGuiCol_ScrollbarBg] = input<vec4>(ss); break;
			case util::hash("ScrollbarGrab")		: style.Colors[ImGuiCol_ScrollbarGrab] = input<vec4>(ss); break;
			case util::hash("ScrollbarGrabHovered")	: style.Colors[ImGuiCol_ScrollbarGrabHovered] = input<vec4>(ss); break;
			case util::hash("ScrollbarGrabActive")	: style.Colors[ImGuiCol_ScrollbarGrabActive] = input<vec4>(ss); break;
			case util::hash("CheckMark")			: style.Colors[ImGuiCol_CheckMark] = input<vec4>(ss); break;
			case util::hash("SliderGrab")			: style.Colors[ImGuiCol_SliderGrab] = input<vec4>(ss); break;
			case util::hash("SliderGrabActive")		: style.Colors[ImGuiCol_SliderGrabActive] = input<vec4>(ss); break;
			case util::hash("Button")				: style.Colors[ImGuiCol_Button] = input<vec4>(ss); break;
			case util::hash("ButtonHovered")		: style.Colors[ImGuiCol_ButtonHovered] = input<vec4>(ss); break;
			case util::hash("ButtonActive")			: style.Colors[ImGuiCol_ButtonActive] = input<vec4>(ss); break;
			case util::hash("Header")				: style.Colors[ImGuiCol_Header] = input<vec4>(ss); break;
			case util::hash("HeaderHovered")		: style.Colors[ImGuiCol_HeaderHovered] = input<vec4>(ss); break;
			case util::hash("HeaderActive")			: style.Colors[ImGuiCol_HeaderActive] = input<vec4>(ss); break;
			case util::hash("Separator")			: style.Colors[ImGuiCol_Separator] = input<vec4>(ss); break;
			case util::hash("SeparatorHovered")		: style.Colors[ImGuiCol_SeparatorHovered] = input<vec4>(ss); break;
			case util::hash("SeparatorActive")		: style.Colors[ImGuiCol_SeparatorActive] = input<vec4>(ss); break;
			case util::hash("ResizeGrip")			: style.Colors[ImGuiCol_ResizeGrip] = input<vec4>(ss); break;
			case util::hash("ResizeGripHovered")	: style.Colors[ImGuiCol_ResizeGripHovered] = input<vec4>(ss); break;
			case util::hash("ResizeGripActive")		: style.Colors[ImGuiCol_ResizeGripActive] = input<vec4>(ss); break;
			case util::hash("Tab")					: style.Colors[ImGuiCol_Tab] = input<vec4>(ss); break;
			case util::hash("TabHovered")			: style.Colors[ImGuiCol_TabHovered] = input<vec4>(ss); break;
			case util::hash("TabActive")			: style.Colors[ImGuiCol_TabActive] = input<vec4>(ss); break;
			case util::hash("TabUnfocused")			: style.Colors[ImGuiCol_TabUnfocused] = input<vec4>(ss); break;
			case util::hash("TabUnfocusedActive")	: style.Colors[ImGuiCol_TabUnfocusedActive] = input<vec4>(ss); break;
			case util::hash("DockingPreview")		: style.Colors[ImGuiCol_DockingPreview] = input<vec4>(ss); break;
			case util::hash("DockingEmptyBg")		: style.Colors[ImGuiCol_DockingEmptyBg] = input<vec4>(ss); break;
			case util::hash("PlotLines")			: style.Colors[ImGuiCol_PlotLines] = input<vec4>(ss); break;
			case util::hash("PlotLinesHovered")		: style.Colors[ImGuiCol_PlotLinesHovered] = input<vec4>(ss); break;
			case util::hash("PlotHistogram")		: style.Colors[ImGuiCol_PlotHistogram] = input<vec4>(ss); break;
			case util::hash("PlotHistogramHovered")	: style.Colors[ImGuiCol_PlotHistogramHovered] = input<vec4>(ss); break;
			case util::hash("TextSelectedBg")		: style.Colors[ImGuiCol_TextSelectedBg] = input<vec4>(ss); break;
			case util::hash("DragDropTarget")		: style.Colors[ImGuiCol_DragDropTarget] = input<vec4>(ss); break;
			case util::hash("NavHighlight")			: style.Colors[ImGuiCol_NavHighlight] = input<vec4>(ss); break;
			case util::hash("NavWindowingHighlight"): style.Colors[ImGuiCol_NavWindowingHighlight] = input<vec4>(ss); break;
			case util::hash("NavWindowingDimBg")	: style.Colors[ImGuiCol_NavWindowingDimBg] = input<vec4>(ss); break;
			case util::hash("ModalWindowDimBg")		: style.Colors[ImGuiCol_ModalWindowDimBg] = input<vec4>(ss); break;
			} } break;
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			}
		}
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}