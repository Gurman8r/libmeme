#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Core/Debug.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/Embed.hpp>

namespace ml::embed
{
	PYBIND11_EMBEDDED_MODULE(libmeme_core, m)
	{
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

		// BUILD
		struct ml_build {};
		py::class_<ml_build>(m, "build")
			.def_property_readonly_static("arch"		, [](py::object) { return ML_arch; })
			.def_property_readonly_static("cc"			, [](py::object) { return ML_cc_name; })
			.def_property_readonly_static("cc_ver"		, [](py::object) { return ML_cc_version; })
			.def_property_readonly_static("config"		, [](py::object) { return ML_configuration; })
			.def_property_readonly_static("is_debug"	, [](py::object) { return ML_is_debug; })
			.def_property_readonly_static("lang"		, [](py::object) { return ML_lang; })
			.def_property_readonly_static("platform"	, [](py::object) { return ML_platform; })
			.def_property_readonly_static("system"		, [](py::object) { return ML_os_name; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// OUTPUT
		struct ml_output
		{
			std::reference_wrapper<std::ostream> m_os;

			ml_output(std::ostream & os = std::cout) : m_os{ std::ref(os) } {}

			int32_t fileno() const noexcept
			{
				if (auto const addr{ std::addressof(m_os.get()) }; !addr) { return -2; }
				else if (addr == std::addressof(std::cout)) { return 1; }
				else if (addr == std::addressof(std::cerr)) { return 2; }
				else return -1;
			}

			void flush() { m_os.get().flush(); }

			void write(py::object o) { m_os << o; }

			void writelines(py::list l) { for (auto const & e : l) { m_os << e; } }
		};
		py::class_<ml_output>(m, "output_handler")
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

		// STDIO
		struct ml_stdio
		{
			static auto & err(py::object) noexcept
			{
				static ml_output temp{ std::cerr }; return temp;
			}

			static auto & out(py::object) noexcept
			{
				static ml_output temp{ std::cout }; return temp;
			}
		};
		py::class_<ml_stdio>(m, "stdio")
			.def_property_readonly_static("cerr", &ml_stdio::err)
			.def_property_readonly_static("cout", &ml_stdio::out)
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SETUP
		m.attr("__good__") = ([&
			, sys = py::module::import("sys")]()
		{
			sys.attr("stdout") = m.attr("stdio").attr("cout");
			sys.attr("stderr") = m.attr("stdio").attr("cout");
			sys.attr("stdin") = py::none{};
			return true;
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}