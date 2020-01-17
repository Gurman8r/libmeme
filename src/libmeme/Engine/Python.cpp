#include <libmeme/Engine/Python.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Engine/EngineEvents.hpp>

#include <Python.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool	Python::m_init{ false };
	path_t	Python::m_name{};
	path_t	Python::m_home{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Python::startup(path_t const & name, path_t const & home)
	{
		m_name = name;
		m_home = home;
		if (!m_init && !m_name.empty() && !m_home.empty())
		{
			Py_SetProgramName(util::widen(m_name.string()).c_str());

			Py_SetPythonHome(util::widen(m_home.string()).c_str());

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

	int32_t Python::do_string(std::string const & value)
	{
		return ((!value.empty() && m_init) ? PyRun_SimpleString(value.c_str()) : 0);
	}

	int32_t Python::do_file(path_t const & path)
	{
		if (auto o{ FS::read_file(path.string()) }; o && !o->empty())
		{
			return do_string(std::string{ o->begin(), o->end() });
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::python::embedded
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using py_str_t		= typename std::string;
	using py_list_t		= typename std::vector<py_str_t>;
	using py_dict_t		= typename std::map<py_str_t, py_str_t>;
	using py_table_t	= typename std::vector<py_dict_t>;
	using py_vec2_t		= typename std::array<float_t, 2>;
	using py_rect_t		= typename std::array<float_t, 4>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME, m)
	{
		struct ml_py_config final {};
		pybind11::class_<ml_py_config>(m, "cfg")
			.def_static("args",				[]() { return py_list_t{ ML_ARGV, ML_ARGV + ML_ARGC }; })
			.def_static("architecture",		[]() { return ML_ARCH; })
			.def_static("compiler_name",	[]() { return ML_CC_NAME; })
			.def_static("compiler_version", []() { return ML_CC_VER; })
			.def_static("configuration",	[]() { return ML_CONFIGURATION; })
			.def_static("cpp_version",		[]() { return ML_LANG; })
			.def_static("is_debug",			[]() { return ML_DEBUG; })
			.def_static("platform_target",	[]() { return ML_ARCH_NAME; })
			.def_static("project_author",	[]() { return ML__AUTHOR; })
			.def_static("project_date",		[]() { return ML__DATE; })
			.def_static("project_name",		[]() { return ML__NAME; })
			.def_static("project_time",		[]() { return ML__TIME; })
			.def_static("project_url",		[]() { return ML__URL; })
			.def_static("project_version",	[]() { return ML__VERSION; })
			.def_static("system_name",		[]() { return ML_OS_NAME; });

		struct ml_py_io final {};
		pybind11::class_<ml_py_io>(m, "io")
			.def_static("clear",	[]() { return debug::clear(); })
			.def_static("exit",		[]() { return std::exit(0); })
			.def_static("pause",	[]() { return debug::pause(0); })
			.def_static("print",	[](py_str_t const & s) { std::cout << s; })
			.def_static("printf",	[](py_str_t const & s, py_list_t const & l) { std::cout << util::format(s, l); })
			.def_static("printl",	[](py_str_t const & s) { std::cout << s << '\n'; })
			.def_static("log",		[](py_str_t const & s) { return debug::log_info(s); })
			.def_static("warning",	[](py_str_t const & s) { return debug::log_warning(s); })
			.def_static("error",	[](py_str_t const & s) { return debug::log_error(s); });
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}