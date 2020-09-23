#include <libmeme/Core/Python.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	python_context * python_context::g_py{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	python_context::python_context(fs::path const & name, fs::path const & home, allocator_type alloc)
		: m_alloc	{ alloc }
		, m_name	{ name }
		, m_home	{ home }
	{
		ML_assert(!g_py && (g_py = this));

		ML_assert(!Py_IsInitialized());
		
		PyObject_SetArenaAllocator(std::invoke([&]() noexcept
		{
			static PyObjectArenaAllocator temp
			{
				std::addressof(m_alloc),
				[](auto alloc, size_t size) noexcept
				{
					return (void *)((allocator_type *)alloc)->allocate(size);
				},
				[](auto alloc, void * addr, size_t size) noexcept
				{
					return ((allocator_type *)alloc)->deallocate((byte_t *)addr, size);
				}
			};
			return &temp;
		}));
		
		Py_SetProgramName(m_name.c_str());
		
		Py_SetPythonHome(m_home.c_str());
		
		Py_InitializeEx(1);
		
		ML_assert(Py_IsInitialized());
	}

	python_context::~python_context() noexcept
	{
		ML_assert(g_py == this && !(g_py = nullptr));

		ML_assert(Py_IsInitialized());

		ML_assert(Py_FinalizeEx() == EXIT_SUCCESS);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t python_context::do_file(cstring path) const noexcept
	{
		ML_assert(Py_IsInitialized());

		return PyRun_SimpleFileExFlags(std::fopen(path, "r"), path, true, nullptr);
	}

	int32_t python_context::do_string(cstring str) const noexcept
	{
		ML_assert(Py_IsInitialized());

		return PyRun_SimpleStringFlags(str, nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}