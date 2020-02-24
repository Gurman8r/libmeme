#include <libmeme/Engine/Python.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/Engine.hpp>

#include <Python.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static bool		s_py_init{ false };
	static fs::path	s_py_name{};
	static fs::path	s_py_home{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ml_python::initialized() noexcept
	{
		return s_py_init;
	}

	bool ml_python::startup(fs::path const & name, fs::path const & home)
	{
		if (s_py_init) return false;
		if ((s_py_name = name).empty()) return false;
		if ((s_py_home = home).empty()) return false;

		static PyObjectArenaAllocator alloc{
			nullptr,
			[](auto, size_t s) { return pmr::get_default_resource()->allocate(s); },
			[](auto, void * p, size_t s) { return pmr::get_default_resource()->deallocate(p, s); }
		};
		PyObject_SetArenaAllocator(&alloc);

		Py_SetProgramName(s_py_name.c_str());
		
		Py_SetPythonHome(s_py_home.c_str());
		
		Py_InitializeEx(Py_DontWriteBytecodeFlag);

		return (s_py_init = true);
	}

	bool ml_python::shutdown()
	{
		Py_IsInitialized();
		if (!s_py_init) return false;

		Py_FinalizeEx();

		return !(s_py_init = false);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t ml_python::do_string(cstring value)
	{
		return (s_py_init && value) ? PyRun_SimpleString(value) : 0;
	}

	int32_t ml_python::do_string(pmr::string const & value)
	{
		return do_string(value.c_str());
	}

	int32_t ml_python::do_file(fs::path const & path)
	{
		if (!s_py_init) return 0;
		if (auto const o{ FS::read_file(path.string()) }; o && !o->empty())
		{
			return do_string(pmr::string{ o->begin(), o->end() });
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::python_embedded
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using str_t			= typename pmr::string;
	using list_t		= typename pmr::vector<str_t>;
	using dict_t		= typename pmr::map<str_t, str_t>;
	using table_t		= typename pmr::vector<dict_t>;
	using vec2_t		= typename std::array<float_t, 2>;
	using rect_t		= typename std::array<float_t, 4>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME, m)
	{
		struct ml_py_config final {};
		pybind11::class_<ml_py_config>(m, "config")
			.def_static("author",	[]() { return ML__AUTHOR; })
			.def_static("date",		[]() { return ML__DATE; })
			.def_static("name",		[]() { return ML__NAME; })
			.def_static("time",		[]() { return ML__TIME; })
			.def_static("url",		[]() { return ML__URL; })
			.def_static("version",	[]() { return ML__VERSION; })
			.def_static("argv",		[]() { return list_t{ ML_ARGV, ML_ARGV + ML_ARGC }; })
			.def_static("arch",		[]() { return ML_ARCH; })
			.def_static("arch_id",	[]() { return ML_ARCH_NAME; })
			.def_static("cc_id",	[]() { return ML_CC_NAME; })
			.def_static("cc_ver",	[]() { return ML_CC_VER; })
			.def_static("cpp_ver",	[]() { return ML_LANG; })
			.def_static("is_debug",	[]() { return ML_DEBUG; })
			.def_static("os",		[]() { return ML_OS_NAME; });

		struct ml_py_io final {};
		pybind11::class_<ml_py_io>(m, "io")
			.def_static("print",	[](cstring s) { std::cout << s; })
			.def_static("printl",	[](cstring s) { std::cout << s << '\n'; })
			.def_static("clear",	[]() { return debug::clear(); })
			.def_static("exit",		[]() { return debug::exit(); })
			.def_static("pause",	[]() { return debug::pause(); })
			.def_static("info",		[](cstring s) { return debug::log_info(s); })
			.def_static("warning",	[](cstring s) { return debug::log_warning(s); })
			.def_static("error",	[](cstring s) { return debug::log_error(s); });

		struct ml_py_engine final {};
		pybind11::class_<ml_py_engine>(m, "engine")
			.def_static("load_plugin", [](cstring s) { return engine::load_plugin(s); });

	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}