#include <libmeme/Engine/ScriptManager.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/Embed.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	script_manager::script_manager(allocator_type const & alloc) noexcept
	{
	}

	script_manager::~script_manager() noexcept
	{
		shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool script_manager::startup(fs::path const & name, fs::path const & home)
	{
		if (Py_IsInitialized()) { return false; }

		PyObject_SetArenaAllocator(([&]()
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
			return std::addressof(alloc);
		})());

		Py_SetProgramName(name.c_str());

		Py_SetPythonHome(home.c_str());

		Py_InitializeEx(1);

		return Py_IsInitialized();
	}

	bool script_manager::shutdown()
	{
		if (!Py_IsInitialized()) { return false; }

		Py_FinalizeEx();

		return !Py_IsInitialized();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t script_manager::do_file(fs::path const & path)
	{
		if (path.empty() || !fs::exists(path)) { return 0; }

		auto file{ std::fopen(path.string().c_str(), "r") };

		return PyRun_SimpleFileExFlags(file, path.string().c_str(), true, nullptr);
	}

	int32_t script_manager::do_string(pmr::string const & text)
	{
		if (text.empty()) { return 0; }

		return PyRun_SimpleStringFlags(text.c_str(), nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}