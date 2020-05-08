#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Platform/SharedLibrary.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// engine context
	class engine::engine_context final : trackable, non_copyable
	{
		friend class		engine		;
		json				m_config	; // config
		file_manager		m_fs		; // files
		gui_manager			m_gui		; // gui
		plugin_manager		m_plugins	; // plugins
		script_manager		m_scripts	; // scripts
		time_manager		m_time		; // time
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

	static engine::engine_context * g_engine{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::is_initialized() noexcept
	{
		return (g_engine != nullptr);
	}

	bool engine::create_context(json const & j, allocator_type alloc) noexcept
	{
		if (is_initialized()) { return debug::error("engine context is already initialized"); }

		g_engine = new engine_context{ j, alloc };

		return is_initialized();
	}

	bool engine::destroy_context() noexcept
	{
		if (!is_initialized()) { return debug::error("engine context is not initialized"); }
		
		delete g_engine;

		g_engine = nullptr;
		
		return !is_initialized();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::startup() noexcept
	{
		if (!is_initialized()) { return debug::error("engine context is not initialized"); }

		// initialize window backend
		debug::info("initialize window backend");
		if (!window::backend_initialize())
		{
			return debug::error("failed starting window backend");
		}

		// startup scripting
		debug::info("startup scripting");
		if (!scripts().startup())
		{
			return debug::error("failed initializing scripting");
		}

		// execute setup script
		if (config().contains("setup_script"))
		{
			debug::info("execute setup");
			scripts().do_file(fs().path2(config()["setup_script"]));
		}

		return true;
	}

	bool engine::shutdown() noexcept
	{
		if (!is_initialized()) { return debug::error("engine context is not initialized"); }

		// FIXME: need to clear menus before plugins because menu code can live inside plugins
		gui().main_menu_bar().menus.clear();

		// clear plugins
		plugins().clear();

		// shutdown gui
		if (!gui().shutdown()) {}

		// shutdown scripting
		if (!scripts().shutdown()) {}

		// destroy window
		window().destroy();

		// finalize window backend
		window::backend_finalize();

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	json & engine::config() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_config;
	}

	file_manager & engine::fs() noexcept
	{
		ML_assert(g_engine);
		return g_engine->m_fs;
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

	time_manager & engine::time() noexcept
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