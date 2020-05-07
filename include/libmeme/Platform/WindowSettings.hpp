#ifndef _ML_WINDOW_SETTINGS_HPP_
#define _ML_WINDOW_SETTINGS_HPP_

#include <libmeme/Platform/ContextSettings.hpp>
#include <libmeme/Platform/VideoMode.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias cursor_handle = typename void *;
	ML_alias window_handle = typename void *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using window_char_fn			= typename void(*)(window_handle, uint32_t);
	using window_close_fn			= typename void(*)(window_handle);
	using window_cursor_enter_fn	= typename void(*)(window_handle, int32_t);
	using window_cursor_pos_fn		= typename void(*)(window_handle, float64_t, float64_t);
	using window_error_fn			= typename void(*)(int32_t, cstring);
	using window_focus_fn			= typename void(*)(window_handle, int32_t);
	using window_frame_size_fn		= typename void(*)(window_handle, int32_t, int32_t);
	using window_key_fn				= typename void(*)(window_handle, int32_t, int32_t, int32_t, int32_t);
	using window_mouse_fn			= typename void(*)(window_handle, int32_t, int32_t, int32_t);
	using window_position_fn		= typename void(*)(window_handle, int32_t, int32_t);
	using window_scroll_fn			= typename void(*)(window_handle, float64_t, float64_t);
	using window_size_fn			= typename void(*)(window_handle, int32_t, int32_t);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum window_hints_ : int32_t
	{
		window_hints_none				= (0 << 0), // none
		window_hints_resizable			= (1 << 0), // resizable
		window_hints_visible			= (1 << 1), // visible
		window_hints_decorated			= (1 << 2), // decorated
		window_hints_focused			= (1 << 3), // focused
		window_hints_auto_iconify		= (1 << 4), // auto iconify
		window_hints_floating			= (1 << 5), // floating
		window_hints_maximized			= (1 << 6), // maximized
		window_hints_fullscreen			= (1 << 7), // fullscreen
		window_hints_double_buffered	= (1 << 8), // double buffered
		window_hints_install_callbacks	= (1 << 9), // install callbacks

		// resizable / visible / decorated / focused / auto iconify / install callbacks
		window_hints_default
			= window_hints_resizable
			| window_hints_visible
			| window_hints_decorated
			| window_hints_focused
			| window_hints_auto_iconify
			| window_hints_install_callbacks,

		// resizable / decorated / focused / auto iconify / maximized / install callbacks
		window_hints_default_max
			= window_hints_resizable
			| window_hints_decorated
			| window_hints_focused
			| window_hints_auto_iconify
			| window_hints_maximized
			| window_hints_install_callbacks,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD window_settings final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pmr::string			title	{};
		video_mode			video	{};
		context_settings	context	{};
		int32_t				hints	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static void from_json(json const & j, window_settings & value)
	{
		j.at("title").get_to(value.title);
		j.at("video").get_to(value.video);
		j.at("context").get_to(value.context);
		j.at("hints").get_to(value.hints);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_SETTINGS_HPP_