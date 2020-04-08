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
				, [](py::object) { return engine::get_config().command_line; })
					
			.def_property_readonly_static("content_home"
				, [](py::object) { return engine::get_config().content_home.native(); })
					
			.def_property_readonly_static("library_home"
				, [](py::object) { return engine::get_config().library_home.native(); })
					
			.def_property_readonly_static("program_name"
				, [](py::object) { return engine::get_config().program_name.native(); })
					
			.def_property_readonly_static("program_path"
				, [](py::object) { return engine::get_config().program_path.native(); })
					
			.def_property_readonly_static("setup_script"
				, [](py::object) { return engine::get_config().setup_script.native(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// ENGINE IO
		struct ml_engine_io {};
		py::class_<ml_engine_io>(m, "runtime")

			.def_property_readonly_static("delta_time"
				, [](py::object) { return engine::get_io().delta_time; })
					
			.def_property_readonly_static("frame_count"
				, [](py::object) { return engine::get_io().frame_count; })
					
			.def_property_readonly_static("frame_rate"
				, [](py::object) { return engine::get_io().frame_rate; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		
		m // FUNCTIONS
			.def("close", [](py::args) { engine::get_window().close(); })
				
			.def("do_file", [](cstring s) { return engine::get_scripts().do_file(s); })

			.def("do_string", [](cstring s) { return engine::get_scripts().do_string(s); })
				
			.def("get_time", []() { return engine::get_time().count(); })
				
			.def("load_plugin", [](cstring s) { return engine::get_plugins().load(s); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// INSTALL
		([&	, builtins = py::module::import("builtins")
			, sys = py::module::import("sys")
		]()
		{
			script_object::install(m);
			builtins.attr("exit") = m.attr("close");
			sys.attr("exit") = m.attr("close");
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}