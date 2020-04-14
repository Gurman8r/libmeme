#ifndef _ML_EMBED_HPP_
#define _ML_EMBED_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Matrix.hpp>

namespace ml::embed {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_EMBED_PYTHON

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Python.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/iostream.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::embed
{
    namespace py = pybind11;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace pybind11::detail
{
    template <typename T, size_t N> struct type_caster<_ML ds::array<T, N>>
        : array_caster<_ML ds::array<T, N>, T, false, N> {};

    template <typename T, size_t W, size_t H> struct type_caster<_ML ds::matrix<T, W, H>>
        : array_caster<_ML ds::matrix<T, W, H>, T, false, W * H> {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // ML_EMBED_PYTHON

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_EMBED_HPP_