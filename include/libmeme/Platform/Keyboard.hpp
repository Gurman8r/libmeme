#ifndef _ML_KEY_CODE_HPP_
#define _ML_KEY_CODE_HPP_

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum key_state_ : int32_t
	{
		key_state_release,	// high -> low
		key_state_press,	// low -> high
		key_state_repeat	// high -> high
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum key_mods_ : int32_t
	{
		key_mods_none,					// none
		key_mods_shift		= (0 << 1),	// shift
		key_mods_ctrl		= (1 << 1),	// ctrl
		key_mods_alt		= (1 << 2),	// alt
		key_mods_super		= (1 << 3),	// super
		key_mods_caps_lock	= (1 << 4),	// caps
		key_mods_num_lock	= (1 << 5)	// numlock
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum key_code_ : int32_t
	{
		key_code_invalid,

		key_code_space			= 32,	//
		key_code_apostrophe		= 39,	//
		key_code_comma			= 44,	//
		key_code_minus			= 45,	//
		key_code_period			= 46,	//
		key_code_slash			= 47,	//
		key_code_num0			= 48,	//
		key_code_num1			= 49,	//
		key_code_num2			= 50,	//
		key_code_num3			= 51,	//
		key_code_num4			= 52,	//
		key_code_num5			= 53,	//
		key_code_num6			= 54,	//
		key_code_num7			= 55,	//
		key_code_num8			= 56,	//
		key_code_num9			= 57,	//
		key_code_semicolon		= 59,	//
		key_code_equal			= 61,	//
		key_code_a				= 65,	//
		key_code_b				= 66,	//
		key_code_c				= 67,	//
		key_code_d				= 68,	//
		key_code_e				= 69,	//
		key_code_f				= 70,	//
		key_code_g				= 71,	//
		key_code_h				= 72,	//
		key_code_i				= 73,	//
		key_code_j				= 74,	//
		key_code_k				= 75,	//
		key_code_l				= 76,	//
		key_code_m				= 77,	//
		key_code_n				= 78,	//
		key_code_o				= 79,	//
		key_code_p				= 80,	//
		key_code_q				= 81,	//
		key_code_r				= 82,	//
		key_code_s				= 83,	//
		key_code_t				= 84,	//
		key_code_u				= 85,	//
		key_code_v				= 86,	//
		key_code_w				= 87,	//
		key_code_x				= 88,	//
		key_code_y				= 89,	//
		key_code_z				= 90,	//
		key_code_left_bracket	= 91,	//
		key_code_backslash		= 92,	//
		key_code_right_bracket	= 93,	//
		key_code_grave_accent	= 96,	//
		key_code_world_1		= 161,	// non-US #1
		key_code_world_2		= 162,	// non-US #2
		key_code_escape			= 256,	// 
		key_code_enter			= 257,	// 
		key_code_tab			= 258,	// 
		key_code_backspace		= 259,	// 
		key_code_insert			= 260,	// 
		key_code_del			= 261,	// 
		key_code_right			= 262,	// 
		key_code_left			= 263,	// 
		key_code_down			= 264,	// 
		key_code_up				= 265,	// 
		key_code_page_up		= 266,	// 
		key_code_page_down		= 267,	// 
		key_code_home			= 268,	// 
		key_code_end			= 269,	// 
		key_code_caps_lock		= 280,	// 
		key_code_scroll_lock	= 281,	// 
		key_code_num_lock		= 282,	// 
		key_code_print_screen	= 283,	// 
		key_code_pause			= 284,	// 
		key_code_f1				= 290,	// 
		key_code_f2				= 291,	// 
		key_code_f3				= 292,	// 
		key_code_f4				= 293,	// 
		key_code_f5				= 294,	// 
		key_code_f6				= 295,	// 
		key_code_f7				= 296,	// 
		key_code_f8				= 297,	// 
		key_code_f9				= 298,	// 
		key_code_f10			= 299,	// 
		key_code_f11			= 300,	// 
		key_code_f12			= 301,	// 
		key_code_f13			= 302,	// 
		key_code_f14			= 303,	// 
		key_code_f15			= 304,	// 
		key_code_f16			= 305,	// 
		key_code_f17			= 306,	// 
		key_code_f18			= 307,	// 
		key_code_f19			= 308,	// 
		key_code_f20			= 309,	// 
		key_code_f21			= 310,	// 
		key_code_f22			= 311,	// 
		key_code_f23			= 312,	// 
		key_code_f24			= 313,	// 
		key_code_f25			= 314,	// 
		key_code_kp_0			= 320,	// 
		key_code_kp_1			= 321,	// 
		key_code_kp_2			= 322,	// 
		key_code_kp_3			= 323,	// 
		key_code_kp_4			= 324,	// 
		key_code_kp_5			= 325,	// 
		key_code_kp_6			= 326,	// 
		key_code_kp_7			= 327,	// 
		key_code_kp_8			= 328,	// 
		key_code_kp_9			= 329,	// 
		key_code_kp_decimal		= 330,	// 
		key_code_kp_divide		= 331,	// 
		key_code_kp_multiply	= 332,	// 
		key_code_kp_subtract	= 333,	// 
		key_code_kp_add			= 334,	// 
		key_code_kp_enter		= 335,	// 
		key_code_kp_equal		= 336,	// 
		key_code_left_shift		= 340,	// 
		key_code_left_control	= 341,	// 
		key_code_left_alt		= 342,	// 
		key_code_left_super		= 343,	// 
		key_code_right_shift	= 344,	// 
		key_code_right_control	= 345,	// 
		key_code_right_alt		= 346,	// 
		key_code_right_super	= 347,	// 
		key_code_menu			= 348,	// 
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_KEY_CODE_HPP_