#include <libmeme/Engine/Python.hpp>
#include <Python.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Py::init(std::string const & name, std::string const & home)
	{
		if (!m_init && !name.empty() && ML_FS.dirExists(home))
		{
			Py_SetProgramName(util::widen(name).c_str());
			Py_SetPythonHome(util::widen(home).c_str());
			Py_InitializeEx(Py_DontWriteBytecodeFlag);
			m_init = true;
			return true;
		}
		return false;
	}

	bool Py::restart()
	{
		return dispose() && init(m_name, m_home);
	}

	bool Py::dispose()
	{
		if (m_init)
		{
			Py_Finalize();
			m_init = false;
			return true;
		}
		return false;
	}

	int32_t Py::doString(std::string const & value) const
	{
		return ((!value.empty() && m_init) ? PyRun_SimpleString(value.c_str()) : 0);
	}

	int32_t Py::doFile(std::string const & filename) const
	{
		std::string content;
		if (ML_FS.getFileContents(filename, content))
		{
			return doString(content);
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}