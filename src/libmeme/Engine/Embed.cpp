#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Core/Debug.hpp>

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

		// FILESYSTEM
		struct ml_fs {};
		py::class_<ml_fs>(m, "fs")
			.def(py::init<>())
			.def_static("path_to", [](cstring s) { return engine::path_to(s).string(); });
		;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// PLATFORM API
		py::class_<platform_api>(m, "platform")
			.def(py::init<>())
			.def_property_readonly_static("unknown"	, [](py::object) { return (int32_t)platform_api::unknown; })
			.def_property_readonly_static("opengl"	, [](py::object) { return (int32_t)platform_api::opengl; })
			.def_property_readonly_static("vulkan"	, [](py::object) { return (int32_t)platform_api::vulkan; })
			.def_property_readonly_static("directx"	, [](py::object) { return (int32_t)platform_api::directx; })
			.def_property_readonly_static("any"		, [](py::object) { return (int32_t)platform_api::any; })
			.def_property_readonly_static("core"	, [](py::object) { return (int32_t)platform_api::core; })
			.def_property_readonly_static("compat"	, [](py::object) { return (int32_t)platform_api::compat; })
			.def_property_readonly_static("debug"	, [](py::object) { return (int32_t)platform_api::debug; })
			;

		// CONTEXT SETTINGS
		py::class_<context_settings>(m, "context_settings")
			.def(py::init<>())
			.def(py::init<int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, bool, bool>())
			.def("__nonzero__"				, &context_settings::operator bool, py::is_operator())
			.def("__bool__"					, &context_settings::operator bool, py::is_operator())
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
		py::class_<display_settings>(m, "display_settings")
			.def(py::init<>())
			.def(py::init<vec2i, uint32_t>())
			.def("__nonzero__"		, &display_settings::operator bool, py::is_operator())
			.def("__bool__"			, &display_settings::operator bool, py::is_operator())
			.def_readwrite("size"	, &display_settings::size)
			.def_readwrite("depth"	, &display_settings::depth)
			;

		// WINDOW SETTINGS
		py::class_<window_settings>(m, "window_settings")
			.def(py::init<>())
			.def(py::init<pmr::string const &, display_settings const &, context_settings const &, int32_t>())
			.def_readwrite("title"	, &window_settings::title)
			.def_readwrite("display", &window_settings::display)
			.def_readwrite("context", &window_settings::context)
			.def_readwrite("hints"	, &window_settings::hints)
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SETUP
		([&m, sys = py::module::import("sys")
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

		// ASSETS
		struct ml_engine_assets {};
		py::class_<ml_engine_assets>(m, "assets")
			.def(py::init<>())
			.def_static("load", [](py::args) { return false; })
			.def_static("free", [](py::args) { return false; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// CONFIG
		struct ml_engine_config {};
		py::class_<ml_engine_config>(m, "config")
			.def(py::init<>())
			.def_property_readonly_static("command_line", [](py::object) { return engine::config().command_line; })
			.def_property_readonly_static("content_home", [](py::object) { return engine::config().content_home.native(); })
			.def_property_readonly_static("library_home", [](py::object) { return engine::config().library_home.native(); })
			.def_property_readonly_static("program_name", [](py::object) { return engine::config().program_name.native(); })
			.def_property_readonly_static("program_path", [](py::object) { return engine::config().program_path.native(); })
			.def_property_readonly_static("startup_script", [](py::object) { return engine::config().startup_script.native(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// GUI
		struct ml_engine_gui {};
		py::class_<ml_engine_gui>(m, "gui")
			.def(py::init<>())
			.def_static("init", [](cstring s) { return engine::gui().startup(engine::window(), s); })
			.def_static("load_style", [](cstring s) { return engine::gui().load_style(s); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// OBJECTS
		struct ml_engine_objects {};
		py::class_<ml_engine_objects>(m, "objects")
			.def(py::init<>())
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// PLUGINS
		struct ml_engine_plugins {};
		py::class_<ml_engine_plugins>(m, "plugins")
			.def(py::init<>())
			.def_static("load", [](cstring s) { return engine::plugins().load(s); })
			.def_static("free", [](cstring s) { return false; })
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
			.def_static("open", [](window_settings const & ws, bool ic) { return engine::window().open(ws, ic); })
			.def_static("close", [](py::args) { engine::window().close(); })
			.def_static("get_size", []() { return engine::window().get_size(); })
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