#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Core/Debug.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Engine/Embed.hpp>

namespace ml::embed
{
	PYBIND11_EMBEDDED_MODULE(LIBMEME_ENGINE, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// PROJECT ATTRIBS
		m.attr("arch")			= ML_ARCH;
		m.attr("cc_name")		= ML_CC_NAME;
		m.attr("cc_ver")		= ML_CC_VER;
		m.attr("configuration")	= ML_CONFIGURATION;
		m.attr("debug")			= ML_DEBUG;
		m.attr("lang")			= ML_LANG;
		m.attr("lib_author")	= ML__AUTHOR;
		m.attr("lib_date")		= ML__DATE;
		m.attr("lib_name")		= ML__NAME;
		m.attr("lib_time")		= ML__TIME;
		m.attr("lib_url")		= ML__URL;
		m.attr("lib_version")	= ML__VERSION;
		m.attr("platform")		= ML_PLATFORM;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// STDIO
		class output_wrapper
		{
			std::ostream & m_os; int32_t m_no{ -1 };
		public:
			explicit output_wrapper(std::ostream & os = std::cout) : m_os{ os }
			{
				auto const addr{ std::addressof(m_os) };
				if (addr == std::addressof(std::cout)) { m_no = 1; }
				else if (addr == std::addressof(std::cerr)) { m_no = 2; }
			}

			inline auto fileno() const noexcept { return m_no; }
			
			inline void flush() { m_os.flush(); }
			
			inline void write(py::object const & o) { m_os << o; }

			inline void writelines(py::list const & l) { for (auto const & e : l) m_os << e; }
		};
		py::class_<output_wrapper>(m, "output_wrapper")
			.def(py::init<>())
			.def("closed"		, []() { return false; })
			.def("isatty"		, []() { return false; })
			.def("readable"		, []() { return false; })
			.def("seekable"		, []() { return false; })
			.def("writable"		, []() { return true; })
			.def("fileno"		, &output_wrapper::fileno)
			.def("flush"		, &output_wrapper::flush)
			.def("write"		, &output_wrapper::write)
			.def("writelines"	, &output_wrapper::writelines)
			;
		m.attr("cout") = output_wrapper{ std::cout };
		m.attr("cerr") = output_wrapper{ std::cerr };
		m.attr("cin") = py::none{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// CONFIG ATTRIBS
		m.attr("args")			= engine::get_config().arguments;
		m.attr("content_home")	= engine::get_config().content_home.native();
		m.attr("library_home")	= engine::get_config().library_home.native();
		m.attr("program_name")	= engine::get_config().program_name.native();
		m.attr("program_path")	= engine::get_config().program_path.native();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		m /* FUNCTIONS */
			.def("close"		, [](int32_t i = 0) { return engine::close(); })
			.def("do_string"	, [](int32_t l, cstring s) { return engine::do_script(l, s); })
			.def("do_file"		, [](cstring s) { return engine::do_script(s); })
			.def("get_time"		, []() { return engine::get_time().count(); })
			.def("is_running"	, []() { return engine::is_running(); })
			.def("load_plugin"	, [](cstring s) { return engine::load_plugin(s); })
			.def("path_to"		, [](cstring s) { return engine::path_to(s).native(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}