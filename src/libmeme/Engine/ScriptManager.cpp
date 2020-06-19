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

		(void)initialize();

		ML_assert(is_initialized());
	}

	script_manager::~script_manager() noexcept
	{
		(void)finalize();

		ML_assert(!is_initialized());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool script_manager::is_initialized() noexcept
	{
		return Py_IsInitialized();
	}

	bool script_manager::initialize()
	{
		if (is_initialized()) { return false; }

		PyObject_SetArenaAllocator(std::invoke([]() noexcept
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
		}));
		
		Py_SetProgramName(fs::path{ __argv[0] }.filename().c_str());
		
		Py_SetPythonHome(m_library_home.c_str());
		
		Py_InitializeEx(1);

		return is_initialized();
	}

	bool script_manager::finalize()
	{
		return is_initialized() && (Py_FinalizeEx() == 0);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t script_manager::do_file(fs::path const & value)
	{
		if (!is_initialized()) { return 0; }
		else
		{
			auto fp{ std::fopen(value.string().c_str(), "r") };
			return PyRun_SimpleFileExFlags(fp, value.string().c_str(), true, nullptr);
		}
	}

	int32_t script_manager::do_string(pmr::string const & value)
	{
		if (!is_initialized()) { return 0; }
		else
		{
			return PyRun_SimpleStringFlags(value.c_str(), nullptr);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}