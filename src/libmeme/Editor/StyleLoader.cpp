#include <libmeme/Editor/StyleLoader.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Core/Input.hpp>

namespace ml
{
	style_loader::style_loader()
	{
	}

	bool style_loader::operator()(fs::path const & path)
	{
		std::ifstream file{ path };
		ML_DEFER{ file.close(); };
		if (!file) return false;

		pmr::string line;
		while (std::getline(file, line))
		{
			line = util::trim(line);

			if (line.empty() || line.front() == '#')
				continue;

			std::stringstream ss{}; ss << line;

			if (auto tag = input<pmr::string>(ss); !tag.empty())
			{
				if (auto name = input<pmr::string>(ss); !name.empty())
				{
					auto & s{ ImGui::GetStyle() };
					auto & c{ s.Colors };
					if (tag == "ImGuiStyle")
					{
						switch (util::hash(name))
						{
						case util::hash("Alpha")					: s.Alpha = input<float_t>(ss); break;
						case util::hash("WindowPadding")			: s.WindowPadding = input<vec2>(ss); break;
						case util::hash("WindowRounding")			: s.WindowRounding = input<float_t>(ss); break;
						case util::hash("WindowBorderSize")			: s.WindowBorderSize = input<float_t>(ss); break;
						case util::hash("WindowMinSize")			: s.WindowMinSize = input<vec2>(ss); break;
						case util::hash("WindowTitleAlign")			: s.WindowTitleAlign = input<vec2>(ss); break;
						case util::hash("ChildRounding")			: s.ChildRounding = input<float_t>(ss); break;
						case util::hash("ChildBorderSize")			: s.ChildBorderSize = input<float_t>(ss); break;
						case util::hash("PopupRounding")			: s.PopupRounding = input<float_t>(ss); break;
						case util::hash("PopupBorderSize")			: s.PopupBorderSize = input<float_t>(ss); break;
						case util::hash("FramePadding")				: s.FramePadding = input<vec2>(ss); break;
						case util::hash("FrameRounding")			: s.FrameRounding = input<float_t>(ss); break;
						case util::hash("FrameBorderSize")			: s.FrameBorderSize = input<float_t>(ss); break;
						case util::hash("ItemSpacing")				: s.ItemInnerSpacing = input<vec2>(ss); break;
						case util::hash("ItemInnerSpacing")			: s.ItemInnerSpacing = input<vec2>(ss); break;
						case util::hash("TouchExtraPadding")		: s.TouchExtraPadding = input<vec2>(ss); break;
						case util::hash("IndentSpacing")			: s.IndentSpacing = input<float_t>(ss); break;
						case util::hash("ColumnsMinSpacing")		: s.ColumnsMinSpacing = input<float_t>(ss); break;
						case util::hash("ScrollbarSize")			: s.ScrollbarSize = input<float_t>(ss); break;
						case util::hash("ScrollbarRounding")		: s.ScrollbarRounding = input<float_t>(ss); break;
						case util::hash("GrabMinSize")				: s.GrabMinSize = input<float_t>(ss); break;
						case util::hash("GrabRounding")				: s.GrabRounding = input<float_t>(ss); break;
						case util::hash("TabRounding")				: s.TabRounding = input<float_t>(ss); break;
						case util::hash("TabBorderSize")			: s.TabBorderSize = input<float_t>(ss); break;
						case util::hash("ButtonTextAlign")			: s.ButtonTextAlign = input<vec2>(ss); break;
						case util::hash("SelectableTextAlign")		: s.SelectableTextAlign = input<vec2>(ss); break;
						case util::hash("DisplayWindowPadding")		: s.DisplayWindowPadding = input<vec2>(ss); break;
						case util::hash("DisplaySafeAreaPadding")	: s.DisplaySafeAreaPadding = input<vec2>(ss); break;
						case util::hash("MouseCursorScale")			: s.MouseCursorScale = input<float_t>(ss); break;
						case util::hash("AntiAliasedLines")			: s.AntiAliasedLines = input<bool>(ss); break;
						case util::hash("AntiAliasedFill")			: s.AntiAliasedFill = input<bool>(ss); break;
						case util::hash("CurveTessellationTol")		: s.CurveTessellationTol = input<float_t>(ss); break;
						}

					}
					else if (tag == "ImGuiCol")
					{
						switch (util::hash(name))
						{
						case util::hash("Text")					: c[ImGuiCol_Text] = input<vec4>(ss); break;
						case util::hash("TextDisabled")			: c[ImGuiCol_TextDisabled] = input<vec4>(ss); break;
						case util::hash("WindowBg")				: c[ImGuiCol_WindowBg] = input<vec4>(ss); break;
						case util::hash("ChildBg")				: c[ImGuiCol_ChildBg] = input<vec4>(ss); break;
						case util::hash("PopupBg")				: c[ImGuiCol_PopupBg] = input<vec4>(ss); break;
						case util::hash("Border")				: c[ImGuiCol_Border] = input<vec4>(ss); break;
						case util::hash("BorderShadow")			: c[ImGuiCol_BorderShadow] = input<vec4>(ss); break;
						case util::hash("FrameBg")				: c[ImGuiCol_FrameBg] = input<vec4>(ss); break;
						case util::hash("FrameBgHovered")		: c[ImGuiCol_FrameBgHovered] = input<vec4>(ss); break;
						case util::hash("FrameBgActive")		: c[ImGuiCol_FrameBgActive] = input<vec4>(ss); break;
						case util::hash("TitleBg")				: c[ImGuiCol_TitleBg] = input<vec4>(ss); break;
						case util::hash("TitleBgActive")		: c[ImGuiCol_TitleBgActive] = input<vec4>(ss); break;
						case util::hash("TitleBgCollapsed")		: c[ImGuiCol_TitleBgCollapsed] = input<vec4>(ss); break;
						case util::hash("MenuBarBg")			: c[ImGuiCol_MenuBarBg] = input<vec4>(ss); break;
						case util::hash("ScrollbarBg")			: c[ImGuiCol_ScrollbarBg] = input<vec4>(ss); break;
						case util::hash("ScrollbarGrab")		: c[ImGuiCol_ScrollbarGrab] = input<vec4>(ss); break;
						case util::hash("ScrollbarGrabHovered")	: c[ImGuiCol_ScrollbarGrabHovered] = input<vec4>(ss); break;
						case util::hash("ScrollbarGrabActive")	: c[ImGuiCol_ScrollbarGrabActive] = input<vec4>(ss); break;
						case util::hash("CheckMark")			: c[ImGuiCol_CheckMark] = input<vec4>(ss); break;
						case util::hash("SliderGrab")			: c[ImGuiCol_SliderGrab] = input<vec4>(ss); break;
						case util::hash("SliderGrabActive")		: c[ImGuiCol_SliderGrabActive] = input<vec4>(ss); break;
						case util::hash("Button")				: c[ImGuiCol_Button] = input<vec4>(ss); break;
						case util::hash("ButtonHovered")		: c[ImGuiCol_ButtonHovered] = input<vec4>(ss); break;
						case util::hash("ButtonActive")			: c[ImGuiCol_ButtonActive] = input<vec4>(ss); break;
						case util::hash("Header")				: c[ImGuiCol_Header] = input<vec4>(ss); break;
						case util::hash("HeaderHovered")		: c[ImGuiCol_HeaderHovered] = input<vec4>(ss); break;
						case util::hash("HeaderActive")			: c[ImGuiCol_HeaderActive] = input<vec4>(ss); break;
						case util::hash("Separator")			: c[ImGuiCol_Separator] = input<vec4>(ss); break;
						case util::hash("SeparatorHovered")		: c[ImGuiCol_SeparatorHovered] = input<vec4>(ss); break;
						case util::hash("SeparatorActive")		: c[ImGuiCol_SeparatorActive] = input<vec4>(ss); break;
						case util::hash("ResizeGrip")			: c[ImGuiCol_ResizeGrip] = input<vec4>(ss); break;
						case util::hash("ResizeGripHovered")	: c[ImGuiCol_ResizeGripHovered] = input<vec4>(ss); break;
						case util::hash("ResizeGripActive")		: c[ImGuiCol_ResizeGripActive] = input<vec4>(ss); break;
						case util::hash("Tab")					: c[ImGuiCol_Tab] = input<vec4>(ss); break;
						case util::hash("TabHovered")			: c[ImGuiCol_TabHovered] = input<vec4>(ss); break;
						case util::hash("TabActive")			: c[ImGuiCol_TabActive] = input<vec4>(ss); break;
						case util::hash("TabUnfocused")			: c[ImGuiCol_TabUnfocused] = input<vec4>(ss); break;
						case util::hash("TabUnfocusedActive")	: c[ImGuiCol_TabUnfocusedActive] = input<vec4>(ss); break;
						case util::hash("DockingPreview")		: c[ImGuiCol_DockingPreview] = input<vec4>(ss); break;
						case util::hash("DockingEmptyBg")		: c[ImGuiCol_DockingEmptyBg] = input<vec4>(ss); break;
						case util::hash("PlotLines")			: c[ImGuiCol_PlotLines] = input<vec4>(ss); break;
						case util::hash("PlotLinesHovered")		: c[ImGuiCol_PlotLinesHovered] = input<vec4>(ss); break;
						case util::hash("PlotHistogram")		: c[ImGuiCol_PlotHistogram] = input<vec4>(ss); break;
						case util::hash("PlotHistogramHovered")	: c[ImGuiCol_PlotHistogramHovered] = input<vec4>(ss); break;
						case util::hash("TextSelectedBg")		: c[ImGuiCol_TextSelectedBg] = input<vec4>(ss); break;
						case util::hash("DragDropTarget")		: c[ImGuiCol_DragDropTarget] = input<vec4>(ss); break;
						case util::hash("NavHighlight")			: c[ImGuiCol_NavHighlight] = input<vec4>(ss); break;
						case util::hash("NavWindowingHighlight"): c[ImGuiCol_NavWindowingHighlight] = input<vec4>(ss); break;
						case util::hash("NavWindowingDimBg")	: c[ImGuiCol_NavWindowingDimBg] = input<vec4>(ss); break;
						case util::hash("ModalWindowDimBg")		: c[ImGuiCol_ModalWindowDimBg] = input<vec4>(ss); break;
						}
					}
					
				}
			}
		}
		return true;
	}
}