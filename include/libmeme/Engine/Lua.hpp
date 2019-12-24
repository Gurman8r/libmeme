#ifndef _ML_LUA_HPP_
#define _ML_LUA_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Singleton.hpp>

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#define ML_Lua ::ml::Lua::getInstance()

namespace ml
{
	struct ML_ENGINE_API Lua final : public Singleton<Lua>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool init();

		bool init(bool openLibs, luaL_Reg const * userLib);

		bool dispose();

		int32_t do_string(std::string const & value) const;

		int32_t do_file(path_t const & filename) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<Lua>;

		Lua();
		~Lua();

		lua_State * m_L{ nullptr };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_LUA_HPP_