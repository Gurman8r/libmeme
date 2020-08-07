#include <libmeme/Engine/ScriptManager.hpp>
#include <libmeme/Engine/API_Embed.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	script_manager::script_manager(json const & j, allocator_type alloc) noexcept
	{
		ML_assert(!Py_IsInitialized());

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

#if 0
		Py_SetPythonHome((
			j["path"]["content"].get<fs::path>().native() +
			j["path"]["library"].get<fs::path>().native()
		).c_str());
#else
		Py_SetPythonHome(j["path"]["library"].get<fs::path>().c_str());
#endif

		Py_InitializeEx(1);

		ML_assert(Py_IsInitialized());
	}

	script_manager::~script_manager() noexcept
	{
		ML_assert(Py_FinalizeEx() == EXIT_SUCCESS);

		ML_assert(!Py_IsInitialized());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t script_manager::do_file(fs::path const & value)
	{
		ML_assert(Py_IsInitialized());
		
		auto fp{ std::fopen(value.string().c_str(), "r") };
		
		return PyRun_SimpleFileExFlags(fp, value.string().c_str(), true, nullptr);
	}

	int32_t script_manager::do_string(pmr::string const & value)
	{
		ML_assert(Py_IsInitialized());

		return PyRun_SimpleStringFlags(value.c_str(), nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}