#ifndef _ML_API_EMBED_HPP_
#define _ML_API_EMBED_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Core/Memory.hpp>

namespace pybind11 {}

namespace ml
{
	namespace py = pybind11;

	struct ML_ENGINE_API script_context final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		script_context(fs::path const & name, fs::path const & home);

		~script_context() noexcept;

		script_context(script_context &&) noexcept = default;

		script_context & operator=(script_context &&) noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t do_file(cstring path) const noexcept;

		int32_t do_file(fs::path const & path) const noexcept
		{
			return do_file(path.string().c_str());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t do_string(cstring str) const noexcept;

		int32_t do_string(pmr::string const & str) const noexcept
		{
			return do_string(str.c_str());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

#endif // !_ML_API_EMBED_HPP_