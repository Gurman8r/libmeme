#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Client/Export.hpp>

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

	struct ML_CLIENT_API py_interpreter final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		py_interpreter(pmr::memory_resource * mres, fs::path const & name, fs::path const & home);

		~py_interpreter() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t do_file(cstring path) noexcept;

		static int32_t do_file(fs::path const & path) noexcept
		{
			return do_file(path.string().c_str());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t do_string(cstring str) noexcept;

		static int32_t do_string(pmr::string const & str) noexcept
		{
			return do_string(str.c_str());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PYTHON_HPP_