#include <libmeme/Engine/ScriptManager.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/API_Embed.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	script_manager::script_manager(json const & j, allocator_type alloc) noexcept
	{
		j["library_home"].get_to(m_library_home);
	}

	script_manager::~script_manager() noexcept
	{
		finalize();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool script_manager::is_initialized() noexcept
	{
		return Py_IsInitialized();
	}

	bool script_manager::initialize()
	{
		if (is_initialized()) { return false; }

		PyObject_SetArenaAllocator(std::invoke([&temp = PyObjectArenaAllocator{}]() noexcept
		{
			temp.alloc = [](auto, size_t s) noexcept
			{
				return pmr::get_default_resource()->allocate(s);
			};
			temp.free = [](auto, void * p, size_t s) noexcept
			{
				return pmr::get_default_resource()->deallocate(p, s);
			};
			return &temp;
		}));

		Py_SetProgramName(fs::path{ __argv[0] }.filename().c_str());

		Py_SetPythonHome(m_library_home.c_str());

		Py_InitializeEx(1);

		return Py_IsInitialized();
	}

	bool script_manager::finalize()
	{
		if (!is_initialized()) { return false; }

		return (0 < Py_FinalizeEx());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t script_manager::do_file(fs::path const & value)
	{
		if (!is_initialized())
		{
			return 0;
		}
		else if (auto fp{ std::fopen(value.string().c_str(), "r") })
		{
			return PyRun_SimpleFileExFlags(fp, value.string().c_str(), true, nullptr);
		}
		else
		{
			return 0;
		}
	}

	int32_t script_manager::do_string(pmr::string const & value)
	{
		if (!is_initialized())
		{
			return 0;
		}
		else
		{
			return PyRun_SimpleStringFlags(value.c_str(), nullptr);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}