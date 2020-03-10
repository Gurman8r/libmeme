#ifndef _ML_EMBED_HPP_
#define _ML_EMBED_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Hash.hpp>

namespace ml::embed
{
	class api final
	{
	public:
		enum language { unknown, lua, python };

		template <class Str = cstring
		> ML_NODISCARD static constexpr int32_t ext_id(Str const & str) noexcept
		{
			switch (util::hash(str))
			{
			default: return language::unknown;
			case util::hash(".lua"): return language::lua;
			case util::hash(".py"): return language::python;
			}
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// LUA
#ifdef ML_EMBED_LUA
extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// PYTHON
#ifdef ML_EMBED_PYTHON
#include <Python.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/iostream.h>
namespace ml::embed { namespace py = pybind11; }
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_EMBED_HPP_