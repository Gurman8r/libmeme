#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Renderer/RenderStates.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_EMBED_LUA
#define ML_EMBED_PYTHON
#include <libmeme/Engine/Embed.hpp>

// global lua state
namespace ml::embed { static lua_State * g_L{}; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static engine::context * g_engine{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool engine::running() noexcept
	{
		return g_engine;
	}

	bool engine::create_context()
	{
		return !g_engine && (g_engine = new engine::context{});
	}

	bool engine::destroy_context()
	{
		if (!g_engine) return false;
		delete g_engine;
		return !(g_engine = nullptr);
	}

	engine::context * const engine::get_context() noexcept
	{
		return g_engine;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::startup()
	{
		if (!g_engine) return false;

		// LUA STARTUP
		if (!embed::g_L && !([&]()
		{
			// allocator
			auto alloc = [](auto, void * p, size_t o, size_t n) noexcept
			{
				return memory_manager::reallocate(p, o, n);
			};

			// libs
			static constexpr struct luaL_Reg lua_defaults[] =
			{
			luaL_Reg{ "exit", [](auto L) { engine::close(); return 0; } },
			luaL_Reg{ "print", [](auto L)
			{
				for (int32_t i = 1; i <= lua_gettop(L); ++i)
					std::cout << lua_tostring(L, i);
				return 0;
			} },
			luaL_Reg{ nullptr, nullptr },
			};

			if (embed::g_L = lua_newstate(alloc, nullptr))
			{
				luaL_openlibs(embed::g_L);
				
				lua_getglobal(embed::g_L, "_G");
				
				luaL_setfuncs(embed::g_L, lua_defaults, 0);
				
				lua_pop(embed::g_L, 1);
			}
			return embed::g_L;

		})()) return debug::log::error("engine failed starting lua");

		// PYTHON STARTUP
		if (!Py_IsInitialized() && !([&]()
		{
			// allocator
			static PyObjectArenaAllocator alloc
			{
				nullptr,
				[](auto, size_t s) noexcept
				{
					return pmr::get_default_resource()->allocate(s);
				},
				[](auto, void * p, size_t s) noexcept
				{
					return pmr::get_default_resource()->deallocate(p, s);
				}
			};
			
			PyObject_SetArenaAllocator(&alloc);
			
			Py_SetProgramName(g_engine->m_config.program_name.c_str());
			
			Py_SetPythonHome(g_engine->m_config.library_home.c_str());
			
			Py_InitializeEx(1);
			
			return Py_IsInitialized();

		})()) return debug::log::error("engine failed starting python");

		return true;
	}

	bool engine::shutdown()
	{
		if (!g_engine) return false;

		// PLUGINS SHUTDOWN
		g_engine->m_plugin_libs.for_each([](auto const &, plugin * p)
		{
			memory_manager::deallocate(p);
		});

		// WINDOW SHUTDOWN
		if (g_engine->m_window.is_open())
		{
			g_engine->m_window.destroy();

			window::terminate();
		}

		// PYTHON SHUTDOWN
		if (Py_IsInitialized()) { Py_FinalizeEx(); }

		// LUA SHUTDOWN
		if (embed::g_L) { lua_close(embed::g_L); embed::g_L = nullptr; }
		
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void engine::begin_loop()
	{
		// update delta time
		g_engine->m_io.delta_time = g_engine->m_loop_timer.stop().elapsed().count<float_t>();
		g_engine->m_loop_timer.start();

		// poll window events
		window::poll_events();
	}

	void engine::end_loop()
	{
		// increment frame count
		++g_engine->m_io.frame_count;

		// update fps tracker
		g_engine->m_io.frame_rate = g_engine->m_fps_tracker(g_engine->m_io.delta_time);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void engine::begin_draw()
	{
		// clear color
		g_engine->m_window.clear_color(colors::black);

		// viewport
		g_engine->m_window.viewport({ {}, g_engine->m_window.get_frame_size() });

		// default states
		constexpr render_states states{
		}; states();
	}

	void engine::end_draw()
	{
		// 
		if ML_LIKELY(!(g_engine->m_window.has_hint(window_hints_double_buffered)))
		{
			GL::flush();
		}
		else
		{
			g_engine->m_window.swap_buffers();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::load_plugin(filesystem::path const & path)
	{
		// check file name already loaded
		if (auto const file{ g_engine->m_plugin_files.insert(path.filename()) }; file.second)
		{
			// load library
			if (auto lib{ make_shared_library(*file.first) })
			{
				// load plugin
				if (auto const inst{ lib.call_function<plugin *>("ml_plugin_main") })
				{
					return (*g_engine->m_plugin_libs.insert(std::move(lib), *inst).second);
				}
			}
			g_engine->m_plugin_files.erase(file.first);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t engine::do_script(int32_t lang, pmr::string const & text)
	{
		if (!running() || text.empty()) return 0;
		switch (lang)
		{
		default: return 0;
		case embed::api::lua:
		{
			return luaL_dostring(embed::g_L, text.c_str());
		}
		case embed::api::python:
		{
			return PyRun_SimpleStringFlags(text.c_str(), nullptr);
		}
		}
	}

	int32_t engine::do_script(filesystem::path const & path)
	{
		if (!running() || !filesystem::exists(path)) return 0;
		switch (embed::api::ext_id(util::to_lower(path.extension().string())))
		{
		default: return 0;
		case embed::api::lua:
		{
			return luaL_dofile(embed::g_L, path.string().c_str());
		}
		case embed::api::python:
		{
			std::FILE * fp{ std::fopen(path.string().c_str(), "r") };
			return PyRun_SimpleFileExFlags(fp, path.string().c_str(), true, nullptr);
		}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}