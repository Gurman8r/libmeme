#include <libmeme/Engine/Engine.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/ScriptObject.hpp>

namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// LIBMEME
	PYBIND11_EMBEDDED_MODULE(libmeme, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// PROJECT
		struct ml_project {};
		py::class_<ml_project>(m, "project")
			.def(py::init<>())
			.def_property_readonly_static("author"	, [](py::object) { return ML__author; })
			.def_property_readonly_static("date"	, [](py::object) { return ML__date; })
			.def_property_readonly_static("name"	, [](py::object) { return ML__name; })
			.def_property_readonly_static("time"	, [](py::object) { return ML__time; })
			.def_property_readonly_static("url"		, [](py::object) { return ML__url; })
			.def_property_readonly_static("version"	, [](py::object) { return ML__version; })
			;

		// BUILD
		struct ml_build {};
		py::class_<ml_build>(m, "build")
			.def(py::init<>())
			.def_property_readonly_static("arch",			[](py::object) { return ML_arch; })
			.def_property_readonly_static("compiler",		[](py::object) { return ML_cc_name; })
			.def_property_readonly_static("compiler_ver",	[](py::object) { return ML_cc_version; })
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
		py::class_<ml_stdio::output>(m, "stdio_output_facade")
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

		// PLATFORM API
		py::class_<window_api>(m, "window_api")
			.def(py::init<>())
			.def_property_readonly_static("unknown"	, [](py::object) { return (int32_t)window_api::unknown; })
			.def_property_readonly_static("opengl"	, [](py::object) { return (int32_t)window_api::opengl; })
			.def_property_readonly_static("vulkan"	, [](py::object) { return (int32_t)window_api::vulkan; })
			.def_property_readonly_static("directx"	, [](py::object) { return (int32_t)window_api::directx; })
			.def_property_readonly_static("any"		, [](py::object) { return (int32_t)window_api::any; })
			.def_property_readonly_static("core"	, [](py::object) { return (int32_t)window_api::core; })
			.def_property_readonly_static("compat"	, [](py::object) { return (int32_t)window_api::compat; })
			.def_property_readonly_static("debug"	, [](py::object) { return (int32_t)window_api::debug; })
			;

		py::class_<window_hints_>(m, "window_hints")
			.def(py::init<>())
			.def_property_readonly_static("none"				, [](py::object) { return (int32_t)window_hints_none; })
			.def_property_readonly_static("resizable"			, [](py::object) { return (int32_t)window_hints_resizable; })
			.def_property_readonly_static("visible"				, [](py::object) { return (int32_t)window_hints_visible; })
			.def_property_readonly_static("decorated"			, [](py::object) { return (int32_t)window_hints_decorated; })
			.def_property_readonly_static("focused"				, [](py::object) { return (int32_t)window_hints_focused; })
			.def_property_readonly_static("auto_iconify"		, [](py::object) { return (int32_t)window_hints_auto_iconify; })
			.def_property_readonly_static("floating"			, [](py::object) { return (int32_t)window_hints_floating; })
			.def_property_readonly_static("maximized"			, [](py::object) { return (int32_t)window_hints_maximized; })
			.def_property_readonly_static("fullscreen"			, [](py::object) { return (int32_t)window_hints_fullscreen; })
			.def_property_readonly_static("double_buffered"		, [](py::object) { return (int32_t)window_hints_double_buffered; })
			.def_property_readonly_static("install_callbacks"	, [](py::object) { return (int32_t)window_hints_install_callbacks; })
			.def_property_readonly_static("default"				, [](py::object) { return (int32_t)window_hints_default; })
			.def_property_readonly_static("default_max"			, [](py::object) { return (int32_t)window_hints_default_max; })
			;

		// CONTEXT SETTINGS
		py::class_<context_settings>(m, "context_settings")
			.def(py::init<>())
			.def(py::init<int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, bool, bool>())
			.def_readwrite("api"			, &context_settings::api)
			.def_readwrite("major"			, &context_settings::major)
			.def_readwrite("minor"			, &context_settings::minor)
			.def_readwrite("profile"		, &context_settings::profile)
			.def_readwrite("depth_bits"		, &context_settings::depth_bits)
			.def_readwrite("stencil_bits"	, &context_settings::stencil_bits)
			.def_readwrite("multisample"	, &context_settings::multisample)
			.def_readwrite("srgb_capable"	, &context_settings::srgb_capable)
			;

		// DISPLAY SETTINGS
		py::class_<video_mode>(m, "video_mode")
			.def(py::init<>())
			.def(py::init<vec2i, uint32_t>())
			.def_readwrite("size"	, &video_mode::size)
			.def_readwrite("depth"	, &video_mode::depth)
			;

		// WINDOW SETTINGS
		py::class_<window_settings>(m, "window_settings")
			.def(py::init<>())
			.def(py::init<pmr::string const &, video_mode const &, context_settings const &, int32_t>())
			.def_readwrite("title"	, &window_settings::title)
			.def_readwrite("video"	, &window_settings::video)
			.def_readwrite("context", &window_settings::context)
			.def_readwrite("hints"	, &window_settings::hints)
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SETUP
		([&m, builtins = py::module::import("builtins")
			, sys = py::module::import("sys")
		]()
		{
			sys.attr("stdout") = m.attr("stdio").attr("cout");
			sys.attr("stderr") = m.attr("stdio").attr("cout");
			sys.attr("stdin") = py::none{};
			script_object::install(m);
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// LIBMEME_ENGINE
	PYBIND11_EMBEDDED_MODULE(libmeme_engine, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// FILES
		struct ml_engine_files {};
		py::class_<ml_engine_files>(m, "fs")
			.def(py::init<>())
			.def_static("path_to", [](cstring s) { return engine::fs().path_to(s).string(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// GUI
		struct ml_engine_gui {};
		py::class_<ml_engine_gui>(m, "gui")
			.def(py::init<>())
			.def_static("startup", [](cstring s) { return engine::gui().startup(engine::window(), s); })
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
			.def_property_readonly_static("total", [](py::object) { return engine::time().total().count(); })
			.def_property_readonly_static("delta", [](py::object) { return engine::time().delta().count(); })
			.def_property_readonly_static("frame_count", [](py::object) { return engine::time().frame_count(); })
			.def_property_readonly_static("frame_rate", [](py::object) { return engine::time().frame_rate(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// WINDOW
		struct ml_engine_window {};
		py::class_<ml_engine_window>(m, "window")
			.def(py::init<>())
			.def_static("open", [](window_settings const & ws) { return engine::window().create(ws); })
			.def_static("close", [](py::args) { return engine::window().close(); })
			.def_static("get_size", []() { return engine::window().get_frame_size(); })
			.def_static("get_settings", []() { return engine::window().get_settings(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SETUP
		([&m, builtins = py::module::import("builtins")
			, sys = py::module::import("sys")
		]()
		{
			builtins.attr("exit") = m.attr("window").attr("close");
			sys.attr("exit") = m.attr("window").attr("close");
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}