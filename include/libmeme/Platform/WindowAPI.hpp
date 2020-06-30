#ifndef _ML_WINDOW_API_HPP_
#define _ML_WINDOW_API_HPP_

#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Duration.hpp>
#include <libmeme/Platform/ContextSettings.hpp>
#include <libmeme/Platform/VideoMode.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD window_settings final
	{
		pmr::string			title	{};
		video_mode			video	{};
		context_settings	context	{};
		int32_t				hints	{};
	};

	inline void from_json(json const & j, window_settings & value)
	{
		j.at("title").get_to(value.title);
		j.at("video").get_to(value.video);
		j.at("context").get_to(value.context);
		j.at("hints").get_to(value.hints);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_handle(cursor_handle)	; // cursor handle
	ML_decl_handle(monitor_handle)	; // monitor handle
	ML_decl_handle(window_handle)	; // window handle
	ML_decl_handle(window_proc)		; // window procedure

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias window_char_fn				= typename void(*)(window_handle, uint32_t);
	ML_alias window_char_mods_fn		= typename void(*)(window_handle, uint32_t, int32_t);
	ML_alias window_close_fn			= typename void(*)(window_handle);
	ML_alias window_content_scale_fn	= typename void(*)(window_handle, float_t, float_t);
	ML_alias window_cursor_enter_fn		= typename void(*)(window_handle, int32_t);
	ML_alias window_cursor_position_fn	= typename void(*)(window_handle, float64_t, float64_t);
	ML_alias window_drop_fn				= typename void(*)(window_handle, int32_t, cstring[]);
	ML_alias window_error_fn			= typename void(*)(int32_t, cstring);
	ML_alias window_focus_fn			= typename void(*)(window_handle, int32_t);
	ML_alias window_framebuffer_size_fn	= typename void(*)(window_handle, int32_t, int32_t);
	ML_alias window_iconify_fn			= typename void(*)(window_handle, int32_t);
	ML_alias window_key_fn				= typename void(*)(window_handle, int32_t, int32_t, int32_t, int32_t);
	ML_alias window_maximize_fn			= typename void(*)(window_handle, int32_t);
	ML_alias window_mouse_fn			= typename void(*)(window_handle, int32_t, int32_t, int32_t);
	ML_alias window_position_fn			= typename void(*)(window_handle, int32_t, int32_t);
	ML_alias window_refresh_fn			= typename void(*)(window_handle);
	ML_alias window_scroll_fn			= typename void(*)(window_handle, float64_t, float64_t);
	ML_alias window_size_fn				= typename void(*)(window_handle, int32_t, int32_t);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum window_attr_ : int32_t
	{
		window_attr_focused,
		window_attr_iconified,
		window_attr_resizable,
		window_attr_visible,
		window_attr_decorated,
		window_attr_auto_iconify,
		window_attr_floating,
		window_attr_maximized,
		window_attr_center_cursor,
		window_attr_transparent_framebuffer,
		window_attr_hovered,
		window_attr_focus_on_show,

		window_attr_red_bits,
		window_attr_green_bits,
		window_attr_blue_bits,
		window_attr_alpha_bits,
		window_attr_depth_bits,
		window_attr_stencil_bits,
		window_attr_accum_red_bits,
		window_attr_accum_green_bits,
		window_attr_accum_blue_bits,
		window_attr_accum_alpha_bits,
		window_attr_aux_buffers,
		window_attr_stereo,
		window_attr_samples,
		window_attr_srgb_capable,
		window_attr_refresh_rate,
		window_attr_doublebuffer,

		window_attr_client_api,
		window_attr_context_version_major,
		window_attr_context_version_minor,
		window_attr_context_revision,
		window_attr_context_robustness,
		window_attr_backend_forward_compat,
		window_attr_backend_debug_context,
		window_attr_backend_profile,
		window_attr_context_release_behavior,
		window_attr_context_no_error,
		window_attr_context_creation_api,
		window_attr_scale_to_monitor,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum window_client_ : int32_t
	{
		window_client_unknown	, // unknown
		window_client_opengl	, // opengl
		window_client_vulkan	, // vulkan
		window_client_directx	, // directx
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum window_profile_ : int32_t
	{
		window_profile_any		, // any
		window_profile_core		, // core
		window_profile_compat	, // compat
		window_profile_debug	, // debug
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum window_hints_ : int32_t
	{
		window_hints_none				= 0,		// none
		window_hints_resizable			= (1 << 0),	// resizable	
		window_hints_visible			= (1 << 1),	// visible	
		window_hints_decorated			= (1 << 2),	// decorated	
		window_hints_focused			= (1 << 3),	// focused	
		window_hints_auto_iconify		= (1 << 4),	// auto_iconify
		window_hints_floating			= (1 << 5),	// floating	
		window_hints_maximized			= (1 << 6),	// maximized	
		window_hints_doublebuffer		= (1 << 7),	// doublebuffer

		// resizable / visible / decorated / focused / auto iconify
		window_hints_default
			= window_hints_resizable
			| window_hints_visible
			| window_hints_decorated
			| window_hints_focused
			| window_hints_auto_iconify,

		// resizable / decorated / focused / auto iconify / maximized
		window_hints_default_maximized
			= window_hints_resizable
			| window_hints_decorated
			| window_hints_focused
			| window_hints_auto_iconify
			| window_hints_maximized,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// cursor modes
	enum cursor_mode_ : int32_t
	{
		cursor_mode_normal		, // normal
		cursor_mode_hidden		, // hidden
		cursor_mode_disabled	, // disabled
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// cursor shapes
	enum cursor_shape_ : int32_t
	{
		cursor_shape_arrow,
		cursor_shape_ibeam,
		cursor_shape_crosshair,
		cursor_shape_pointing_hand,
		cursor_shape_ew,
		cursor_shape_ns,
		cursor_shape_nesw,
		cursor_shape_nwse,
		cursor_shape_resize_all,
		cursor_shape_not_allowed,
		cursor_shape_hresize,
		cursor_shape_vresize,
		cursor_shape_hand,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input state
	enum input_state_ : int32_t
	{
		input_state_release	, // release | high -> low
		input_state_press	, // press | low -> high
		input_state_repeat	, // repeat | high -> high
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input modifiers
	enum input_mods_ : int32_t
	{
		input_mods_none			= (0 << 0),	// none
		input_mods_shift		= (0 << 1),	// shift
		input_mods_ctrl			= (1 << 1),	// ctrl
		input_mods_alt			= (1 << 2),	// alt
		input_mods_super		= (1 << 3),	// super
		input_mods_caps_lock	= (1 << 4),	// caps
		input_mods_num_lock		= (1 << 5)	// numlock
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// key codes
	enum key_code_ : int32_t
	{
		key_code_space			= 32,	// space
		key_code_apostrophe		= 39,	// apostrophe
		key_code_comma			= 44,	// comma
		key_code_minus			= 45,	// minus
		key_code_period			= 46,	// period
		key_code_slash			= 47,	// slash
		key_code_num_0			= 48,	// 0
		key_code_num_1			= 49,	// 1
		key_code_num_2			= 50,	// 2
		key_code_num_3			= 51,	// 3
		key_code_num_4			= 52,	// 4
		key_code_num_5			= 53,	// 5
		key_code_num_6			= 54,	// 6
		key_code_num_7			= 55,	// 7
		key_code_num_8			= 56,	// 8
		key_code_num_9			= 57,	// 9
		key_code_semicolon		= 59,	// semicolon
		key_code_equal			= 61,	// equal
		key_code_a				= 65,	// a
		key_code_b				= 66,	// b
		key_code_c				= 67,	// c
		key_code_d				= 68,	// d
		key_code_e				= 69,	// e
		key_code_f				= 70,	// f
		key_code_g				= 71,	// g
		key_code_h				= 72,	// h
		key_code_i				= 73,	// i
		key_code_j				= 74,	// j
		key_code_k				= 75,	// k
		key_code_l				= 76,	// l
		key_code_m				= 77,	// m
		key_code_n				= 78,	// n
		key_code_o				= 79,	// o
		key_code_p				= 80,	// p
		key_code_q				= 81,	// q
		key_code_r				= 82,	// r
		key_code_s				= 83,	// s
		key_code_t				= 84,	// t
		key_code_u				= 85,	// u
		key_code_v				= 86,	// v
		key_code_w				= 87,	// w
		key_code_x				= 88,	// x
		key_code_y				= 89,	// y
		key_code_z				= 90,	// z
		key_code_left_bracket	= 91,	// left bracket
		key_code_backslash		= 92,	// backslash
		key_code_right_bracket	= 93,	// right bracket
		key_code_grave_accent	= 96,	// grave accent
		key_code_world_1		= 161,	// world 1
		key_code_world_2		= 162,	// world 2
		key_code_escape			= 256,	// escape
		key_code_enter			= 257,	// enter
		key_code_tab			= 258,	// tab
		key_code_backspace		= 259,	// backspace
		key_code_insert			= 260,	// insert
		key_code_del			= 261,	// del
		key_code_right			= 262,	// right
		key_code_left			= 263,	// left
		key_code_down			= 264,	// down
		key_code_up				= 265,	// up
		key_code_page_up		= 266,	// page up
		key_code_page_down		= 267,	// page down
		key_code_home			= 268,	// home
		key_code_end			= 269,	// end
		key_code_caps_lock		= 280,	// caps lock
		key_code_scroll_lock	= 281,	// scroll lock
		key_code_num_lock		= 282,	// num lock
		key_code_print_screen	= 283,	// print screen
		key_code_pause			= 284,	// pause
		key_code_fn_1			= 290,	// f1
		key_code_fn_2			= 291,	// f2
		key_code_fn_3			= 292,	// f3
		key_code_fn_4			= 293,	// f4
		key_code_fn_5			= 294,	// f5
		key_code_fn_6			= 295,	// f6
		key_code_fn_7			= 296,	// f7
		key_code_fn_8			= 297,	// f8
		key_code_fn_9			= 298,	// f9
		key_code_fn_10			= 299,	// f10
		key_code_fn_11			= 300,	// f11
		key_code_fn_12			= 301,	// f12
		key_code_fn_13			= 302,	// f13
		key_code_fn_14			= 303,	// f14
		key_code_fn_15			= 304,	// f15
		key_code_fn_16			= 305,	// f16
		key_code_fn_17			= 306,	// f17
		key_code_fn_18			= 307,	// f18
		key_code_fn_19			= 308,	// f19
		key_code_fn_20			= 309,	// f20
		key_code_fn_21			= 310,	// f21
		key_code_fn_22			= 311,	// f22
		key_code_fn_23			= 312,	// f23
		key_code_fn_24			= 313,	// f24
		key_code_fn_25			= 314,	// f25
		key_code_kp_0			= 320,	// kp 0
		key_code_kp_1			= 321,	// kp 1
		key_code_kp_2			= 322,	// kp 2
		key_code_kp_3			= 323,	// kp 3
		key_code_kp_4			= 324,	// kp 4
		key_code_kp_5			= 325,	// kp 5
		key_code_kp_6			= 326,	// kp 6
		key_code_kp_7			= 327,	// kp 7
		key_code_kp_8			= 328,	// kp 8
		key_code_kp_9			= 329,	// kp 9
		key_code_kp_decimal		= 330,	// kp decimal
		key_code_kp_divide		= 331,	// kp divide
		key_code_kp_multiply	= 332,	// kp multiply
		key_code_kp_subtract	= 333,	// kp subtract
		key_code_kp_add			= 334,	// kp add
		key_code_kp_enter		= 335,	// kp enter
		key_code_kp_equal		= 336,	// kp equal
		key_code_left_shift		= 340,	// left shift
		key_code_left_control	= 341,	// left control
		key_code_left_alt		= 342,	// left alt
		key_code_left_super		= 343,	// left super
		key_code_right_shift	= 344,	// right shift
		key_code_right_control	= 345,	// right control
		key_code_right_alt		= 346,	// right alt
		key_code_right_super	= 347,	// right super
		key_code_menu			= 348,	// menu
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// mouse buttons
	enum mouse_button_ : int32_t
	{
		mouse_button_0, // mb 0
		mouse_button_1, // mb 1
		mouse_button_2, // mb 2
		mouse_button_3, // mb 3
		mouse_button_4, // mb 4
		mouse_button_5, // mb 5
		mouse_button_6, // mb 6
		mouse_button_7, // mb 7
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_API_HPP_