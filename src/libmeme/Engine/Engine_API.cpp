#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Core/Debug.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Engine/Embed.hpp>

namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME_CONFIG, m)
	{
		m /* LIBMEME_CONFIG */

			// project
			.def("proj_author"	, []() { return ML__AUTHOR; })
			.def("proj_date"	, []() { return ML__DATE; })
			.def("proj_name"	, []() { return ML__NAME; })
			.def("proj_time"	, []() { return ML__TIME; })
			.def("proj_url"		, []() { return ML__URL; })
			.def("proj_version"	, []() { return ML__VERSION; })

			// operating system
			.def("os"			, []() { return ML_OS_NAME; })

			// compiler
			.def("cc_lang"		, []() { return ML_LANG; })
			.def("cc_name"		, []() { return ML_CC_NAME; })
			.def("cc_ver"		, []() { return ML_CC_VER; })

			// architecture
			.def("arch"			, []() { return ML_ARCH; })
			.def("arch_name"	, []() { return ML_ARCH_NAME; })

			// configuration
			.def("debug"		, []() { return ML_DEBUG; })
			.def("config"		, []() { return ML_CONFIGURATION; })
			
			;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME_SYSTEM, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// OSTREAM WRAPPER
		class ostream_wrapper
		{
			std::ostream & m_os; int32_t m_no;
		public:
			explicit ostream_wrapper(std::ostream & os = std::cout) : m_os{ os }, m_no{ -1 }
			{
				auto const addr{ std::addressof(m_os) };
				if (addr == std::addressof(std::cout)) { m_no = 1; }
				else if (addr == std::addressof(std::cerr)) { m_no = 2; }
			}

			inline auto fileno() const noexcept { return m_no; }
			
			inline void flush() noexcept { m_os.flush(); }
			
			inline void write(py::object const & obj) noexcept { m_os << obj; }

			inline void writelines(py::list const & lines) noexcept
			{
				for (auto const & l : lines) { m_os << l; }
			}
		};
		py::class_<ostream_wrapper>(m, "ostream_wrapper")
			.def(py::init<>())
			.def("closed"		, []() { return false; })
			.def("isatty"		, []() { return false; })
			.def("readable"		, []() { return false; })
			.def("seekable"		, []() { return false; })
			.def("writable"		, []() { return true; })

			.def("fileno"		, &ostream_wrapper::fileno)
			.def("flush"		, &ostream_wrapper::flush)
			.def("write"		, &ostream_wrapper::write)
			.def("writelines"	, &ostream_wrapper::writelines)
			;
		static ostream_wrapper g_cout{ std::cout }, g_cerr{ std::cerr };

		m /* LIBMEME_SYSTEM */

			.def("cout"	, []() { return g_cout; })
			.def("cerr"	, []() { return g_cerr; })
			.def("cin"	, []() { return py::none{}; })
			
			.def("clear", []() { debug::clear(); })
			.def("pause", []() { debug::pause(); })
			.def("write", [](py::object const & value) { std::cout << value; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME_ENGINE, m)
	{
		m /* LIBMEME_ENGINE */
			
			// general
			.def("close"		, []() { return engine::close(); })
			.def("is_open"		, []() { return engine::is_open(); })
			
			// plugins
			.def("load_plugin"	, [](cstring s) { return engine::load_plugin(s); })

			// scripting
			.def("do_string"	, [](int32_t l, cstring s) { return engine::do_script(l, s); })
			.def("do_file"		, [](cstring s) { return engine::do_script(s); })
			
			// config
			.def("path_to"		, [](cstring s) { return engine::path_to(s).native(); })
			.def("library_home"	, []() { return engine::get_config().library_home; })
			.def("command_line"	, []() { return engine::get_config().command_line; })
			.def("content_home"	, []() { return engine::get_config().content_home; })
			.def("program_name"	, []() { return engine::get_config().program_name; })
			.def("program_path"	, []() { return engine::get_config().program_path; })
			
			// runtime
			.def("total_time"	, []() { return engine::get_time().count(); })
			.def("delta_time"	, []() { return engine::get_runtime().delta_time; })
			.def("frame_count"	, []() { return engine::get_runtime().frame_count; })
			.def("frame_rate"	, []() { return engine::get_runtime().frame_rate; })

			;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}