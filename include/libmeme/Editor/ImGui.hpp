#ifndef _ML_IMGUI_HPP_
#define _ML_IMGUI_HPP_

#include <libmeme/Core/Matrix.hpp>

#ifndef IM_VEC2_CLASS_EXTRA
#define IM_VEC2_CLASS_EXTRA \
	inline operator _ML vec2() const noexcept { return { x, y }; }
#endif

#ifndef IM_VEC4_CLASS_EXTRA
#define IM_VEC4_CLASS_EXTRA \
	inline operator _ML vec4() const noexcept { return { x, y }; }
#endif

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <libmeme/Core/ScopeGuard.hpp>

#define ML_ImGui_ScopeID(id)	\
	ImGui::PushID(id);			\
	ML_DEFER{ ImGui::PopID(); };

#endif // !_ML_IMGUI_HPP_