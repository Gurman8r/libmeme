#include <libmeme/Engine/Engine.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/API_Embed.hpp>

namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// LIBMEME (structures)
	PYBIND11_EMBEDDED_MODULE(libmeme, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// PROJECT
		struct ml_project {};
		py::class_<ml_project>(m, "project")
			.def(py::init<>())
			.def_property_readonly_static("author"	, [](py::object) { return ML__author; })
			.def_property_readonly_static("date"	, [](py::object) { return ML__date; })
			.def_property_readonly_static("libname"	, [](py::object) { return ML__libname; })
			.def_property_readonly_static("time"	, [](py::object) { return ML__time; })
			.def_property_readonly_static("url"		, [](py::object) { return ML__url; })
			.def_property_readonly_static("version"	, [](py::object) { return ML__version; })
			;

		// BUILD
		struct ml_build {};
		py::class_<ml_build>(m, "build")
			.def(py::init<>())
			.def_property_readonly_static("arch",			[](py::object) { return ML_arch; })
			.def_property_readonly_static("cc_name",		[](py::object) { return ML_cc_name; })
			.def_property_readonly_static("cc_version",		[](py::object) { return ML_cc_version; })
			.def_property_readonly_static("configuration",	[](py::object) { return ML_configuration; })
			.def_property_readonly_static("is_debug",		[](py::object) { return ML_is_debug; })
			.def_property_readonly_static("lang",			[](py::object) { return ML_lang; })
			.def_property_readonly_static("platform",		[](py::object) { return ML_platform; })
			.def_property_readonly_static("system",			[](py::object) { return ML_os_name; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// STDIO
		struct ml_stdio
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			struct output
			{
				std::reference_wrapper<std::ostream> m_os;

				output(std::ostream & os = std::cout) noexcept : m_os{ os }
				{
				}

				int32_t fileno() const noexcept
				{
					if (auto const addr{ std::addressof(m_os.get()) }; !addr) { return -2; }
					else if (addr == std::addressof(std::cout)) { return 1; }
					else if (addr == std::addressof(std::cerr)) { return 2; }
					else { return -1; }
				}

				void flush() noexcept { m_os.get().flush(); }

				void write(py::object o) noexcept { m_os << o; }

				void writelines(py::list l) noexcept { for (auto const & e : l) { m_os << e; } }
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static auto & cerr(py::object) noexcept
			{
				static output temp{ std::cerr }; return temp;
			}

			static auto & cout(py::object) noexcept
			{
				static output temp{ std::cout }; return temp;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};
		py::class_<ml_stdio::output>(m, "output_facade")
			.def(py::init<>())
			.def("closed"		, []() { return false; })
			.def("isatty"		, []() { return false; })
			.def("readable"		, []() { return false; })
			.def("seekable"		, []() { return false; })
			.def("writable"		, []() { return true; })
			.def("fileno"		, &ml_stdio::output::fileno)
			.def("flush"		, &ml_stdio::output::flush)
			.def("write"		, &ml_stdio::output::write)
			.def("writelines"	, &ml_stdio::output::writelines)
			;
		py::class_<ml_stdio>(m, "stdio")
			.def(py::init<>())
			.def_property_readonly_static("cerr", &ml_stdio::cerr)
			.def_property_readonly_static("cout", &ml_stdio::cout)
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// CURSOR MODE
		py::class_<cursor_mode_>(m, "cursor_mode")
			.def(py::init<>())
			.def_property_readonly_static("normal", [](py::object) { return (int32_t)cursor_mode_normal; })
			.def_property_readonly_static("hidden", [](py::object) { return (int32_t)cursor_mode_hidden; })
			.def_property_readonly_static("disabled", [](py::object) { return (int32_t)cursor_mode_disabled; })
			;

		// CURSOR SHAPE
		py::class_<cursor_shape_>(m, "cursor_shape")
			.def(py::init<>())
			.def_property_readonly_static("arrow", [](py::object) { return (int32_t)cursor_shape_arrow; })
			.def_property_readonly_static("ibeam", [](py::object) { return (int32_t)cursor_shape_ibeam; })
			.def_property_readonly_static("crosshair", [](py::object) { return (int32_t)cursor_shape_crosshair; })
			.def_property_readonly_static("hand", [](py::object) { return (int32_t)cursor_shape_hand; })
			.def_property_readonly_static("ew", [](py::object) { return (int32_t)cursor_shape_ew; })
			.def_property_readonly_static("ns", [](py::object) { return (int32_t)cursor_shape_ns; })
			.def_property_readonly_static("nesw", [](py::object) { return (int32_t)cursor_shape_nesw; })
			.def_property_readonly_static("nwse", [](py::object) { return (int32_t)cursor_shape_nwse; })
			;

		// INPUT STATE
		py::class_<input_state_>(m, "input_state")
			.def(py::init<>())
			.def_property_readonly_static("release", [](py::object) { return (int32_t)input_state_release; })
			.def_property_readonly_static("press", [](py::object) { return (int32_t)input_state_press; })
			.def_property_readonly_static("repeat", [](py::object) { return (int32_t)input_state_repeat; })
			;

		// INPUT MODS
		py::class_<input_mods_>(m, "input_mods")
			.def(py::init<>())
			.def_property_readonly_static("none", [](py::object) { return (int32_t)input_mods_none; })
			.def_property_readonly_static("shift", [](py::object) { return (int32_t)input_mods_shift; })
			.def_property_readonly_static("ctrl", [](py::object) { return (int32_t)input_mods_ctrl; })
			.def_property_readonly_static("alt", [](py::object) { return (int32_t)input_mods_alt; })
			.def_property_readonly_static("super", [](py::object) { return (int32_t)input_mods_super; })
			.def_property_readonly_static("caps_lock", [](py::object) { return (int32_t)input_mods_caps_lock; })
			.def_property_readonly_static("num_lock", [](py::object) { return (int32_t)input_mods_num_lock; })
			;

		// KEY CODE
		py::class_<key_code_>(m, "key_code")
			.def(py::init<>())
			.def_property_readonly_static("space", [](py::object) { return (int32_t)key_code_space; })
			.def_property_readonly_static("apostrophe", [](py::object) { return (int32_t)key_code_apostrophe; })
			.def_property_readonly_static("comma", [](py::object) { return (int32_t)key_code_comma; })
			.def_property_readonly_static("minus", [](py::object) { return (int32_t)key_code_minus; })
			.def_property_readonly_static("period", [](py::object) { return (int32_t)key_code_period; })
			.def_property_readonly_static("slash", [](py::object) { return (int32_t)key_code_slash; })
			.def_property_readonly_static("num0", [](py::object) { return (int32_t)key_code_num0; })
			.def_property_readonly_static("num1", [](py::object) { return (int32_t)key_code_num1; })
			.def_property_readonly_static("num2", [](py::object) { return (int32_t)key_code_num2; })
			.def_property_readonly_static("num3", [](py::object) { return (int32_t)key_code_num3; })
			.def_property_readonly_static("num4", [](py::object) { return (int32_t)key_code_num4; })
			.def_property_readonly_static("num5", [](py::object) { return (int32_t)key_code_num5; })
			.def_property_readonly_static("num6", [](py::object) { return (int32_t)key_code_num6; })
			.def_property_readonly_static("num7", [](py::object) { return (int32_t)key_code_num7; })
			.def_property_readonly_static("num8", [](py::object) { return (int32_t)key_code_num8; })
			.def_property_readonly_static("num9", [](py::object) { return (int32_t)key_code_num9; })
			.def_property_readonly_static("semicolon", [](py::object) { return (int32_t)key_code_semicolon; })
			.def_property_readonly_static("equal", [](py::object) { return (int32_t)key_code_equal; })
			.def_property_readonly_static("a", [](py::object) { return (int32_t)key_code_a; })
			.def_property_readonly_static("b", [](py::object) { return (int32_t)key_code_b; })
			.def_property_readonly_static("c", [](py::object) { return (int32_t)key_code_c; })
			.def_property_readonly_static("d", [](py::object) { return (int32_t)key_code_d; })
			.def_property_readonly_static("e", [](py::object) { return (int32_t)key_code_e; })
			.def_property_readonly_static("f", [](py::object) { return (int32_t)key_code_f; })
			.def_property_readonly_static("g", [](py::object) { return (int32_t)key_code_g; })
			.def_property_readonly_static("h", [](py::object) { return (int32_t)key_code_h; })
			.def_property_readonly_static("i", [](py::object) { return (int32_t)key_code_i; })
			.def_property_readonly_static("j", [](py::object) { return (int32_t)key_code_j; })
			.def_property_readonly_static("k", [](py::object) { return (int32_t)key_code_k; })
			.def_property_readonly_static("l", [](py::object) { return (int32_t)key_code_l; })
			.def_property_readonly_static("m", [](py::object) { return (int32_t)key_code_m; })
			.def_property_readonly_static("n", [](py::object) { return (int32_t)key_code_n; })
			.def_property_readonly_static("o", [](py::object) { return (int32_t)key_code_o; })
			.def_property_readonly_static("p", [](py::object) { return (int32_t)key_code_p; })
			.def_property_readonly_static("q", [](py::object) { return (int32_t)key_code_q; })
			.def_property_readonly_static("r", [](py::object) { return (int32_t)key_code_r; })
			.def_property_readonly_static("s", [](py::object) { return (int32_t)key_code_s; })
			.def_property_readonly_static("t", [](py::object) { return (int32_t)key_code_t; })
			.def_property_readonly_static("u", [](py::object) { return (int32_t)key_code_u; })
			.def_property_readonly_static("v", [](py::object) { return (int32_t)key_code_v; })
			.def_property_readonly_static("w", [](py::object) { return (int32_t)key_code_w; })
			.def_property_readonly_static("x", [](py::object) { return (int32_t)key_code_x; })
			.def_property_readonly_static("y", [](py::object) { return (int32_t)key_code_y; })
			.def_property_readonly_static("z", [](py::object) { return (int32_t)key_code_z; })
			.def_property_readonly_static("left_bracket", [](py::object) { return (int32_t)key_code_left_bracket; })
			.def_property_readonly_static("backslash", [](py::object) { return (int32_t)key_code_backslash; })
			.def_property_readonly_static("right_bracket", [](py::object) { return (int32_t)key_code_right_bracket; })
			.def_property_readonly_static("grave_accent", [](py::object) { return (int32_t)key_code_grave_accent; })
			.def_property_readonly_static("world_1", [](py::object) { return (int32_t)key_code_world_1; })
			.def_property_readonly_static("world_2", [](py::object) { return (int32_t)key_code_world_2; })
			.def_property_readonly_static("escape", [](py::object) { return (int32_t)key_code_escape; })
			.def_property_readonly_static("enter", [](py::object) { return (int32_t)key_code_enter; })
			.def_property_readonly_static("tab", [](py::object) { return (int32_t)key_code_tab; })
			.def_property_readonly_static("backspace", [](py::object) { return (int32_t)key_code_backspace; })
			.def_property_readonly_static("insert", [](py::object) { return (int32_t)key_code_insert; })
			.def_property_readonly_static("del", [](py::object) { return (int32_t)key_code_del; })
			.def_property_readonly_static("right", [](py::object) { return (int32_t)key_code_right; })
			.def_property_readonly_static("left", [](py::object) { return (int32_t)key_code_left; })
			.def_property_readonly_static("down", [](py::object) { return (int32_t)key_code_down; })
			.def_property_readonly_static("up", [](py::object) { return (int32_t)key_code_up; })
			.def_property_readonly_static("page_up", [](py::object) { return (int32_t)key_code_page_up; })
			.def_property_readonly_static("page_down", [](py::object) { return (int32_t)key_code_page_down; })
			.def_property_readonly_static("home", [](py::object) { return (int32_t)key_code_home; })
			.def_property_readonly_static("end", [](py::object) { return (int32_t)key_code_end; })
			.def_property_readonly_static("caps_lock", [](py::object) { return (int32_t)key_code_caps_lock; })
			.def_property_readonly_static("scroll_lock", [](py::object) { return (int32_t)key_code_scroll_lock; })
			.def_property_readonly_static("num_lock", [](py::object) { return (int32_t)key_code_num_lock; })
			.def_property_readonly_static("print_screen", [](py::object) { return (int32_t)key_code_print_screen; })
			.def_property_readonly_static("pause", [](py::object) { return (int32_t)key_code_pause; })
			.def_property_readonly_static("f1", [](py::object) { return (int32_t)key_code_f1; })
			.def_property_readonly_static("f2", [](py::object) { return (int32_t)key_code_f2; })
			.def_property_readonly_static("f3", [](py::object) { return (int32_t)key_code_f3; })
			.def_property_readonly_static("f4", [](py::object) { return (int32_t)key_code_f4; })
			.def_property_readonly_static("f5", [](py::object) { return (int32_t)key_code_f5; })
			.def_property_readonly_static("f6", [](py::object) { return (int32_t)key_code_f6; })
			.def_property_readonly_static("f7", [](py::object) { return (int32_t)key_code_f7; })
			.def_property_readonly_static("f8", [](py::object) { return (int32_t)key_code_f8; })
			.def_property_readonly_static("f9", [](py::object) { return (int32_t)key_code_f9; })
			.def_property_readonly_static("f10", [](py::object) { return (int32_t)key_code_f10; })
			.def_property_readonly_static("f11", [](py::object) { return (int32_t)key_code_f11; })
			.def_property_readonly_static("f12", [](py::object) { return (int32_t)key_code_f12; })
			.def_property_readonly_static("f13", [](py::object) { return (int32_t)key_code_f13; })
			.def_property_readonly_static("f14", [](py::object) { return (int32_t)key_code_f14; })
			.def_property_readonly_static("f15", [](py::object) { return (int32_t)key_code_f15; })
			.def_property_readonly_static("f16", [](py::object) { return (int32_t)key_code_f16; })
			.def_property_readonly_static("f17", [](py::object) { return (int32_t)key_code_f17; })
			.def_property_readonly_static("f18", [](py::object) { return (int32_t)key_code_f18; })
			.def_property_readonly_static("f19", [](py::object) { return (int32_t)key_code_f19; })
			.def_property_readonly_static("f20", [](py::object) { return (int32_t)key_code_f20; })
			.def_property_readonly_static("f21", [](py::object) { return (int32_t)key_code_f21; })
			.def_property_readonly_static("f22", [](py::object) { return (int32_t)key_code_f22; })
			.def_property_readonly_static("f23", [](py::object) { return (int32_t)key_code_f23; })
			.def_property_readonly_static("f24", [](py::object) { return (int32_t)key_code_f24; })
			.def_property_readonly_static("f25", [](py::object) { return (int32_t)key_code_f25; })
			.def_property_readonly_static("kp_0", [](py::object) { return (int32_t)key_code_kp_0; })
			.def_property_readonly_static("kp_1", [](py::object) { return (int32_t)key_code_kp_1; })
			.def_property_readonly_static("kp_2", [](py::object) { return (int32_t)key_code_kp_2; })
			.def_property_readonly_static("kp_3", [](py::object) { return (int32_t)key_code_kp_3; })
			.def_property_readonly_static("kp_4", [](py::object) { return (int32_t)key_code_kp_4; })
			.def_property_readonly_static("kp_5", [](py::object) { return (int32_t)key_code_kp_5; })
			.def_property_readonly_static("kp_6", [](py::object) { return (int32_t)key_code_kp_6; })
			.def_property_readonly_static("kp_7", [](py::object) { return (int32_t)key_code_kp_7; })
			.def_property_readonly_static("kp_8", [](py::object) { return (int32_t)key_code_kp_8; })
			.def_property_readonly_static("kp_9", [](py::object) { return (int32_t)key_code_kp_9; })
			.def_property_readonly_static("kp_decimal", [](py::object) { return (int32_t)key_code_kp_decimal; })
			.def_property_readonly_static("kp_divide", [](py::object) { return (int32_t)key_code_kp_divide; })
			.def_property_readonly_static("kp_multiply", [](py::object) { return (int32_t)key_code_kp_multiply; })
			.def_property_readonly_static("kp_subtract", [](py::object) { return (int32_t)key_code_kp_subtract; })
			.def_property_readonly_static("kp_add", [](py::object) { return (int32_t)key_code_kp_add; })
			.def_property_readonly_static("kp_enter", [](py::object) { return (int32_t)key_code_kp_enter; })
			.def_property_readonly_static("kp_equal", [](py::object) { return (int32_t)key_code_kp_equal; })
			.def_property_readonly_static("left_shift", [](py::object) { return (int32_t)key_code_left_shift; })
			.def_property_readonly_static("left_control", [](py::object) { return (int32_t)key_code_left_control; })
			.def_property_readonly_static("left_alt", [](py::object) { return (int32_t)key_code_left_alt; })
			.def_property_readonly_static("left_super", [](py::object) { return (int32_t)key_code_left_super; })
			.def_property_readonly_static("right_shift", [](py::object) { return (int32_t)key_code_right_shift; })
			.def_property_readonly_static("right_control", [](py::object) { return (int32_t)key_code_right_control; })
			.def_property_readonly_static("right_alt", [](py::object) { return (int32_t)key_code_right_alt; })
			.def_property_readonly_static("right_super", [](py::object) { return (int32_t)key_code_right_super; })
			.def_property_readonly_static("menu", [](py::object) { return (int32_t)key_code_menu; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// WINDOW ATTRIBUTES
		py::class_<window_attr_>(m, "window_attr")
			.def(py::init<>())

			.def_property_readonly_static("focused", [](py::object) { return (int32_t)window_attr_focused; })
			.def_property_readonly_static("iconified", [](py::object) { return (int32_t)window_attr_iconified; })
			.def_property_readonly_static("resizable", [](py::object) { return (int32_t)window_attr_resizable; })
			.def_property_readonly_static("visible", [](py::object) { return (int32_t)window_attr_visible; })
			.def_property_readonly_static("decorated", [](py::object) { return (int32_t)window_attr_decorated; })
			.def_property_readonly_static("auto_iconify", [](py::object) { return (int32_t)window_attr_auto_iconify; })
			.def_property_readonly_static("floating", [](py::object) { return (int32_t)window_attr_floating; })
			.def_property_readonly_static("maximized", [](py::object) { return (int32_t)window_attr_maximized; })
			.def_property_readonly_static("center_cursor", [](py::object) { return (int32_t)window_attr_center_cursor; })
			.def_property_readonly_static("transparent_framebuffer", [](py::object) { return (int32_t)window_attr_transparent_framebuffer; })
			.def_property_readonly_static("hovered", [](py::object) { return (int32_t)window_attr_hovered; })
			.def_property_readonly_static("focus_on_show", [](py::object) { return (int32_t)window_attr_focus_on_show; })

			.def_property_readonly_static("red_bits", [](py::object) { return (int32_t)window_attr_red_bits; })
			.def_property_readonly_static("green_bits", [](py::object) { return (int32_t)window_attr_green_bits; })
			.def_property_readonly_static("blue_bits", [](py::object) { return (int32_t)window_attr_blue_bits; })
			.def_property_readonly_static("alpha_bits", [](py::object) { return (int32_t)window_attr_alpha_bits; })
			.def_property_readonly_static("depth_bits", [](py::object) { return (int32_t)window_attr_depth_bits; })
			.def_property_readonly_static("stencil_bits", [](py::object) { return (int32_t)window_attr_stencil_bits; })
			.def_property_readonly_static("accum_red_bits", [](py::object) { return (int32_t)window_attr_accum_red_bits; })
			.def_property_readonly_static("accum_green_bits", [](py::object) { return (int32_t)window_attr_accum_green_bits; })
			.def_property_readonly_static("accum_blue_bits", [](py::object) { return (int32_t)window_attr_accum_blue_bits; })
			.def_property_readonly_static("accum_alpha_bits", [](py::object) { return (int32_t)window_attr_accum_alpha_bits; })
			.def_property_readonly_static("aux_buffers", [](py::object) { return (int32_t)window_attr_aux_buffers; })
			.def_property_readonly_static("stereo", [](py::object) { return (int32_t)window_attr_stereo; })
			.def_property_readonly_static("samples", [](py::object) { return (int32_t)window_attr_samples; })
			.def_property_readonly_static("srgb_capable", [](py::object) { return (int32_t)window_attr_srgb_capable; })
			.def_property_readonly_static("refresh_rate", [](py::object) { return (int32_t)window_attr_refresh_rate; })
			.def_property_readonly_static("doublebuffer", [](py::object) { return (int32_t)window_attr_doublebuffer; })

			.def_property_readonly_static("client_api", [](py::object) { return (int32_t)window_attr_client_api; })
			.def_property_readonly_static("context_version_major", [](py::object) { return (int32_t)window_attr_context_version_major; })
			.def_property_readonly_static("context_version_minor", [](py::object) { return (int32_t)window_attr_context_version_minor; })
			.def_property_readonly_static("context_revision", [](py::object) { return (int32_t)window_attr_context_revision; })
			.def_property_readonly_static("context_robustness", [](py::object) { return (int32_t)window_attr_context_robustness; })
			.def_property_readonly_static("context_forward_compat", [](py::object) { return (int32_t)window_attr_backend_forward_compat; })
			.def_property_readonly_static("context_debug_context", [](py::object) { return (int32_t)window_attr_backend_debug_context; })
			.def_property_readonly_static("renderer_profile", [](py::object) { return (int32_t)window_attr_backend_profile; })
			.def_property_readonly_static("context_release_behavior", [](py::object) { return (int32_t)window_attr_context_release_behavior; })
			.def_property_readonly_static("context_no_error", [](py::object) { return (int32_t)window_attr_context_no_error; })
			.def_property_readonly_static("context_creation_api", [](py::object) { return (int32_t)window_attr_context_creation_api; })
			.def_property_readonly_static("scale_to_monitor", [](py::object) { return (int32_t)window_attr_scale_to_monitor; })
			;

		// WINDOW CLIENT
		py::class_<window_client_>(m, "window_client")
			.def(py::init<>())
			.def_property_readonly_static("unknown", [](py::object) { return (int32_t)window_client_unknown; })
			.def_property_readonly_static("opengl", [](py::object) { return (int32_t)window_client_opengl; })
			.def_property_readonly_static("vulkan", [](py::object) { return (int32_t)window_client_vulkan; })
			.def_property_readonly_static("directx", [](py::object) { return (int32_t)window_client_directx; })
			;

		// WINDOW PROFILE
		py::class_<window_profile_>(m, "window_profile")
			.def(py::init<>())
			.def_property_readonly_static("any", [](py::object) { return (int32_t)window_profile_any; })
			.def_property_readonly_static("core", [](py::object) { return (int32_t)window_profile_core; })
			.def_property_readonly_static("compat", [](py::object) { return (int32_t)window_profile_compat; })
			.def_property_readonly_static("debug", [](py::object) { return (int32_t)window_profile_debug; })
			;

		// WINDOW HINTS
		py::class_<window_hints_>(m, "window_hints")
			.def(py::init<>())
			.def_property_readonly_static("none", [](py::object) { return (int32_t)window_hints_none; })
			.def_property_readonly_static("resizable", [](py::object) { return (int32_t)window_hints_resizable; })
			.def_property_readonly_static("visible", [](py::object) { return (int32_t)window_hints_visible; })
			.def_property_readonly_static("decorated", [](py::object) { return (int32_t)window_hints_decorated; })
			.def_property_readonly_static("focused", [](py::object) { return (int32_t)window_hints_focused; })
			.def_property_readonly_static("auto_iconify", [](py::object) { return (int32_t)window_hints_auto_iconify; })
			.def_property_readonly_static("floating", [](py::object) { return (int32_t)window_hints_floating; })
			.def_property_readonly_static("maximized", [](py::object) { return (int32_t)window_hints_maximized; })
			.def_property_readonly_static("doublebuffer", [](py::object) { return (int32_t)window_hints_doublebuffer; })
			.def_property_readonly_static("default", [](py::object) { return (int32_t)window_hints_default; })
			.def_property_readonly_static("default_maximized", [](py::object) { return (int32_t)window_hints_default_maximized; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// CONTEXT SETTINGS
		py::class_<context_settings>(m, "context_settings")
			.def(py::init<>())
			.def(py::init<int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, bool, bool>())
			.def_readwrite("api", &context_settings::api)
			.def_readwrite("major", &context_settings::major)
			.def_readwrite("minor", &context_settings::minor)
			.def_readwrite("profile", &context_settings::profile)
			.def_readwrite("depth_bits", &context_settings::depth_bits)
			.def_readwrite("stencil_bits", &context_settings::stencil_bits)
			.def_readwrite("multisample", &context_settings::multisample)
			.def_readwrite("srgb_capable", &context_settings::srgb_capable)
			;

		// VIDEO MODE
		py::class_<video_mode>(m, "video_mode")
			.def(py::init<>())
			.def(py::init<vec2i const &>())
			.def(py::init<vec2i const &, uint32_t>())
			.def_readwrite("size", &video_mode::size)
			.def_readwrite("depth", &video_mode::depth)
			.def_static("get_desktop_mode", &video_mode::get_desktop_mode)
			.def_static("get_fullscreen_modes", &video_mode::get_fullscreen_modes)
			;

		// WINDOW SETTINGS
		py::class_<window_settings>(m, "window_settings")
			.def(py::init<>())
			.def(py::init<pmr::string const &, video_mode const &, context_settings const &, int32_t>())
			.def_readwrite("title", &window_settings::title)
			.def_readwrite("video", &window_settings::video)
			.def_readwrite("context", &window_settings::context)
			.def_readwrite("hints", &window_settings::hints)
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SETUP
		([&m, sys = py::module::import("sys")
		]()
		{
			sys.attr("stdout") = m.attr("stdio").attr("cout");
			sys.attr("stderr") = m.attr("stdio").attr("cout");
			sys.attr("stdin") = py::none{};
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// LIBMEME_ENGINE (systems)
	PYBIND11_EMBEDDED_MODULE(libmeme_engine, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// FS
		struct ml_engine_fs {};
		py::class_<ml_engine_fs>(m, "fs")
			.def(py::init<>())
			.def_static("path2", [](cstring s) { return engine::fs().path2(s).string(); })
			.def_property_readonly_static("program_path", [](py::object) { return engine::fs().program_path(); })
			.def_property_readonly_static("content_home", [](py::object) { return engine::fs().content_home(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// GUI
		struct ml_engine_gui {};
		py::class_<ml_engine_gui>(m, "gui")
			.def(py::init<>())
			.def_static("initialize", [](cstring s) { return engine::gui().initialize(engine::window(), s); })
			.def_static("load_style", [](cstring s) { return engine::gui().load_style(s); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// PLUGINS
		struct ml_engine_plugins {};
		py::class_<ml_engine_plugins>(m, "plugins")
			.def(py::init<>())
			.def_static("clear", []() { engine::plugins().clear(); })
			.def_static("free", [](cstring s) { return engine::plugins().free(s); })
			.def_static("load", [](cstring s) { return engine::plugins().load(s); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SCRIPTS
		struct ml_engine_scripts {};
		py::class_<ml_engine_scripts>(m, "scripts")
			.def(py::init<>())
			.def_static("do_file", [](cstring s) { return engine::scripts().do_file(s); })
			.def_static("do_string", [](cstring s) { return engine::scripts().do_string(s); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// TIME
		struct ml_engine_time {};
		py::class_<ml_engine_time>(m, "time")
			.def(py::init<>())
			.def_property_readonly_static("total_time", [](py::object) { return engine::time().total_time().count(); })
			.def_property_readonly_static("delta_time", [](py::object) { return engine::time().delta_time().count(); })
			.def_property_readonly_static("frame_count", [](py::object) { return engine::time().frame_count(); })
			.def_property_readonly_static("frame_rate", [](py::object) { return engine::time().frame_rate(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// WINDOW
		struct ml_engine_window {};
		py::class_<ml_engine_window>(m, "window")
			.def(py::init<>())
			.def_static("open"					, [](window_settings const & ws, bool ic) { return engine::window().open(ws, ic); })
			.def_static("destroy"				, []() { engine::window().destroy(); })
			.def_static("iconify"				, []() { engine::window().iconify(); })
			.def_static("maximize"				, []() { engine::window().maximize(); })
			.def_static("restore"				, []() { engine::window().restore(); })
			.def_static("swap_buffers"			, []() { engine::window().swap_buffers(); })

			.def_static("is_fullscreen"			, []() { return engine::window().is_fullscreen(); })
			.def_static("is_open"				, []() { return engine::window().is_open(); })
			.def_static("get_attribute"			, [](int32_t v) { return engine::window().get_attribute(v); })
			.def_static("get_bounds"			, []() { return (vec4i)engine::window().get_bounds(); })
			.def_static("get_clipboard_string"	, []() { return engine::window().get_clipboard_string(); })
			.def_static("get_content_scale"		, []() { return engine::window().get_content_scale(); })
			.def_static("get_cursor_position"	, []() { return engine::window().get_cursor_position(); })
			.def_static("get_framebuffer_size"	, []() { return engine::window().get_framebuffer_size(); })
			.def_static("get_handle"			, []() { return engine::window().get_handle(); })
			.def_static("get_input_mode"		, [](int32_t v) { return engine::window().get_input_mode(v); })
			.def_static("get_key"				, [](int32_t v) { return engine::window().get_key(v); })
			.def_static("get_mouse_button"		, [](int32_t v) { return engine::window().get_mouse_button(v); })
			.def_static("get_native_handle"		, []() { return engine::window().get_native_handle(); })
			.def_static("get_opacity"			, []() { return engine::window().get_opacity(); })
			.def_static("get_position"			, []() { return engine::window().get_position(); })

			.def_static("set_clipboard_string"	, [](cstring v) { engine::window().set_clipboard_string(v); })
			.def_static("set_cursor"			, [](cursor_handle v) { engine::window().set_cursor(v); })
			.def_static("set_cursor_mode"		, [](int32_t v) { engine::window().set_cursor_mode(v); })
			.def_static("set_cursor_position"	, [](vec2d v) { engine::window().set_cursor_position(v); })
			.def_static("set_fullscreen"		, [](bool v) { engine::window().set_fullscreen(v); })
			.def_static("set_icon"				, [](size_t w, size_t h, byte_t const * p) { engine::window().set_icon(w, h, p); })
			.def_static("set_input_mode"		, [](int32_t m, int32_t v) { engine::window().set_input_mode(m, v); })
			.def_static("set_opacity"			, [](float_t v) { engine::window().set_opacity(v); })
			.def_static("set_position"			, [](vec2i v) { engine::window().set_position(v); })
			.def_static("set_monitor"			, [](monitor_handle v, vec4i b) { engine::window().set_monitor(v, b); })
			.def_static("set_should_close"		, [](bool v) { engine::window().set_should_close(v); })
			.def_static("set_size"				, [](vec2i v) { engine::window().set_size(v); })
			.def_static("set_title"				, [](cstring v) { engine::window().set_title(v); })

			.def_static("destroy_cursor"		, [](cursor_handle v) { window::destroy_cursor(v); })
			.def_static("finalize"				, []() { window::finalize(); })
			.def_static("poll_events"			, []() { window::poll_events(); })
			.def_static("set_current_context"	, [](window_handle v) { window::set_current_context(v); })
			.def_static("set_swap_interval"		, [](int32_t v) { window::set_swap_interval(v); })

			.def_static("create_custom_cursor"	, [](size_t w, size_t h, byte_t const * p) { return window::create_custom_cursor(w, h, p); })
			.def_static("create_standard_cursor", [](int32_t v) { return window::create_standard_cursor(v); })
			.def_static("extension_supported"	, [](cstring v) { return window::extension_supported(v); })
			.def_static("get_current_context"	, []() { return window::get_current_context(); })
			.def_static("get_proc_address"		, [](cstring v) { return window::get_proc_address(v); })
			.def_static("get_monitors"			, []() { return window::get_monitors(); })
			.def_static("get_time"				, []() { return window::get_time(); })
			.def_static("initialize"			, []() { return window::initialize(); })

			.def_static("get_context_settings"	, []() { return engine::window().get_context_settings(); })
			.def_static("get_hint"				, [](int32_t v) { return engine::window().get_hint(v); })
			.def_static("get_hints"				, []() { return engine::window().get_hints(); })
			.def_static("get_settings"			, []() { return engine::window().get_settings(); })
			.def_static("get_title"				, []() { return engine::window().get_title(); })
			.def_static("get_video_mode"		, []() { return engine::window().get_video_mode(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SETUP
		([&m, builtins = py::module::import("builtins")
			, sys = py::module::import("sys")
		]()
		{
			m.def("exit", [](py::args) { engine::window().set_should_close(true); });
			builtins.attr("exit") = m.attr("exit");
			sys.attr("exit") = m.attr("exit");
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}