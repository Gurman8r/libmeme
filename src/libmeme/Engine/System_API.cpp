#include <libmeme/Core/Debug.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Engine/Embed.hpp>

namespace ml::embed
{
	PYBIND11_EMBEDDED_MODULE(LIBMEME_SYSTEM, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// PROJECT
		struct system_project {};
		py::class_<system_project>(m, "prj")
			.def_static("auth"	, []() { return ML__AUTHOR; })
			.def_static("date"	, []() { return ML__DATE; })
			.def_static("name"	, []() { return ML__NAME; })
			.def_static("time"	, []() { return ML__TIME; })
			.def_static("url"	, []() { return ML__URL; })
			.def_static("ver"	, []() { return ML__VERSION; })
			;

		// CONFIG
		struct system_config {};
		py::class_<system_config>(m, "cfg")
			.def_static("arch"		, []() { return ML_ARCH; })
			.def_static("cc_name"	, []() { return ML_CC_NAME; })
			.def_static("cc_ver"	, []() { return ML_CC_VER; })
			.def_static("config"	, []() { return ML_CONFIGURATION; })
			.def_static("is_debug"	, []() { return ML_DEBUG; })
			.def_static("lang"		, []() { return ML_LANG; })
			.def_static("platform"	, []() { return ML_PLATFORM; })
			;

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		m /* LIBMEME_SYSTEM */

			.def("clear", []() { debug::clear(); })
			.def("pause", []() { debug::pause(); })
			
			.def("cout"	, []() { return g_cout; })
			.def("cerr"	, []() { return g_cerr; })
			.def("cin"	, []() { return py::none{}; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}