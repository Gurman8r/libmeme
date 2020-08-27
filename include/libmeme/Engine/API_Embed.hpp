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

	static bool initialize_python(
		fs::path const & name,
		fs::path const & home,
		PyObjectArenaAllocator && alloc
	)
	{
		if (Py_IsInitialized()) { return false; }
		PyObject_SetArenaAllocator(std::addressof(alloc));
		Py_SetProgramName(name.c_str());
		Py_SetPythonHome(home.c_str());
		Py_InitializeEx(1);
		return Py_IsInitialized();
	}

	static bool finalize_python()
	{
		return Py_IsInitialized() && (Py_FinalizeEx() == EXIT_SUCCESS);
	}
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