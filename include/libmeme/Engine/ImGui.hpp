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

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_WINDOW_GLFW) && defined(ML_IMPL_RENDERER_OPENGL)
#	include <imgui/examples/imgui_impl_glfw.h>
#	include <imgui/examples/imgui_impl_opengl3.h>
#else
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ImGuiColorTextEdit/TextEditor.h>
#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>
#include <imgui-node-editor/NodeEditor/Include/imgui_node_editor.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// ImGui::PushID followed by deferred ImGui::PopID
#define ML_scoped_imgui_id(...) \
	auto ML_anon = _ML impl::scoped_imgui_id{ ##__VA_ARGS__ }

namespace ml::impl
{
	struct ML_NODISCARD scoped_imgui_id final
	{
		template <class ... Args
		> scoped_imgui_id(Args && ... args) noexcept { ImGui::PushID(ML_forward(args)...); }

		~scoped_imgui_id() noexcept { ImGui::PopID(); }
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMGUI_HPP_