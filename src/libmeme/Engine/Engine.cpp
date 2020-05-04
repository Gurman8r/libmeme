#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Platform/SharedLibrary.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// engine context
	class engine::engine_context final : non_copyable, trackable
	{
		friend class		engine		;
		asset_manager		m_assets	; // assets
		file_manager		m_fs		; // files
		gui_manager			m_gui		; // gui
		object_manager		m_objects	; // objects
		plugin_manager		m_plugins	; // plugins
		script_manager		m_scripts	; // scripts
		time_manager		m_time		; // time
		render_window		m_window	; // window

		engine_context(json const & j, allocator_type const & alloc) noexcept
			: m_assets	{ j, alloc }
			, m_fs		{ j, alloc }
			, m_gui		{ j, alloc }
			, m_objects	{ j, alloc }
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

	bool engine::create_context(json const & j, allocator_type const & alloc) noexcept
	{
		if (is_initialized()) { return debug::log::error("engine is already initialized"); }

		g_engine = new engine_context{ j, alloc };

		return is_initialized();
	}

	bool engine::destroy_context() noexcept
	{
		if (!is_initialized()) { return debug::log::error("engine is not initialized"); }
		
		delete g_engine;

		g_engine = nullptr;
		
		return !is_initialized();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::startup() noexcept
	{
		if (!is_initialized()) { return debug::log::error("engine is not initialized"); }

		// startup scripting
		if (!g_engine->m_scripts.startup()) { return debug::log::error("failed initializing scripting"); }

		return true;
	}

	bool engine::shutdown() noexcept
	{
		if (!is_initialized()) { return debug::log::error("engine is not initialized"); }

		// FIXME: need to clear menus before clearing plugins
		g_engine->m_gui.main_menu_bar.menus.clear();

		// shutdown plugins
		g_engine->m_plugins.clear();

		// shutdown gui
		if (!g_engine->m_gui.shutdown()) {}

		// shutdown window
		if (!g_engine->m_window.close()) {}

		// shutdown scripting
		if (!g_engine->m_scripts.shutdown()) {}

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	asset_manager & engine::assets() noexcept
	{
		ML_assert(is_initialized());
		return g_engine->m_assets;
	}

	file_manager & engine::fs() noexcept
	{
		ML_assert(is_initialized());
		return g_engine->m_fs;
	}

	object_manager & engine::objects() noexcept
	{
		ML_assert(is_initialized());
		return g_engine->m_objects;
	}

	gui_manager & engine::gui() noexcept
	{
		ML_assert(is_initialized());
		return g_engine->m_gui;
	}

	plugin_manager & engine::plugins() noexcept
	{
		ML_assert(is_initialized());
		return g_engine->m_plugins;
	}

	script_manager & engine::scripts() noexcept
	{
		ML_assert(is_initialized());
		return g_engine->m_scripts;
	}

	time_manager & engine::time() noexcept
	{
		ML_assert(is_initialized());
		return g_engine->m_time;
	}

	render_window & engine::window() noexcept
	{
		ML_assert(is_initialized());
		return g_engine->m_window;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}