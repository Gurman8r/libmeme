#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Core/Debug.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/ScriptObject.hpp>

namespace ml::embed
{
	// CONFIG
	PYBIND11_EMBEDDED_MODULE(libmeme_config, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// PROJECT
		struct ml_project {};
		py::class_<ml_project>(m, "project")
			.def_property_readonly_static("author"	, [](py::object) { return ML__author; })
			.def_property_readonly_static("date"	, [](py::object) { return ML__date; })
			.def_property_readonly_static("name"	, [](py::object) { return ML__name; })
			.def_property_readonly_static("time"	, [](py::object) { return ML__time; })
			.def_property_readonly_static("url"		, [](py::object) { return ML__url; })
			.def_property_readonly_static("version"	, [](py::object) { return ML__version; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// BUILD
		struct ml_build {};
		py::class_<ml_build>(m, "build")
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
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	// CORE
	PYBIND11_EMBEDDED_MODULE(libmeme_core, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// OUTPUT REDIRECT
		struct ml_output_facade
		{
			std::reference_wrapper<std::ostream> m_os;

			ml_output_facade(std::ostream & os = std::cout) noexcept : m_os{ os }
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
		py::class_<ml_output_facade>(m, "output_facade")
			.def(py::init<>())
			.def("closed"		, []() { return false; })
			.def("isatty"		, []() { return false; })
			.def("readable"		, []() { return false; })
			.def("seekable"		, []() { return false; })
			.def("writable"		, []() { return true; })
			.def("fileno"		, &ml_output_facade::fileno)
			.def("flush"		, &ml_output_facade::flush)
			.def("write"		, &ml_output_facade::write)
			.def("writelines"	, &ml_output_facade::writelines)
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// STDIO
		struct ml_stdio
		{
			static auto & err(py::object) noexcept
			{
				static ml_output_facade temp{ std::cerr }; return temp;
			}

			static auto & out(py::object) noexcept
			{
				static ml_output_facade temp{ std::cout }; return temp;
			}
		};
		py::class_<ml_stdio>(m, "stdio")
			.def_property_readonly_static("cerr", &ml_stdio::err)
			.def_property_readonly_static("cout", &ml_stdio::out)
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SETUP
		([&m, sys = py::module::import("sys")]()
		{
			sys.attr("stdout") = m.attr("stdio").attr("cout");
			sys.attr("stderr") = m.attr("stdio").attr("cout");
			sys.attr("stdin") = py::none{};
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	// PLATFORM
	PYBIND11_EMBEDDED_MODULE(libmeme_platform, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// CLIENT API
		py::class_<client_api>(m, "client_api")
			.def_property_readonly_static("unknown"	, [](py::object) { return (int32_t)client_api::unknown; })
			.def_property_readonly_static("opengl"	, [](py::object) { return (int32_t)client_api::opengl; })
			.def_property_readonly_static("vulkan"	, [](py::object) { return (int32_t)client_api::vulkan; })
			.def_property_readonly_static("directx"	, [](py::object) { return (int32_t)client_api::directx; })
			.def_property_readonly_static("any"		, [](py::object) { return (int32_t)client_api::any; })
			.def_property_readonly_static("core"	, [](py::object) { return (int32_t)client_api::core; })
			.def_property_readonly_static("compat"	, [](py::object) { return (int32_t)client_api::compat; })
			.def_property_readonly_static("debug"	, [](py::object) { return (int32_t)client_api::debug; })
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
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	// ENGINE
	PYBIND11_EMBEDDED_MODULE(libmeme_engine, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// ASSETS
		struct ml_engine_assets {};
		py::class_<ml_engine_assets>(m, "assets")
			.def_static("load", [](cstring s) { return false; })
			.def_static("free", [](cstring s) { return false; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// CONFIG
		struct ml_engine_config {};
		py::class_<ml_engine_config>(m, "config")
			.def_property_readonly_static("command_line", [](py::object) { return engine::config().command_line; })
			.def_property_readonly_static("content_home", [](py::object) { return engine::config().content_home.native(); })
			.def_property_readonly_static("library_home", [](py::object) { return engine::config().library_home.native(); })
			.def_property_readonly_static("program_name", [](py::object) { return engine::config().program_name.native(); })
			.def_property_readonly_static("program_path", [](py::object) { return engine::config().program_path.native(); })
			.def_property_readonly_static("startup_script", [](py::object) { return engine::config().startup_script.native(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// IO
		struct ml_engine_io {};
		py::class_<ml_engine_io>(m, "io")
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// PLUGINS
		struct ml_engine_plugins {};
		py::class_<ml_engine_plugins>(m, "plugins")
			.def_static("load", [](cstring s) { return engine::plugins().load(s); })
			.def_static("free", [](cstring s) { return false; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SCRIPTS
		struct ml_engine_scripts {};
		py::class_<ml_engine_scripts>(m, "scripts")
			.def_static("do_file", [](cstring s) { return engine::scripts().do_file(s); })
			.def_static("do_string", [](cstring s) { return engine::scripts().do_string(s); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// TIME
		struct ml_engine_time {};
		py::class_<ml_engine_time>(m, "time")
			.def_property_readonly_static("total", [](py::object) { return engine::time().total().count(); })
			.def_property_readonly_static("delta", [](py::object) { return engine::time().delta().count(); })
			.def_property_readonly_static("frame_count", [](py::object) { return engine::time().frame_count(); })
			.def_property_readonly_static("frame_rate", [](py::object) { return engine::time().frame_rate(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// WINDOW
		struct ml_engine_window {};
		py::class_<ml_engine_window>(m, "window")
			.def(py::init<>())
			.def_static("create", [](window_settings const & ws, bool ic) { return engine::window().create(ws, ic); })
			.def_static("close", [](py::args) { engine::window().close(); })
			.def_static("get_size", []() { return engine::window().get_size(); })
			.def_static("get_settings", []() { return engine::window().get_settings(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SETUP
		([&m, builtins = py::module::import("builtins")
			, sys = py::module::import("sys")]()
		{
			builtins.attr("exit") = m.attr("window").attr("close");
			sys.attr("exit") = m.attr("window").attr("close");
			script_object::install(m);
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}