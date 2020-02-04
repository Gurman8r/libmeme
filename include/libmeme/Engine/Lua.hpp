#ifndef _ML_LUA_HPP_
#define _ML_LUA_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Common.hpp>

extern "C"
{
#	include <lua/lua.h>
#	include <lua/lualib.h>
#	include <lua/lauxlib.h>
}

namespace ml
{
	class ML_ENGINE_API lua final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup();

		static bool startup(bool openLibs, luaL_Reg const * userLib);

		static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t do_string(cstring const & value);

		static int32_t do_string(pmr::string const & value);

		static int32_t do_file(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_LUA_HPP_