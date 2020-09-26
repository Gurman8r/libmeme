#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Core/Memory.hpp>

// python
#define HAVE_SNPRINTF
#include <Python.h>

// pybind11
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/iostream.h>

namespace pybind11
{
	namespace detail
	{
		// array caster
		template <class T, size_t N
		> struct type_caster<_ML ds::array<T, N>>
			: array_caster<_ML ds::array<T, N>, T, false, N> {};

		// matrix caster
		template <class T, size_t W, size_t H
		> struct type_caster<_ML ds::matrix<T, W, H>>
			: array_caster<_ML ds::matrix<T, W, H>, T, false, W * H> {};
	}

	static void to_json(_ML json & j, handle const & v)
	{
		j = _ML json::parse((std::string)(str)module::import("json").attr("dumps")(v));
	}

	static void from_json(_ML json const & j, handle & v)
	{
		v = module::import("json").attr("loads")(j.dump());
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	namespace py = pybind11;

	struct py_interpreter final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		py_interpreter(pmr::memory_resource * mres, fs::path const & name, fs::path const & home)
		{
			ML_assert(!Py_IsInitialized());
			PyObject_SetArenaAllocator(std::invoke([&mres]() noexcept
			{
				static PyObjectArenaAllocator temp
				{
					mres,
					[](auto mres, size_t size) noexcept
					{
						return ((pmr::memory_resource *)mres)->allocate(size);
					},
					[](auto mres, void * addr, size_t size) noexcept
					{
						return ((pmr::memory_resource *)mres)->deallocate(addr, size);
					}
				};
				return &temp;
			}));
			Py_SetProgramName(name.c_str());
			Py_SetPythonHome(home.c_str());
			Py_InitializeEx(1);
			ML_assert(Py_IsInitialized());
		}

		~py_interpreter() noexcept
		{
			ML_assert(Py_IsInitialized());
			ML_assert(Py_FinalizeEx() == EXIT_SUCCESS);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t do_file(cstring path) noexcept
		{
			ML_assert(Py_IsInitialized());
			return PyRun_SimpleFileExFlags(std::fopen(path, "r"), path, true, nullptr);
		}

		static int32_t do_file(fs::path const & path) noexcept
		{
			return do_file(path.string().c_str());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t do_string(cstring str) noexcept
		{
			ML_assert(Py_IsInitialized());
			return PyRun_SimpleStringFlags(str, nullptr);
		}

		static int32_t do_string(pmr::string const & str) noexcept
		{
			return do_string(str.c_str());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PYTHON_HPP_