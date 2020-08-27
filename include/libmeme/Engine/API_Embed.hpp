#ifndef _ML_API_EMBED_HPP_
#define _ML_API_EMBED_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Matrix.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Python.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/iostream.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	namespace py = pybind11;

	struct script_context final : non_copyable
	{
		script_context(fs::path const & name, fs::path const & home)
		{
			ML_assert(!Py_IsInitialized());
			PyObject_SetArenaAllocator(std::invoke([&temp = PyObjectArenaAllocator{}]()
			{
				temp.alloc = [](auto, size_t size) noexcept
				{
					return pmr::get_default_resource()->allocate(size);
				};
				temp.free = [](auto, void * addr, size_t size) noexcept
				{
					return pmr::get_default_resource()->deallocate(addr, size);
				};
				return std::addressof(temp);
			}));
			Py_SetProgramName(name.c_str());
			Py_SetPythonHome(home.c_str());
			Py_InitializeEx(1);
			ML_assert(Py_IsInitialized());
		}

		~script_context() noexcept
		{
			ML_assert(Py_IsInitialized());
			ML_assert(Py_FinalizeEx() == EXIT_SUCCESS);
		}

		int32_t do_file(fs::path const & path) const
		{
			ML_assert(Py_IsInitialized());
			pmr::string const str{ path.string() };
			return PyRun_SimpleFileExFlags(std::fopen(str.c_str(), "r"), str.c_str(), true, nullptr);
		}

		int32_t do_string(pmr::string const & str) const
		{
			ML_assert(Py_IsInitialized());
			return PyRun_SimpleStringFlags(str.c_str(), nullptr);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace pybind11::detail
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace pybind11
{
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

#endif // !_ML_API_EMBED_HPP_