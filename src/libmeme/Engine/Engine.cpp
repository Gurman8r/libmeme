#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Renderer/RenderStates.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/Embed.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	using file_list_t	= pmr::vector<fs::path>;
	using file_set_t	= ds::flat_set<fs::path>;
	using libraries_t	= ds::flat_map<shared_library, plugin *>;

	// engine context
	class engine::context final : non_copyable, trackable
	{
		friend class		engine								;
		engine::config		m_config		{}					; // public startup variables
		engine::io			m_io			{}					; // public runtime variables
		timer				m_main_timer	{ true }			; // main timer
		timer				m_loop_timer	{}					; // loop timer
		frame_tracker<120>	m_fps_tracker	{}					; // frame rate tracker
		render_window		m_window		{}					; // main window

		asset_manager		m_assets		{}					; // asset manager
		plugin_manager		m_plugins		{}					; // plugin manager
		script_manager		m_scripts		{}					; // script manager
	};

	static engine::context * g_engine{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::is_initialized() noexcept { return g_engine; }

	bool engine::create_context(json const & j)
	{
		if (g_engine)
		{
			return debug::log::error("engine is already initialized");
		}
		else if (!(g_engine = new engine::context{}))
		{
			return debug::log::error("failed initializing engine instance");
		}
		else
		{
			auto & cfg{ get_config() };

			cfg.command_line = { __argv, __argv + __argc };

			cfg.program_path = fs::current_path();

			cfg.program_name = fs::path{ __argv[0] }.filename();

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
		delete g_engine;
		return !(g_engine = nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::startup()
	{
		if (!g_engine) { return false; }

		// start scripting
		if (!g_engine->m_scripts.startup(
			g_engine->m_config.program_name,
			g_engine->m_config.library_home))
			return debug::log::error("engine failed starting python");

		// run setup script
		g_engine->m_scripts.do_file(path_to(get_config().setup_script));

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
		if (!g_engine) { return false; }

		// clear plugins
		g_engine->m_plugins.clear();

		// destroy window
		if (g_engine->m_window.is_open())
		{
			g_engine->m_window.destroy();

			window::terminate();
		}

		// shutdown python
		g_engine->m_scripts.shutdown();

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void engine::begin_loop()
	{
		ML_assert(g_engine);

		// update delta time
		g_engine->m_io.delta_time = g_engine->m_loop_timer.stop().elapsed().count<float_t>();
		g_engine->m_loop_timer.start();

		// update window
		window::poll_events();
	}

	void engine::begin_draw()
	{
		ML_assert(g_engine);

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
		ML_assert(g_engine);

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
		ML_assert(g_engine);

		// increment frame count
		++g_engine->m_io.frame_count;

		// update fps tracker
		g_engine->m_io.frame_rate = g_engine->m_fps_tracker(g_engine->m_io.delta_time);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	asset_manager & engine::get_assets() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_assets;
	}

	engine::config & engine::get_config() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_config;
	}

	engine::io & engine::get_io() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_io;
	}

	plugin_manager & engine::get_plugins() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_plugins;
	}

	script_manager & engine::get_scripts() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_scripts;
	}

	duration const & engine::get_time() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_main_timer.elapsed();
	}

	render_window & engine::get_window() noexcept
	{
		return g_engine->m_window;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}