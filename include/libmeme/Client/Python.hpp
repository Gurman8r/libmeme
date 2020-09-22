#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Client/Export.hpp>
#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	struct event_bus;

	struct ML_CLIENT_API python_context final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_NODISCARD path_info final
		{
			fs::path const
				name, // program name
				home; // library home
		};

		python_context(event_bus * bus, path_info const & paths);

		~python_context() noexcept;

		python_context(python_context &&) noexcept = default;

		python_context & operator=(python_context &&) noexcept = default;

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

	private:
		event_bus * const m_bus;
		path_info m_paths;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define HAVE_SNPRINTF

#include <Python.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/iostream.h>

namespace ml { namespace py = pybind11; }

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

#endif // !_ML_PYTHON_HPP_