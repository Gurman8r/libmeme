#include <libmeme/Engine/ScriptManager.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/API_Embed.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	script_manager::script_manager(json const & j, allocator_type const & alloc) noexcept
	{
		j["library_home"].get_to(m_library_home);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool script_manager::startup()
	{
		if (Py_IsInitialized()) { return false; }

		PyObject_SetArenaAllocator(([]() noexcept
		{
			static PyObjectArenaAllocator temp
			{
				nullptr,
				[](auto, size_t s) noexcept
				{
					return pmr::get_default_resource()->allocate(s);
				},
				[](auto, void * p, size_t s) noexcept
				{
					return pmr::get_default_resource()->deallocate(p, s);
				}
			};
			return &temp;
		})());

		Py_SetProgramName(fs::path{ ML_argv[0] }.filename().c_str());

		Py_SetPythonHome(m_library_home.c_str());

		Py_InitializeEx(1);

		return Py_IsInitialized();
	}

	bool script_manager::shutdown()
	{
		if (!Py_IsInitialized()) { return false; }

		return (0 < Py_FinalizeEx());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t script_manager::do_file(fs::path const & value)
	{
		if (!Py_IsInitialized()) { return 0; }

		auto fp{ std::fopen(value.string().c_str(), "r") };

		return PyRun_SimpleFileExFlags(fp, value.string().c_str(), true, nullptr);
	}

	int32_t script_manager::do_string(pmr::string const & value)
	{
		if (!Py_IsInitialized()) { return 0; }

		return PyRun_SimpleStringFlags(value.c_str(), nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}