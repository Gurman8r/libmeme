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

	static auto py_json_dumps(py::object o)
	{
		return py::str(py::module::import("json").attr("dumps")(o.attr("__dict__")));
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
	static void to_json(_ML json & j, none const & v) { j = nullptr; }
	static void from_json(_ML json const & j, none & v) {}

	static void to_json(_ML json & j, bool_ const & v) { j = (bool)v; }
	static void from_json(_ML json const & j, bool_ & v) { v = j.get<bool>(); }

	static void to_json(_ML json & j, int_ const & v) { j = (int)v; }
	static void from_json(_ML json const & j, int_ & v) { v = j.get<int32_t>(); }

	static void to_json(_ML json & j, float_ const & v) { j = (float)v; }
	static void from_json(_ML json const & j, float_ & v) { v = j.get<float>(); }

	static void to_json(_ML json & j, str const & v) { j = (std::string)v; }
	static void from_json(_ML json const & j, str & v) { v = j.get<std::string>(); }

	static void to_json(_ML json & j, list const & v) {}
	static void from_json(_ML json const & j, list & v) {}

	static void to_json(_ML json & j, dict const & v) {}
	static void from_json(_ML json const & j, dict & v) {}

	static void to_json(_ML json & j, object const & v)
	{
		j = _ML json::parse((std::string)_ML py_json_dumps(v));
	}

	static void from_json(_ML json const & j, object & v) {}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	template <class T> static auto py_json_str(T o) noexcept
	{
		return py::str(py::str(json{ o }.dump()).attr("strip")("[]"));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_API_EMBED_HPP_