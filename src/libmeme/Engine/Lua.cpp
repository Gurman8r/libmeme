#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static lua_State * m_L{ nullptr };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Lua::startup()
	{
		// My Print
		auto my_print{ ([](lua_State * L)
		{
			for (int32_t i = 1, imax = lua_gettop(L); i <= imax; ++i)
			{
				std::cout << lua_tostring(L, i);
			}
			return EXIT_SUCCESS;
		}) };

		// Lib
		static const struct luaL_Reg lib[] =
		{
			{ "print", my_print },
			{ nullptr, nullptr }
		};

		return startup(true, lib);
	}

	bool Lua::startup(bool openLibs, luaL_Reg const * userLib)
	{
		if (!m_L && (m_L = luaL_newstate()))
		{
			if (openLibs) { luaL_openlibs(m_L); }

			lua_getglobal(m_L, "_G");

			if (userLib) { luaL_setfuncs(m_L, userLib, 0); }

			lua_pop(m_L, 1);
		}
		return m_L;
	}

	bool Lua::shutdown()
	{
		if (m_L)
		{
			lua_close(m_L);
			
			m_L = nullptr;
			
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t Lua::do_string(C_string const & value)
	{
		return (value && m_L) ? luaL_dostring(m_L, value) : 0;
	}

	int32_t Lua::do_string(pmr::string const & value)
	{
		return do_string(value.c_str());
	}

	int32_t Lua::do_file(path_t const & path)
	{
		if (auto o{ FS::read_file(path.string()) }; o && !o->empty())
		{
			return do_string(pmr::string{ o->begin(), o->end() });
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}