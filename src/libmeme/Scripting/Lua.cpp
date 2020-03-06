#include <libmeme/Scripting/Lua.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Core/FileSystem.hpp>

#define ML_EMBED_LUA
#include <libmeme/Scripting/Embed.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
	template <class Fn
	> static int32_t lua_for(lua_State * L, Fn && fn)
	{
		if (!L) return EXIT_FAILURE * 1;
		for (int32_t i = 1, imax = lua_gettop(L); i <= imax; ++i)
		{
			std::invoke(ML_FWD(fn), i);
		}
		return EXIT_SUCCESS;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::embedded
{
	static constexpr struct luaL_Reg lua_default[] =
	{
		luaL_Reg{ "exit", [](auto L)
		{
			engine::get_window()->close(); return 0;
		} },
		
		luaL_Reg{ "print", [](auto L)
		{
			return impl::lua_for(L, [&](int32_t i)
			{
				std::cout << lua_tostring(L, i);
			});
		} },

		luaL_Reg{ nullptr, nullptr },
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static lua_State * s_lua{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ml_lua::is_initialized() noexcept
	{
		return engine::is_initialized() && s_lua;
	}

	bool ml_lua::initialize()
	{
		if (is_initialized()) return false;

		// lua allocator
		auto alloc = [](auto, void * p, size_t o, size_t n)
		{
			return memory_manager::reallocate(p, o, n);
		};

		// new state
		if (!(s_lua = lua_newstate(alloc, nullptr)))
		{
			return false;
		}

		// open libs
		luaL_openlibs(s_lua);

		lua_getglobal(s_lua, "_G");

		// user libs
		luaL_setfuncs(s_lua, embedded::lua_default, 0);

		lua_pop(s_lua, 1);

		return is_initialized();
	}

	bool ml_lua::finalize()
	{
		if (s_lua)
		{
			lua_close(s_lua);
			
			s_lua = nullptr;
			
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t ml_lua::do_string(cstring const & value)
	{
		if (!is_initialized() || !value) return 0;
		return luaL_dostring(s_lua, value);
	}

	int32_t ml_lua::do_string(pmr::string const & value)
	{
		return do_string(value.c_str());
	}

	int32_t ml_lua::do_file(filesystem::path const & path)
	{
		if (auto o{ util::get_file_contents(path.string()) }; o && !o->empty())
		{
			return do_string(pmr::string{ o->begin(), o->end() });
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}