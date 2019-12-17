#include <libmeme/Engine/Python.hpp>
#include <libmeme/Core/StringUtility.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <Python.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Python::Python() {}

	Python::~Python() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Python::init(Path const & name, Path const & home)
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

	bool Python::dispose()
	{
		if (m_init)
		{
			Py_Finalize();
			
			m_init = false;
			
			return true;
		}
		return false;
	}

	int32_t Python::do_string(std::string const & value) const
	{
		return ((!value.empty() && m_init) ? PyRun_SimpleString(value.c_str()) : 0);
	}

	int32_t Python::do_file(Path const & filename) const
	{
		if (auto o{ FS::read_file(filename) }; o && !o.value().empty())
		{
			return do_string(std::string{ o.value().begin(), o.value().end() });
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}