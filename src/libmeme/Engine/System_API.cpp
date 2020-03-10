#include <libmeme/Engine/Engine.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Engine/Embed.hpp>

namespace ml::embed
{
	PYBIND11_EMBEDDED_MODULE(LIBMEME_SYSTEM, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// OUTPUT WRAPPER
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// STDIO ATTRIBS
		m.attr("cout") = output_wrapper{ std::cout };
		m.attr("cerr") = output_wrapper{ std::cerr };
		m.attr("cin") = py::none{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// PROJECT ATTRIBS
		m.attr("architecture")	= ML_ARCH;
		m.attr("build_date")	= ML__DATE;
		m.attr("build_time")	= ML__TIME;
		m.attr("compiler")		= ML_CC_NAME;
		m.attr("compiler_lang")	= ML_CC_LANG;
		m.attr("compiler_ver")	= ML_CC_VER;
		m.attr("configuration")	= ML_CONFIGURATION;
		m.attr("debug")			= ML_DEBUG;
		m.attr("lib_author")	= ML__AUTHOR;
		m.attr("lib_name")		= ML__NAME;
		m.attr("lib_url")		= ML__URL;
		m.attr("lib_ver")		= ML__VERSION;
		m.attr("platform")		= ML_PLATFORM;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// CONFIG ATTRIBS
		m.attr("arguments")		= engine::get_config().arguments;
		m.attr("content_home")	= engine::get_config().content_home.native();
		m.attr("library_home")	= engine::get_config().library_home.native();
		m.attr("program_name")	= engine::get_config().program_name.native();
		m.attr("program_path")	= engine::get_config().program_path.native();
		m.attr("setup_script")	= engine::get_config().setup_script.native();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		m /* FUNCTIONS */
			.def("exit"			, [](int32_t ec = {}) { engine::close(); })
			.def("do_string"	, [](int32_t l, cstring s) { return engine::do_script(l, s); })
			.def("do_file"		, [](cstring s) { return engine::do_script(s); })
			.def("load_plugin"	, [](cstring s) { return engine::load_plugin(s); })
			.def("path_to"		, [](cstring s) { return engine::path_to(s).native(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}