#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Core/Debug.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/ScriptObject.hpp>

// libmeme
namespace ml::embed
{
	PYBIND11_EMBEDDED_MODULE(libmeme_engine, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// ASSETS
		struct ml_engine_assets {};
		py::class_<ml_engine_assets>(m, "assets")
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// CONFIG
		struct ml_engine_config {};
		py::class_<ml_engine_config>(m, "config")
			.def_property_readonly_static("command_line", [](py::object) { return engine::config().command_line; })
			.def_property_readonly_static("content_home", [](py::object) { return engine::config().content_home.native(); })
			.def_property_readonly_static("library_home", [](py::object) { return engine::config().library_home.native(); })
			.def_property_readonly_static("program_name", [](py::object) { return engine::config().program_name.native(); })
			.def_property_readonly_static("program_path", [](py::object) { return engine::config().program_path.native(); })
			.def_property_readonly_static("setup_script", [](py::object) { return engine::config().setup_script.native(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// IO
		struct ml_engine_io {};
		py::class_<ml_engine_io>(m, "io")
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// PLUGINS
		struct ml_engine_plugins {};
		py::class_<ml_engine_plugins>(m, "plugins")
			.def_static("load", [](cstring s) { return engine::plugins().load(s); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SCRIPTS
		struct ml_engine_scripts {};
		py::class_<ml_engine_scripts>(m, "scripts")
			.def_static("do_file", [](cstring s) { return engine::scripts().do_file(s); })
			.def_static("do_string", [](cstring s) { return engine::scripts().do_string(s); })
			;

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
		
		// WINDOW
		struct ml_engine_window {};
		py::class_<ml_engine_window>(m, "window")
			.def_static("close", [](py::args) { engine::window().close(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SETUP
		m.attr("__good__") = ([&
			, builtins = py::module::import("builtins")
			, sys = py::module::import("sys")]()
		{
			builtins.attr("exit") = m.attr("window").attr("close");
			sys.attr("exit") = m.attr("window").attr("close");
			script_object::install(m);
			return true;
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}