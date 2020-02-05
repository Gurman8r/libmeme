#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static lua_State * s_L{ nullptr };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool lua::initialized() noexcept
	{
		return s_L;
	}

	bool lua::startup()
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

	bool lua::startup(bool openLibs, luaL_Reg const * userLib)
	{
		if (!s_L && (s_L = luaL_newstate()))
		{
			if (openLibs) { luaL_openlibs(s_L); }

			lua_getglobal(s_L, "_G");

			if (userLib) { luaL_setfuncs(s_L, userLib, 0); }

			lua_pop(s_L, 1);
		}
		return s_L;
	}

	bool lua::shutdown()
	{
		if (s_L)
		{
			lua_close(s_L);
			
			s_L = nullptr;
			
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t lua::do_string(cstring const & value)
	{
		return (value && s_L) ? luaL_dostring(s_L, value) : 0;
	}

	int32_t lua::do_string(pmr::string const & value)
	{
		return do_string(value.c_str());
	}

	int32_t lua::do_file(fs::path const & path)
	{
		if (auto o{ FS::read_file(path.string()) }; o && !o->empty())
		{
			return do_string(pmr::string{ o->begin(), o->end() });
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}