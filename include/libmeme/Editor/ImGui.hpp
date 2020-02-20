#ifndef _ML_IMGUI_HPP_
#define _ML_IMGUI_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Matrix.hpp>

#ifndef IM_VEC2_CLASS_EXTRA
#define IM_VEC2_CLASS_EXTRA							\
	constexpr ImVec2(_ML vec2 const & v) noexcept	\
		: x{ (float)v[0] }							\
		, y{ (float)v[1] }							\
	{}												\
	constexpr operator _ML vec2() const noexcept {	\
		return { x, y };							\
	}
#endif

#ifndef IM_VEC4_CLASS_EXTRA
#define IM_VEC4_CLASS_EXTRA							\
	constexpr ImVec4(_ML vec4 const & v) noexcept	\
		: x{ (float)v[0] }							\
		, y{ (float)v[1] }							\
		, z{ (float)v[2] }							\
		, w{ (float)v[3] }							\
	{}												\
	constexpr operator _ML vec4() const noexcept {	\
		return { x, y, z, w };						\
	}
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#ifdef ML_RENDERER_OPENGL
#	include <imgui/examples/imgui_impl_glfw.h>
#	include <imgui/examples/imgui_impl_opengl3.h>
#endif

#include <ImGuiColorTextEdit/TextEditor.h>
#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/ScopeGuard.hpp>

#define ML_ImGui_ScopeID(id)	\
	ImGui::PushID(id);			\
	ML_DEFER{ ImGui::PopID(); };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMGUI_HPP_