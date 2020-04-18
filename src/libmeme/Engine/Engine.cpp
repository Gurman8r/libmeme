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
		friend class		engine		;
		asset_manager		m_assets	; // asset manager
		engine_config		m_cfg		; // engine config
		object_manager		m_objects	; // object manager
		gui_manager			m_gui		; // gui manager
		plugin_manager		m_plugins	; // plugin manager
		script_manager		m_scripts	; // script manager
		game_time			m_time		; // time manager
		render_window		m_window	; // render window

		engine_context(json const & j, allocator_type const & alloc)
			: m_assets	{ j, alloc }
			, m_cfg		{}
			, m_objects	{ j, alloc }
			, m_gui		{ j, alloc }
			, m_plugins	{ j, alloc }
			, m_scripts	{ j, alloc }
			, m_time	{ j, alloc }
			, m_window	{}
		{
			m_cfg.command_line = { __argv, __argv + __argc };

			m_cfg.program_path = fs::current_path();

			m_cfg.program_name = fs::path{ __argv[0] }.filename();

			j["content_home"].get_to(m_cfg.content_home);

			j["library_home"].get_to(m_cfg.library_home);

			j["startup_script"].get_to(m_cfg.startup_script);
		}
	};

	static engine::engine_context * g_engine{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::is_initialized() noexcept { return g_engine; }

	bool engine::create_context(json const & j, allocator_type const & alloc)
	{
		if (g_engine)
		{
			return debug::log::error("engine is already initialized");
		}
		else if (!(g_engine = new engine_context{ j, alloc }))
		{
			return debug::log::error("failed initializing engine context");
		}
		else
		{
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
			g_engine->m_cfg.program_name,
			g_engine->m_cfg.library_home))
			return debug::log::error("engine failed starting python");

		// run setup script
		g_engine->m_scripts.do_file(path_to(g_engine->m_cfg.startup_script));

		return true;
	}

	bool engine::shutdown()
	{
		if (!g_engine) { return false; }

		// shutdown gui
		if (!g_engine->m_gui.shutdown()) { return false; }

		// clear plugins
		g_engine->m_plugins.clear();

		// destroy window
		if (g_engine->m_window.is_open())
		{
			g_engine->m_window.close();
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

		g_engine->m_time.begin_loop();

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

		g_engine->m_window.swap_buffers();

		GL::flush();
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

	engine_config & engine::config() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_cfg;
	}

	object_manager & engine::objects() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_objects;
	}

	gui_manager & engine::gui() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_gui;
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