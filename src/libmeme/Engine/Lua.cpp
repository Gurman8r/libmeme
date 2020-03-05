#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Core/Memory.hpp>

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

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static lua_State * s_L{ nullptr };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ml_lua::initialized() noexcept
	{
		return s_L;
	}

	bool ml_lua::startup()
	{
		static constexpr struct luaL_Reg lib[] =
		{
			// print
			luaL_Reg{ "print", ([](auto L)
			{
				return impl::lua_for(L, [&](int32_t i)
				{
					std::cout << lua_tostring(L, i);
				});
			}) },
			
			// npos
			{ nullptr, nullptr }
		};
		return startup(true, lib);
	}


	bool ml_lua::startup(bool openLibs, luaL_Reg const * userLib)
	{
		if (initialized()) return false;

		// lua allocator
		auto alloc = [](auto, void * p, size_t o, size_t n)
		{
			return memory_manager::reallocate(p, o, n);
		};

		// new state
		if (!(s_L = lua_newstate(alloc, nullptr)))
		{
			return false;
		}

		// builtin libs
		if (openLibs) { luaL_openlibs(s_L); }

		lua_getglobal(s_L, "_G");

		// custom libs
		if (userLib) { luaL_setfuncs(s_L, userLib, 0); }

		lua_pop(s_L, 1);

		return s_L;
	}

	bool ml_lua::shutdown()
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

	int32_t ml_lua::do_string(cstring const & value)
	{
		return (value && s_L) ? luaL_dostring(s_L, value) : 0;
	}

	int32_t ml_lua::do_string(pmr::string const & value)
	{
		return do_string(value.c_str());
	}

	int32_t ml_lua::do_file(fs::path const & path)
	{
		if (auto o{ FS::get_file_contents(path.string()) }; o && !o->empty())
		{
			return do_string(pmr::string{ o->begin(), o->end() });
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}