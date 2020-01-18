#ifndef _ML_LUA_HPP_
#define _ML_LUA_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Platform/FileSystem.hpp>

extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

namespace ml
{
	struct ML_ENGINE_API Lua final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Lua() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup();

		static bool startup(bool openLibs, luaL_Reg const * userLib);

		static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t do_string(C_string const & value);

		static int32_t do_string(pmr::string const & value);

		static int32_t do_file(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static lua_State * m_L;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_LUA_HPP_