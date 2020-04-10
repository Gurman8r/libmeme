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
		
		// ENGINE CONFIG
		struct ml_engine_config {};
		py::class_<ml_engine_config>(m, "config")

			.def_property_readonly_static("command_line"
				, [](py::object) { return engine::config().command_line; })
					
			.def_property_readonly_static("content_home"
				, [](py::object) { return engine::config().content_home.native(); })
					
			.def_property_readonly_static("library_home"
				, [](py::object) { return engine::config().library_home.native(); })
					
			.def_property_readonly_static("program_name"
				, [](py::object) { return engine::config().program_name.native(); })
					
			.def_property_readonly_static("program_path"
				, [](py::object) { return engine::config().program_path.native(); })
					
			.def_property_readonly_static("setup_script"
				, [](py::object) { return engine::config().setup_script.native(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// ENGINE IO
		struct ml_engine_io {};
		py::class_<ml_engine_io>(m, "io")

			.def_property_readonly_static("delta_time"
				, [](py::object) { return engine::io().delta_time; })
					
			.def_property_readonly_static("frame_count"
				, [](py::object) { return engine::io().frame_count; })
					
			.def_property_readonly_static("frame_rate"
				, [](py::object) { return engine::io().frame_rate; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// ASSET MANAGER
		struct ml_asset_manager {};
		py::class_<ml_asset_manager>(m, "assets")
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// PLUGIN MANAGER
		struct ml_plugin_manager {};
		py::class_<ml_plugin_manager>(m, "plugins")
			.def_static("load", [](cstring s) { return engine::plugins().load(s); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SCRIPT MANAGER
		struct ml_script_manager {};
		py::class_<ml_script_manager>(m, "scripts")
			.def_static("do_file", [](cstring s) { return engine::scripts().do_file(s); })
			.def_static("do_string", [](cstring s) { return engine::scripts().do_string(s); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		m // FUNCTIONS
			.def("close", [](py::args) { engine::window().close(); })
			.def("time", []() { return engine::time().total().count(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SETUP
		m.attr("__good__") = ([&
			, builtins = py::module::import("builtins")
			, sys = py::module::import("sys")]()
		{
			builtins.attr("exit") = m.attr("close");
			sys.attr("exit") = m.attr("close");
			script_object::install(m);
			return true;
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}