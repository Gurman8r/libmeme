#ifndef _ML_IMGUI_HPP_
#define _ML_IMGUI_HPP_

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <libmeme/Core/ScopeGuard.hpp>

#define ML_ImGui_ScopeID(id) ImGui::PushID(id); ML_SCOPE_EXIT{ ImGui::PopID(); };

#endif // !_ML_IMGUI_HPP_