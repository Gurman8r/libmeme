#include <libmeme/Scripting/Python.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Engine/Engine.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Scripting/Embed.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ml_python::is_initialized() noexcept
	{
		return engine::is_initialized() && Py_IsInitialized();
	}

	bool ml_python::initialize()
	{
		if (is_initialized()) return false;

		// get config
		auto const & cfg{ engine::get_config() };

		// set allocators
		static PyObjectArenaAllocator arena
		{
			nullptr,
			[](auto, size_t s)
			{
				return pmr::get_default_resource()->allocate(s);
			},
			[](auto, void * p, size_t s)
			{
				return pmr::get_default_resource()->deallocate(p, s);
			}
		};
		PyObject_SetArenaAllocator(&arena);

		// program name
		Py_SetProgramName(cfg.program_name.c_str());

		// library path
		Py_SetPythonHome(cfg.library_home.c_str());

		// initialize
		Py_Initialize();

		return is_initialized();
	}

	bool ml_python::finalize()
	{
		if (!is_initialized()) return false;
		
		// finalize
		Py_Finalize();
		
		return !is_initialized();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t ml_python::do_string(cstring value)
	{
		if (!is_initialized() || !value) return 0;
		return PyRun_SimpleString(value);
	}

	int32_t ml_python::do_string(pmr::string const & value)
	{
		return do_string(value.c_str());
	}

	int32_t ml_python::do_file(filesystem::path const & path)
	{
		if (!is_initialized())
		{
			return 0;
		}
		else if (auto const o{ util::get_file_contents(path.string()) }; o && !o->empty())
		{
			return do_string(pmr::string{ o->begin(), o->end() });
		}
		else
		{
			return 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}