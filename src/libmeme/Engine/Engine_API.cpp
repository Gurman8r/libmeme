#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Script.hpp>
#include <libmeme/Core/Debug.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Engine/Embed.hpp>

// OUTPUT WRAPPER
namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class output_wrapper final
	{
		std::ostream & m_os; int32_t m_no;
	public:
		explicit output_wrapper(std::ostream & os = std::cout) noexcept : m_os{ os }
		{
			if (auto const addr{ std::addressof(m_os) }; !addr) { m_no = -2; }
			else if (addr == std::addressof(std::cout)) { m_no = 1; }
			else if (addr == std::addressof(std::cerr)) { m_no = 2; }
		}

		inline int32_t fileno() const noexcept { return m_no; }

		inline void flush() { m_os.flush(); }

		inline void write(py::object const & o) { m_os << o; }

		inline void writelines(py::list const & l) { for (auto const & e : l) m_os << e; }

		static decltype(auto) install(py::module m, cstring name)
		{
			return py::class_<output_wrapper>(m, name)
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
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// LIBMEME_PROJECT
namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME_PROJECT, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		m.attr("arch")			= ML_ARCH;
		m.attr("author")		= ML__AUTHOR;
		m.attr("build_date")	= ML__DATE;
		m.attr("build_time")	= ML__TIME;
		m.attr("cc")			= ML_CC_NAME;
		m.attr("cc_ver")		= ML_CC_VER;
		m.attr("config")		= ML_CONFIGURATION;
		m.attr("is_debug")		= ML_IS_DEBUG;
		m.attr("lang")			= ML_LANG;
		m.attr("name")			= ML__NAME;
		m.attr("platform")		= ML_PLATFORM;
		m.attr("url")			= ML__URL;
		m.attr("ver")			= ML__VERSION;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// LIBMEME_ENGINE
namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME_ENGINE, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// OUTPUT
		output_wrapper::install(m, "output_wrapper");
		m.attr("cout") = output_wrapper{ std::cout };
		m.attr("cerr") = output_wrapper{ std::cerr };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// CONFIG
		py::class_<engine::config>(m, "config")
			.def("arguments"	, []() { return engine::get_config().arguments; })
			.def("content_home"	, []() { return engine::get_config().content_home.native(); })
			.def("library_home"	, []() { return engine::get_config().library_home.native(); })
			.def("program_name"	, []() { return engine::get_config().program_name.native(); })
			.def("program_path"	, []() { return engine::get_config().program_path.native(); })
			.def("setup_script"	, []() { return engine::get_config().setup_script.native(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// RUNTIME
		py::class_<engine::runtime>(m, "runtime")
			.def("delta_time"	, []() { return engine::get_runtime().delta_time; })
			.def("frame_count"	, []() { return engine::get_runtime().frame_count; })
			.def("frame_rate"	, []() { return engine::get_runtime().frame_rate; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SCRIPT
		struct impl_script
		{
			using callback_t = typename std::function<py::object(py::args, py::kwargs)>;
			
			py::object	m_self;
			py::args	m_args;
			py::kwargs	m_kwargs;
			callback_t	m_clbk;

			impl_script(py::object self, py::args args, py::kwargs kwargs)
				: m_self	{ self }
				, m_args	{ args }
				, m_kwargs	{ kwargs }
				, m_clbk	{ m_self.attr("invoke").cast<callback_t>() }
			{
			}

			py::object operator()(py::args args, py::kwargs kwargs)
			{
				return m_clbk ? std::invoke(m_clbk, args, kwargs) : py::none{};
			}
		};
		py::class_<impl_script>(m, "script")
			.def(py::init<py::object, py::args, py::kwargs>())
			.def("__call__", &impl_script::operator(), py::is_operator())
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		m // FUNCTIONS
			.def("close"		, []() { return engine::close(); })
			.def("path_to"		, [](cstring s) { return engine::path_to(s).native(); })
			.def("load_plugin"	, [](cstring s) { return engine::load_plugin(s); })
			.def("do_string"	, [](int32_t l, cstring s) { return engine::do_string(l, s); })
			.def("do_file"		, [](cstring s) { return engine::do_file(s); })
			.def("hook"			, &engine::add_hook)
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// INSTALL
		m.attr("__ml_install__") = ([&
			, builtins = py::module::import("builtins")
			, sys = py::module::import("sys")
		]()
		{
			builtins.attr("exit")	= m.attr("close");
			sys.attr("exit")		= m.attr("close");
			sys.attr("stdout")		= m.attr("cout");
			sys.attr("stderr")		= m.attr("cout");
			sys.attr("stdin")		= py::none{};
			return true;
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}