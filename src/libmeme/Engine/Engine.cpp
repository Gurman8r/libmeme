#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Renderer/RenderStates.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static engine::context * g_engine{};
	
	bool engine::is_initialized() noexcept
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

		// create window
		if (g_engine->m_window.create(
			g_engine->m_config.window_title,
			g_engine->m_config.window_display,
			g_engine->m_config.window_context,
			g_engine->m_config.window_hints
		))
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
		if (!g_engine) return false;

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
		if ML_LIKELY(!(g_engine->m_window & WindowHints_DoubleBuffered))
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
}