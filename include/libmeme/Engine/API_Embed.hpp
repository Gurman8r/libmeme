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

namespace ml { namespace py = pybind11; }

namespace pybind11::detail
{
	// array caster
	template <class T, size_t N
	> struct type_caster<_ML_DS array<T, N>>
		: array_caster<_ML_DS array<T, N>, T, false, N> {};
	
	// matrix caster
	template <class T, size_t W, size_t H
	> struct type_caster<_ML_DS matrix<T, W, H>>
		: array_caster<_ML_DS matrix<T, W, H>, T, false, W * H> {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_API_EMBED_HPP_