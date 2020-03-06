#ifndef _ML_EMBED_HPP_
#define _ML_EMBED_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_EMBED_LUA
extern "C"
{
#	include <lua/lua.h>
#	include <lua/lualib.h>
#	include <lua/lauxlib.h>
}
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_EMBED_PYTHON
#	include <Python.h>
#	include <pybind11/embed.h>
#	include <pybind11/stl.h>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_EMBED_HPP_