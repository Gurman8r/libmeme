#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Renderer/RenderStates.hpp>

#ifndef ML_EMBED_LUA
#define ML_EMBED_LUA
#endif
#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/ScriptableObject.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	using file_list_t	= pmr::vector<filesystem::path>;
	using file_set_t	= ds::flat_set<filesystem::path>;
	using libraries_t	= ds::flat_map<struct shared_library, struct plugin *>;

	// engine context
	class engine::context final : trackable, non_copyable
	{
		friend class		engine						;
		engine::config		m_config		{}			; // public startup variables
		engine::runtime		m_runtime		{}			; // public runtime variables
		timer				m_main_timer	{ true }	; // main timer
		timer				m_loop_timer	{}			; // loop timer
		frame_tracker<120>	m_fps_tracker	{}			; // frame rate tracker
		render_window		m_window		{}			; // main window
		file_set_t			m_plugin_files	{}			; // plugin filenames
		libraries_t			m_plugin_libs	{}			; // plugin instances
		lua_State *			m_lua			{}			; // lua state
	};

	static engine::context * g_engine{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::is_initialized() noexcept
	{
		return g_engine;
	}

	bool engine::create_context(json const & j)
	{
		if (is_initialized() || !(g_engine = new engine::context{}))
		{
			return debug::log::error("engine is already initialized");
		}
		else
		{
			auto & cfg{ get_config() };

			cfg.arguments = { ML_argv, ML_argv + ML_argc };

			cfg.program_path = filesystem::current_path();

			cfg.program_name = filesystem::path{ ML_argv[0] }.filename();

			j["content_home"].get_to(cfg.content_home);

			j["library_home"].get_to(cfg.library_home);

			j["setup_script"].get_to(cfg.setup_script);

			j["window_settings"].get_to(cfg.window_settings);

			return g_engine;
		}
	}

	bool engine::destroy_context()
	{
		if (!g_engine) { return false; }
		else
		{
			delete g_engine;
			return !(g_engine = nullptr);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	engine::config & engine::get_config() noexcept
	{
		ML_assert(is_initialized());
		return (*g_engine).m_config;
	}

	engine::runtime & engine::get_runtime() noexcept
	{
		ML_assert(is_initialized());
		return (*g_engine).m_runtime;
	}

	duration const & engine::get_time() noexcept
	{
		ML_assert(is_initialized());
		return (*g_engine).m_main_timer.elapsed();
	}

	render_window & engine::get_window() noexcept
	{
		ML_assert(is_initialized());
		return (*g_engine).m_window;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::startup()
	{
		if (!is_initialized()) { return false; }

		// start lua
		if (!g_engine->m_lua && !([&]()
		{
			auto alloc = [](auto, void * p, size_t o, size_t n) noexcept
			{
				return memory_manager::reallocate(p, o, n);
			};
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
			if (g_engine->m_lua = lua_newstate(alloc, nullptr))
			{
				luaL_openlibs(g_engine->m_lua);
				lua_getglobal(g_engine->m_lua, "_G");
				luaL_setfuncs(g_engine->m_lua, lua_defaults, 0);
				lua_pop(g_engine->m_lua, 1);
			}
			return (*g_engine).m_lua;

		})()) return debug::log::error("engine failed starting lua");

		// start python
		if (!Py_IsInitialized() && !([&]()
		{
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

		// run setup script
		do_file(path_to(get_config().setup_script));

		// create window
		if (g_engine->m_window.create(get_config().window_settings))
		{
			window::install_default_callbacks(&g_engine->m_window);
		}
		else
		{
			return debug::log::error("engine failed creating window");
		}

		return true;
	}

	bool engine::shutdown()
	{
		if (!is_initialized()) { return false; }

		// shutdown plugins
		g_engine->m_plugin_libs.for_each([](auto const &, plugin * p)
		{
			memory_manager::deallocate(p);
		});

		// destroy window
		if (g_engine->m_window.is_open())
		{
			g_engine->m_window.destroy();

			window::terminate();
		}

		// shutdown python
		if (Py_IsInitialized()) { Py_FinalizeEx(); }

		// shutdown lua
		if (g_engine->m_lua) { lua_close(g_engine->m_lua); g_engine->m_lua = nullptr; }
		
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void engine::begin_loop()
	{
		// update delta time
		g_engine->m_runtime.delta_time = g_engine->m_loop_timer.stop().elapsed().count<float_t>();
		g_engine->m_loop_timer.start();

		window::poll_events();
	}

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
		if ML_LIKELY(!(g_engine->m_window.has_hint(window_hints_double_buffered)))
		{
			GL::flush();
		}
		else
		{
			g_engine->m_window.swap_buffers();
		}
	}

	void engine::end_loop()
	{
		// increment frame count
		++g_engine->m_runtime.frame_count;

		// update fps tracker
		g_engine->m_runtime.frame_rate = g_engine->m_fps_tracker(g_engine->m_runtime.delta_time);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::load_plugin(filesystem::path const & path)
	{
		if (path.empty()) return false;

		// check file name already loaded
		if (auto const file{ g_engine->m_plugin_files.insert(path.filename()) }; file.second)
		{
			// load library
			if (auto lib{ make_shared_library(*file.first) })
			{
				// load plugin
				if (auto const ptr{ lib.call_function<plugin *>("ml_plugin_main") })
				{
					return (*g_engine->m_plugin_libs.insert(std::move(lib), *ptr).second);
				}
			}
			g_engine->m_plugin_files.erase(file.first);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t engine::do_string(int32_t lang, pmr::string const & text)
	{
		if (!is_initialized() || text.empty())
			return 0;
		switch (lang)
		{
		case embed::api::lua:
		{
			return luaL_dostring(g_engine->m_lua, text.c_str());
		}
		case embed::api::python:
		{
			return PyRun_SimpleStringFlags(text.c_str(), nullptr);
		}
		}
		return 0;
	}

	int32_t engine::do_file(filesystem::path const & path)
	{
		if (!is_initialized() || !filesystem::exists(path))
			return 0;
		switch (embed::api::ext_id(util::to_lower(path.extension().string())))
		{
		case embed::api::lua:
		{
			return luaL_dofile(g_engine->m_lua, path.string().c_str()); 
		}
		case embed::api::python:
		{
			auto file{ std::fopen(path.string().c_str(), "r") };
			return PyRun_SimpleFileExFlags(file, path.string().c_str(), true, nullptr);
		}
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}