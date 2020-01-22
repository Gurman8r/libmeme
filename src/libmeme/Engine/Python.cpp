#include <libmeme/Engine/Python.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/Engine.hpp>

#include <Python.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static bool		m_init{ false };
	static path_t	m_name{};
	static path_t	m_home{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Python::startup(path_t const & name, path_t const & home)
	{
		m_name = name;
		m_home = home;
		if (!m_init && !m_name.empty() && !m_home.empty())
		{
			Py_SetProgramName(m_name.c_str());

			Py_SetPythonHome(m_home.c_str());

			Py_InitializeEx(Py_DontWriteBytecodeFlag);

			m_init = true;

			return true;
		}
		return false;
	}

	bool Python::shutdown()
	{
		if (m_init)
		{
			Py_Finalize();

			m_init = false;

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t Python::do_string(C_string value)
	{
		return (value && m_init) ? PyRun_SimpleString(value) : 0;
	}

	int32_t Python::do_string(pmr::string const & value)
	{
		return do_string(value.c_str());
	}

	int32_t Python::do_file(path_t const & path)
	{
		if (auto o{ FS::read_file(path.string()) }; o && !o->empty())
		{
			return do_string(pmr::string{ o->begin(), o->end() });
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::python::embedded
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using str_t			= typename std::string;
	using list_t		= typename std::vector<str_t>;
	using dict_t		= typename std::map<str_t, str_t>;
	using table_t		= typename std::vector<dict_t>;
	using vec2_t		= typename std::array<float_t, 2>;
	using rect_t		= typename std::array<float_t, 4>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME, m)
	{
		struct ml_py_config final {};
		pybind11::class_<ml_py_config>(m, "config")
			.def_static("args",		[]() { return list_t{ ML_ARGV, ML_ARGV + ML_ARGC }; })
			.def_static("author",	[]() { return ML__AUTHOR; })
			.def_static("date",		[]() { return ML__DATE; })
			.def_static("name",		[]() { return ML__NAME; })
			.def_static("time",		[]() { return ML__TIME; })
			.def_static("url",		[]() { return ML__URL; })
			.def_static("version",	[]() { return ML__VERSION; })
			.def_static("arch",		[]() { return ML_ARCH; })
			.def_static("arch_id",	[]() { return ML_ARCH_NAME; })
			.def_static("cc_id",	[]() { return ML_CC_NAME; })
			.def_static("cc_ver",	[]() { return ML_CC_VER; })
			.def_static("cpp_ver",	[]() { return ML_LANG; })
			.def_static("is_debug",	[]() { return ML_DEBUG; })
			.def_static("os",		[]() { return ML_OS_NAME; });

		struct ml_py_io final {};
		pybind11::class_<ml_py_io>(m, "io")
			.def_static("clear",	[]() { return debug::clear(); })
			.def_static("exit",		[]() { return std::exit(0); })
			.def_static("pause",	[]() { return debug::pause(0); })
			.def_static("print",	[](C_string s) { std::cout << s; })
			.def_static("printl",	[](C_string s) { std::cout << s << '\n'; })
			.def_static("info",		[](C_string s) { return debug::log_info(s); })
			.def_static("warning",	[](C_string s) { return debug::log_warning(s); })
			.def_static("error",	[](C_string s) { return debug::log_error(s); });

		struct ml_py_engine final {};
		pybind11::class_<ml_py_engine>(m, "engine")
			.def_static("load_plugin", [](C_string s) { return engine::load_plugin(s); });
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}