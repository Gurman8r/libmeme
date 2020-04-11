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
	
	// engine context
	class engine::engine_context final : non_copyable, trackable
	{
		friend class		engine								;
		asset_manager		m_assets		{}					; // asset manager
		engine_config		m_config		{}					; // public startup variables
		engine_io			m_io			{}					; // public runtime variables
		plugin_manager		m_plugins		{}					; // plugin manager
		script_manager		m_scripts		{}					; // script manager
		game_time			m_time			{}					; // game time
		render_window		m_window		{}					; // main window
	};

	static engine::engine_context * g_engine{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::is_initialized() noexcept { return g_engine; }

	bool engine::create_context(json const & j)
	{
		if (g_engine)
		{
			return debug::log::error("engine is already initialized");
		}
		else if (!(g_engine = new engine_context{}))
		{
			return debug::log::error("failed initializing engine context");
		}
		else
		{
			auto & cfg{ config() };

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
		g_engine->m_scripts.do_file(path_to(config().setup_script));

		// create window
		if (g_engine->m_window.create(config().window_settings))
		{
			base_window::install_default_callbacks(&g_engine->m_window);
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

			base_window::terminate();
		}

		// shutdown python
		g_engine->m_scripts.shutdown();

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void engine::begin_loop()
	{
		ML_assert(g_engine);

		g_engine->m_time.begin_loop();

		base_window::poll_events();
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

		if ML_UNLIKELY(g_engine->m_window.has_hint(window_hints_double_buffered))
		{
			g_engine->m_window.swap_buffers();
		}
		else
		{
			GL::flush();
		}
	}

	void engine::end_loop()
	{
		ML_assert(g_engine);

		g_engine->m_time.end_loop();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	asset_manager & engine::assets() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_assets;
	}

	engine::engine_config & engine::config() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_config;
	}

	engine::engine_io & engine::io() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_io;
	}

	plugin_manager & engine::plugins() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_plugins;
	}

	script_manager & engine::scripts() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_scripts;
	}

	game_time & engine::time() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_time;
	}

	render_window & engine::window() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_window;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}