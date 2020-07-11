#ifndef _ML_IMGUI_HPP_
#define _ML_IMGUI_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/System/Debug.hpp>
#include <libmeme/Core/Color.hpp>

#ifndef IM_VEC2_CLASS_EXTRA
#define IM_VEC2_CLASS_EXTRA								\
	constexpr ImVec2(_ML vec2 const & v) noexcept		\
		: x{ v[0] }, y{ v[1] }							\
	{}													\
	constexpr operator _ML vec2() const noexcept {		\
		return { x, y };								\
	}
#endif

#ifndef IM_VEC4_CLASS_EXTRA
#define IM_VEC4_CLASS_EXTRA								\
	constexpr ImVec4(_ML vec4 const & v) noexcept		\
		: x{ v[0] }, y{ v[1] }, z{ v[2] }, w{ v[3] }	\
	{}													\
	constexpr ImVec4(_ML color const & c) noexcept		\
		: x{ c[0] }, y{ c[1] }, z{ c[2] }, w{ c[3] }	\
	{}													\
	constexpr operator _ML vec4() const noexcept {		\
		return { x, y, z, w };							\
	}
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ImGuiColorTextEdit/TextEditor.h>
#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>
#include <imgui-node-editor/NodeEditor/Include/imgui_node_editor.h>
#include <implot/implot.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// ImGui::PushID(...); ML_defer(&){ ImGui::PopID(); };
#define ML_ImGui_ScopeID(...) \
	auto ML_anon = _ML impl::imgui_scope_id{ ##__VA_ARGS__ }

namespace ml::impl
{
	struct ML_NODISCARD imgui_scope_id final
	{
		template <class ... Args
		> imgui_scope_id(Args && ... args) noexcept
		{
			ImGui::PushID(ML_forward(args)...);
		}

		~imgui_scope_id() noexcept
		{
			ImGui::PopID();
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMGUI_HPP_