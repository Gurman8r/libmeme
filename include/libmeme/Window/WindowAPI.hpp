#ifndef _ML_WINDOW_API_HPP_
#define _ML_WINDOW_API_HPP_

#include <libmeme/System/Timer.hpp>
#include <libmeme/Window/ContextSettings.hpp>
#include <libmeme/Window/Cursor.hpp>
#include <libmeme/Window/Input.hpp>
#include <libmeme/Window/Monitor.hpp>
#include <libmeme/Window/VideoMode.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_handle(window_handle); // window handle

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window hints
	enum window_hints_ : int32_t
	{
		window_hints_none				= (0 << 0),		// none
		window_hints_resizable			= (1 << 0),		// resizable
		window_hints_visible			= (1 << 1),		// visible
		window_hints_decorated			= (1 << 2),		// decorated
		window_hints_focused			= (1 << 3),		// focused
		window_hints_auto_iconify		= (1 << 4),		// auto iconify
		window_hints_floating			= (1 << 5),		// floating
		window_hints_maximized			= (1 << 6),		// maximized
		window_hints_doublebuffer		= (1 << 7),		// doublebuffer
		window_hints_center_cursor		= (1 << 8),		// center cursor
		window_hints_focus_on_show		= (1 << 9),		// focus on show

		// resizable / visible / decorated / focused / auto iconify / focus on show
		window_hints_default
			= window_hints_resizable
			| window_hints_visible
			| window_hints_decorated
			| window_hints_focused
			| window_hints_auto_iconify
			| window_hints_focus_on_show,

		// resizable / decorated / focused / auto iconify / maximized / focus on show
		window_hints_default_max
			= window_hints_resizable
			| window_hints_decorated
			| window_hints_focused
			| window_hints_auto_iconify
			| window_hints_maximized
			| window_hints_focus_on_show,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD window_settings final
	{
		pmr::string			title	{ "libmeme" };
		video_mode			video	{};
		context_settings	ctx		{};
		int32_t				hints	{ window_hints_default };
	};

	inline void from_json(json const & j, window_settings & value)
	{
		j["title"	].get_to(value.title);
		j["video"	].get_to(value.video);
		j["ctx"		].get_to(value.ctx);
		j["hints"	].get_to(value.hints);
	}

	inline void to_json(json & j, window_settings const & value)
	{
		j["title"	] = value.title;
		j["video"	] = value.video;
		j["ctx"		] = value.ctx;
		j["hints"	] = value.hints;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias window_char_callback				= typename void(*)(window_handle, uint32_t);
	ML_alias window_char_mods_callback			= typename void(*)(window_handle, uint32_t, int32_t);
	ML_alias window_close_callback				= typename void(*)(window_handle);
	ML_alias window_content_scale_callback		= typename void(*)(window_handle, float_t, float_t);
	ML_alias window_cursor_enter_callback		= typename void(*)(window_handle, int32_t);
	ML_alias window_cursor_position_callback	= typename void(*)(window_handle, float64_t, float64_t);
	ML_alias window_drop_callback				= typename void(*)(window_handle, int32_t, cstring[]);
	ML_alias window_error_callback				= typename void(*)(int32_t, cstring);
	ML_alias window_focus_callback				= typename void(*)(window_handle, int32_t);
	ML_alias window_framebuffer_size_callback	= typename void(*)(window_handle, int32_t, int32_t);
	ML_alias window_iconify_callback			= typename void(*)(window_handle, int32_t);
	ML_alias window_key_callback				= typename void(*)(window_handle, int32_t, int32_t, int32_t, int32_t);
	ML_alias window_maximize_callback			= typename void(*)(window_handle, int32_t);
	ML_alias window_mouse_callback				= typename void(*)(window_handle, int32_t, int32_t, int32_t);
	ML_alias window_position_callback			= typename void(*)(window_handle, int32_t, int32_t);
	ML_alias window_refresh_callback			= typename void(*)(window_handle);
	ML_alias window_scroll_callback				= typename void(*)(window_handle, float64_t, float64_t);
	ML_alias window_size_callback				= typename void(*)(window_handle, int32_t, int32_t);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_API_HPP_