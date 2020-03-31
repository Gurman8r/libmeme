#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Core/Debug.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/ScriptableObject.hpp>

// OUTPUT WRAPPER
namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class output_handler final
	{
		std::ostream & m_os; int32_t m_no;
	public:
		explicit output_handler(std::ostream & os = std::cout) noexcept : m_os{ os }
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
			return py::class_<output_handler>{ m, name }
				.def(py::init<>())
				.def("closed"		, []() { return false; })
				.def("isatty"		, []() { return false; })
				.def("readable"		, []() { return false; })
				.def("seekable"		, []() { return false; })
				.def("writable"		, []() { return true; })
				.def("fileno"		, &output_handler::fileno)
				.def("flush"		, &output_handler::flush)
				.def("write"		, &output_handler::write)
				.def("writelines"	, &output_handler::writelines)
				;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// LIBMEME_ENGINE
namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME_ENGINE, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// LIB
		struct ml_lib {};
		py::class_<ml_lib>{ m, "lib" }
			.def("arch"			, []() { return ML_ARCH; })
			.def("author"		, []() { return ML__AUTHOR; })
			.def("cc"			, []() { return ML_CC_NAME; })
			.def("cc_ver"		, []() { return ML_CC_VER; })
			.def("config"		, []() { return ML_CONFIGURATION; })
			.def("date"			, []() { return ML__DATE; })
			.def("is_debug"		, []() { return ML_IS_DEBUG; })
			.def("lang"			, []() { return ML_LANG; })
			.def("name"			, []() { return ML__NAME; })
			.def("platform"		, []() { return ML_PLATFORM; })
			.def("time"			, []() { return ML__TIME; })
			.def("url"			, []() { return ML__URL; })
			.def("version"		, []() { return ML__VERSION; })
			;
		
		// OUTPUT
		output_handler::install(m, "output_handler");
		m.attr("cout") = output_handler{ std::cout };
		m.attr("cerr") = output_handler{ std::cerr };

		// SCRIPTING
		scriptable_object::install(m, "scriptable_object");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// CONFIG
		py::class_<engine::config>{ m, "config" }
			.def("arguments"	, []() { return engine::get_config().arguments; })
			.def("content_home"	, []() { return engine::get_config().content_home.native(); })
			.def("library_home"	, []() { return engine::get_config().library_home.native(); })
			.def("program_name"	, []() { return engine::get_config().program_name.native(); })
			.def("program_path"	, []() { return engine::get_config().program_path.native(); })
			.def("setup_script"	, []() { return engine::get_config().setup_script.native(); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// RUNTIME
		py::class_<engine::runtime>{ m, "runtime" }
			.def("delta_time"	, []() { return engine::get_runtime().delta_time; })
			.def("frame_count"	, []() { return engine::get_runtime().frame_count; })
			.def("frame_rate"	, []() { return engine::get_runtime().frame_rate; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		m // FUNCTIONS
			.def("close"		, [](py::args) { return engine::close(); })
			.def("load_plugin"	, [](cstring s) { return engine::load_plugin(s); })
			.def("do_string"	, [](int32_t l, cstring s) { return engine::do_string(l, s); })
			.def("do_file"		, [](cstring s) { return engine::do_file(s); })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// INSTALL
		m.attr("__good__") = ([&
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