#ifndef _ML_IMGUI_HPP_
#define _ML_IMGUI_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Matrix.hpp>

#ifndef IM_VEC2_CLASS_EXTRA
#define IM_VEC2_CLASS_EXTRA									\
	template <class T = _ML float_t							\
	> constexpr ImVec2(_ML tvec2<T> const & v) noexcept		\
		: x{ (float)v[0] }									\
		, y{ (float)v[1] }									\
	{}														\
	template <class T = _ML float_t							\
	> constexpr operator _ML tvec2<T>() const noexcept {	\
		return { (T)x, (T)y };								\
	 }
#endif

#ifndef IM_VEC4_CLASS_EXTRA
#define IM_VEC4_CLASS_EXTRA									\
	template <class T = _ML float_t							\
	> constexpr ImVec4(_ML tvec4<T> const & v) noexcept		\
		: x{ (float)v[0] }									\
		, y{ (float)v[1] }									\
		, z{ (float)v[2] }									\
		, w{ (float)v[3] }									\
	{}														\
	template <class T = _ML float_t							\
	> constexpr operator _ML tvec4<T>() const noexcept {	\
		return { (T)x, (T)y, (T)z, (T)w };					\
	}
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/ScopeGuard.hpp>

#define ML_ImGui_ScopeID(id)	\
	ImGui::PushID(id);			\
	ML_DEFER{ ImGui::PopID(); };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMGUI_HPP_