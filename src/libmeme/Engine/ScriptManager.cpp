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
		j["setup_script"].get_to(m_setup_script);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool script_manager::is_initialized() const noexcept
	{
		return Py_IsInitialized();
	}

	bool script_manager::startup()
	{
		if (is_initialized()) { return false; }

		PyObject_SetArenaAllocator(([&]() noexcept
		{
			static PyObjectArenaAllocator alloc
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
			return &alloc;
		})());

		Py_SetProgramName(fs::path{ ML_argv[0] }.filename().c_str());

		Py_SetPythonHome(m_library_home.c_str());

		Py_InitializeEx(1);

		this->do_file(m_setup_script);

		return is_initialized();
	}

	bool script_manager::shutdown()
	{
		if (!is_initialized()) { return false; }

		Py_FinalizeEx();

		return !is_initialized();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t script_manager::do_file(fs::path const & value)
	{
		if (!is_initialized() || value.empty() || !fs::exists(value)) { return 0; }

		auto fp{ std::fopen(value.string().c_str(), "r") };

		return PyRun_SimpleFileExFlags(fp, value.string().c_str(), true, nullptr);
	}

	int32_t script_manager::do_string(pmr::string const & value)
	{
		if (!is_initialized() || value.empty()) { return 0; }

		return PyRun_SimpleStringFlags(value.c_str(), nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}