#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/Python.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Engine/Script.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Renderer/RenderStates.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static engine::context * g_engine{};
	
	bool engine::initialized() noexcept
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
		if (!initialized()) return false;

		// start lua
		if (!ml_lua::startup())
		{
			return debug::log::error("engine failed initializing lua");
		}

		// start python
		if (!ml_python::startup(
			g_engine->m_config.program_name,
			path_to(g_engine->m_config.library_home)
		))
		{
			return debug::log::error("engine failed initializing python");
		}
		
		return true;
	}

	bool engine::shutdown()
	{
		if (!initialized()) return false;

		// cleanup plugins
		g_engine->m_plugin_libs.for_each([](auto const &, plugin * p)
		{
			memory_manager::deallocate(p);
		});

		// close window
		if (g_engine->m_window.is_open())
		{
			g_engine->m_window.destroy();

			window::terminate();
		}

		// shutdown scripting
		if (ml_python::initialized()) { ml_python::shutdown(); }
		if (ml_lua::initialized()) { ml_lua::shutdown(); }

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void engine::begin_loop()
	{
		g_engine->m_io.delta_time = g_engine->m_loop_timer.stop().elapsed().count<float_t>();

		g_engine->m_loop_timer.start();

		window::poll_events();
	}

	void engine::end_loop()
	{
		++g_engine->m_io.frame_count;

		g_engine->m_io.frame_rate = g_engine->m_fps_tracker(g_engine->m_io.delta_time);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void engine::begin_draw()
	{
		g_engine->m_window.clear_color(colors::black);

		g_engine->m_window.viewport({ {}, g_engine->m_window.get_frame_size() });

		constexpr render_states states{
		}; states();
	}

	void engine::end_draw()
	{
		if ML_UNLIKELY(g_engine->m_window & WindowFlags_DoubleBuffered)
		{
			g_engine->m_window.swap_buffers();
		}
		GL::flush();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::load_plugin(fs::path const & path)
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
}