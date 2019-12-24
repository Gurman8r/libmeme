#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Core/StringUtility.hpp>
#include <libmeme/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Lua::Lua() {}

	Lua::~Lua() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Lua::init()
	{
		auto my_print{ ([](lua_State * L)
		{
			for (int32_t i = 1, imax = lua_gettop(L); i <= imax; ++i)
				std::cout << lua_tostring(L, i);
			return EXIT_SUCCESS;
		}) };

		static const struct luaL_Reg lib[] =
		{
			{ "print", my_print },
			{ nullptr, nullptr }
		};

		return init(true, lib);
	}

	bool Lua::init(bool openLibs, luaL_Reg const * userLib)
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

	bool Lua::dispose()
	{
		if (m_L)
		{
			lua_close(m_L);
			m_L = nullptr;
			return true;
		}
		return false;
	}

	int32_t Lua::do_string(::std::string const & value) const
	{
		return ((!value.empty() && m_L) ? luaL_dostring(m_L, value.c_str()) : 0);
	}

	int32_t Lua::do_file(path_t const & filename) const
	{
		if (auto o{ FS::read_file(filename.string()) }; o && !o.value().empty())
		{
			return do_string(std::string{ o.value().begin(), o.value().end() });
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}