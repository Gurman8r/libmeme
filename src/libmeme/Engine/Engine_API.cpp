#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Core/Debug.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/ScriptObject.hpp>

// libmeme
namespace ml::embed
{
	PYBIND11_EMBEDDED_MODULE(libmeme, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// LIB
		struct ml_lib
		{
			static void install(py::module & m)
			{
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				py::class_<ml_lib>(m, "lib")
					.def_property_readonly_static("arch"	, [](py::object) { return ML_arch; })
					.def_property_readonly_static("author"	, [](py::object) { return ML__author; })
					.def_property_readonly_static("cc_name"	, [](py::object) { return ML_cc_name; })
					.def_property_readonly_static("cc_ver"	, [](py::object) { return ML_cc_version; })
					.def_property_readonly_static("cfg"		, [](py::object) { return ML_configuration; })
					.def_property_readonly_static("date"	, [](py::object) { return ML__date; })
					.def_property_readonly_static("is_debug", [](py::object) { return ML_is_debug; })
					.def_property_readonly_static("lang"	, [](py::object) { return ML_lang; })
					.def_property_readonly_static("name"	, [](py::object) { return ML__name; })
					.def_property_readonly_static("platform", [](py::object) { return ML_platform; })
					.def_property_readonly_static("time"	, [](py::object) { return ML__time; })
					.def_property_readonly_static("url"		, [](py::object) { return ML__url; })
					.def_property_readonly_static("version"	, [](py::object) { return ML__version; })
					;

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// STDIO
		struct ml_stdio
		{
			static void install(py::module & m)
			{
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				struct ml_output
				{
					std::reference_wrapper<std::ostream> m_os;

					ml_output(std::ostream & os = std::cout) : m_os{ std::ref(os) }
					{
					}

					int32_t fileno() const noexcept
					{
						if (auto const addr{ std::addressof(m_os.get()) }; !addr) { return -2; }
						else if (addr == std::addressof(std::cout)) { return 1; }
						else if (addr == std::addressof(std::cerr)) { return 2; }
						else return 0;
					}

					void flush() { m_os.get().flush(); }

					void write(py::object o) { m_os << o; }

					void writelines(py::list l) { for (auto const & e : l) { m_os << e; } }
				};
				py::class_<ml_output>(m, "output")
					.def(py::init<>())
					.def("closed"		, []() { return false; })
					.def("isatty"		, []() { return false; })
					.def("readable"		, []() { return false; })
					.def("seekable"		, []() { return false; })
					.def("writable"		, []() { return true; })
					.def("fileno"		, &ml_output::fileno)
					.def("flush"		, &ml_output::flush)
					.def("write"		, &ml_output::write)
					.def("writelines"	, &ml_output::writelines)
					;

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				py::class_<ml_stdio>(m, "stdio")

					.def_property_readonly_static("cerr"
						, [](py::object) { return ml_output{ std::cerr }; })
					
					.def_property_readonly_static("cout"
						, [](py::object) { return ml_output{ std::cout }; })
					
					;

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// ENGINE
		struct ml_engine
		{
			static void install(py::module & m)
			{
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				struct ml_engine_config {};
				py::class_<ml_engine_config>(m, "__ml_engine_config__")

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

				struct ml_engine_io {};
				py::class_<ml_engine_io>(m, "__ml_engine_io__")

					.def_property_readonly_static("delta_time"
						, [](py::object) { return engine::get_io().delta_time; })
					
					.def_property_readonly_static("frame_count"
						, [](py::object) { return engine::get_io().frame_count; })
					
					.def_property_readonly_static("frame_rate"
						, [](py::object) { return engine::get_io().frame_rate; })
					;

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				py::class_<ml_engine>(m, "engine")

					.def_property_readonly_static("config"
						, [](py::object) { return ml_engine_config{}; })

					.def_property_readonly_static("io"
						, [](py::object) { return ml_engine_io{}; })

					.def_static("do_string"
						, [](int32_t l, cstring s) { return engine::do_string(l, s); })
					
					.def_static("do_file"
						, [](cstring s) { return engine::do_file(s); })
					
					.def_static("exit"
						, [](py::args) { engine::get_window().close(); })
					
					.def_static("get_time"
						, []() { return engine::get_time().count(); })
					
					.def_static("load_plugin"
						, [](cstring s) { return engine::load_plugin(s); })
					;

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// INSTALL
		([&	, builtins = py::module::import("builtins")
			, sys = py::module::import("sys")
		]()
		{
			ml_lib::install(m);
			ml_stdio::install(m);
			ml_engine::install(m);
			script_object::install(m);

			builtins.attr("exit")	= m.attr("engine").attr("exit");
			sys.attr("exit")		= m.attr("engine").attr("exit");
			sys.attr("stdout")		= m.attr("stdio").attr("cout");
			sys.attr("stderr")		= m.attr("stdio").attr("cout");
			sys.attr("stdin")		= py::none{};
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}