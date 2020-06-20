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
		window_client_unknown,
		window_client_opengl,
		window_client_vulkan,
		window_client_directx,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum window_profile_ : int32_t
	{
		window_profile_any,
		window_profile_core,
		window_profile_compat,
		window_profile_debug,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum window_hints_ : int32_t
	{
		window_hints_none				= 0,
		window_hints_resizable			= (1 << 0),
		window_hints_visible			= (1 << 1),
		window_hints_decorated			= (1 << 2),
		window_hints_focused			= (1 << 3),
		window_hints_auto_iconify		= (1 << 4),
		window_hints_floating			= (1 << 5),
		window_hints_maximized			= (1 << 6),
		window_hints_doublebuffer		= (1 << 7),

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
		cursor_mode_normal,
		cursor_mode_hidden,
		cursor_mode_disabled,
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
		input_state_release,	// high -> low
		input_state_press,		// low -> high
		input_state_repeat		// high -> high
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
		key_code_space			= 32,
		key_code_apostrophe		= 39,
		key_code_comma			= 44,
		key_code_minus			= 45,
		key_code_period			= 46,
		key_code_slash			= 47,
		key_code_num0			= 48,
		key_code_num1			= 49,
		key_code_num2			= 50,
		key_code_num3			= 51,
		key_code_num4			= 52,
		key_code_num5			= 53,
		key_code_num6			= 54,
		key_code_num7			= 55,
		key_code_num8			= 56,
		key_code_num9			= 57,
		key_code_semicolon		= 59,
		key_code_equal			= 61,
		key_code_a				= 65,
		key_code_b				= 66,
		key_code_c				= 67,
		key_code_d				= 68,
		key_code_e				= 69,
		key_code_f				= 70,
		key_code_g				= 71,
		key_code_h				= 72,
		key_code_i				= 73,
		key_code_j				= 74,
		key_code_k				= 75,
		key_code_l				= 76,
		key_code_m				= 77,
		key_code_n				= 78,
		key_code_o				= 79,
		key_code_p				= 80,
		key_code_q				= 81,
		key_code_r				= 82,
		key_code_s				= 83,
		key_code_t				= 84,
		key_code_u				= 85,
		key_code_v				= 86,
		key_code_w				= 87,
		key_code_x				= 88,
		key_code_y				= 89,
		key_code_z				= 90,
		key_code_left_bracket	= 91,
		key_code_backslash		= 92,
		key_code_right_bracket	= 93,
		key_code_grave_accent	= 96,
		key_code_world_1		= 161,
		key_code_world_2		= 162,
		key_code_escape			= 256,
		key_code_enter			= 257,
		key_code_tab			= 258,
		key_code_backspace		= 259,
		key_code_insert			= 260,
		key_code_del			= 261,
		key_code_right			= 262,
		key_code_left			= 263,
		key_code_down			= 264,
		key_code_up				= 265,
		key_code_page_up		= 266,
		key_code_page_down		= 267,
		key_code_home			= 268,
		key_code_end			= 269,
		key_code_caps_lock		= 280,
		key_code_scroll_lock	= 281,
		key_code_num_lock		= 282,
		key_code_print_screen	= 283,
		key_code_pause			= 284,
		key_code_f1				= 290,
		key_code_f2				= 291,
		key_code_f3				= 292,
		key_code_f4				= 293,
		key_code_f5				= 294,
		key_code_f6				= 295,
		key_code_f7				= 296,
		key_code_f8				= 297,
		key_code_f9				= 298,
		key_code_f10			= 299,
		key_code_f11			= 300,
		key_code_f12			= 301,
		key_code_f13			= 302,
		key_code_f14			= 303,
		key_code_f15			= 304,
		key_code_f16			= 305,
		key_code_f17			= 306,
		key_code_f18			= 307,
		key_code_f19			= 308,
		key_code_f20			= 309,
		key_code_f21			= 310,
		key_code_f22			= 311,
		key_code_f23			= 312,
		key_code_f24			= 313,
		key_code_f25			= 314,
		key_code_kp_0			= 320,
		key_code_kp_1			= 321,
		key_code_kp_2			= 322,
		key_code_kp_3			= 323,
		key_code_kp_4			= 324,
		key_code_kp_5			= 325,
		key_code_kp_6			= 326,
		key_code_kp_7			= 327,
		key_code_kp_8			= 328,
		key_code_kp_9			= 329,
		key_code_kp_decimal		= 330,
		key_code_kp_divide		= 331,
		key_code_kp_multiply	= 332,
		key_code_kp_subtract	= 333,
		key_code_kp_add			= 334,
		key_code_kp_enter		= 335,
		key_code_kp_equal		= 336,
		key_code_left_shift		= 340,
		key_code_left_control	= 341,
		key_code_left_alt		= 342,
		key_code_left_super		= 343,
		key_code_right_shift	= 344,
		key_code_right_control	= 345,
		key_code_right_alt		= 346,
		key_code_right_super	= 347,
		key_code_menu			= 348,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// mouse buttons
	enum mouse_button_ : int32_t
	{
		mouse_button_0,
		mouse_button_1,
		mouse_button_2,
		mouse_button_3,
		mouse_button_4,
		mouse_button_5,
		mouse_button_6,
		mouse_button_7,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_API_HPP_