#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Platform/SharedLibrary.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	class engine::engine_context final : trackable, non_copyable
	{
		friend class		engine		;
		json				m_config	; // config
		file_manager		m_fs		; // files
		gui_manager			m_gui		; // gui
		plugin_manager		m_plugins	; // plugins
		script_manager		m_scripts	; // scripts
		time_manager		m_time		; // timers
		render_window		m_window	; // window

		engine_context(json const & j, allocator_type alloc) noexcept
			: m_config	{ json{ j } }
			, m_fs		{ j, alloc }
			, m_gui		{ j, alloc }
			, m_plugins	{ j, alloc }
			, m_scripts	{ j, alloc }
			, m_time	{ j, alloc }
			, m_window	{}
		{
		}
	};

	static engine::engine_context * g_engine{}; // global engine context

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::is_initialized() noexcept { return (g_engine != nullptr); }

	bool engine::initialize(json const & j, allocator_type alloc) noexcept
	{
		// already initialized
		if (is_initialized()) { return debug::error("engine is already initialized"); }

		// create context
		debug::info("creating engine context...");
		if (!(g_engine = new engine_context{ j, alloc }))
		{
			return debug::error("failed creating engine context");
		}

		// initialize windows
		debug::info("initializing windows...");
		if (!window::initialize())
		{
			return debug::error("failed initializing windows");
		}

		// initialize scripting
		debug::info("initializing scripts...");
		if (!g_engine->m_scripts.initialize())
		{
			return debug::error("failed initializing scripts");
		}

		// execute setup script
		if (g_engine->m_config.contains("setup_script"))
		{
			debug::info("executing setup script...");
			g_engine->m_scripts.do_file(
				g_engine->m_fs.path2(
					g_engine->m_config["setup_script"]));
		}

		return is_initialized();
	}

	bool engine::finalize() noexcept
	{
		// not initialized
		if (!is_initialized()) { return debug::error("engine is not initialized"); }

		// FIXME: need to clear menus before plugins because menu code can live inside plugins
		g_engine->m_gui.main_menu_bar().menus.clear();

		// clear plugins
		g_engine->m_plugins.clear();

		// finalize gui
		(void)g_engine->m_gui.finalize();

		// finalize scripting
		(void)g_engine->m_scripts.finalize();

		// destroy window
		g_engine->m_window.destroy();

		// finalize window backend
		window::finalize();
		
		// destroy context
		delete g_engine; g_engine = nullptr;
		return !is_initialized();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	json & engine::config() noexcept
	{
		ML_assert(g_engine && "engine is not initialized");
		return g_engine->m_config;
	}

	file_manager & engine::fs() noexcept
	{
		ML_assert(g_engine && "engine is not initialized");
		return g_engine->m_fs;
	}

	gui_manager & engine::gui() noexcept
	{
		ML_assert(g_engine && "engine is not initialized");
		return g_engine->m_gui;
	}

	plugin_manager & engine::plugins() noexcept
	{
		ML_assert(g_engine && "engine is not initialized");
		return g_engine->m_plugins;
	}

	script_manager & engine::scripts() noexcept
	{
		ML_assert(g_engine && "engine is not initialized");
		return g_engine->m_scripts;
	}

	time_manager & engine::time() noexcept
	{
		ML_assert(g_engine && "engine is not initialized");
		return g_engine->m_time;
	}

	render_window & engine::window() noexcept
	{
		ML_assert(g_engine && "engine is not initialized");
		return g_engine->m_window;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}