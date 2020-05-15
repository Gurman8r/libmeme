#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Renderer/Renderer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static engine::engine_context * g_engine{};
	
	class engine::engine_context final : trackable, non_copyable, event_listener
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
			event_system::add_listener<	begin_loop_event>(this);
			event_system::add_listener<	begin_draw_event>(this);
			event_system::add_listener<	begin_gui_event	>(this);
			event_system::add_listener<	draw_gui_event	>(this);
			event_system::add_listener<	end_gui_event	>(this);
			event_system::add_listener<	end_draw_event	>(this);
			event_system::add_listener<	end_loop_event	>(this);
		}

		void on_event(event const & ev) override
		{
			switch (ev.ID)
			{
			case hashof_v<begin_loop_event>:
				m_window.poll_events();
				break;

			case hashof_v<begin_draw_event>:
				render_command::set_clear_color(colors::black)();
				render_command::clear(gl::clear_flags_color | gl::clear_flags_depth)();
				render_command::set_viewport(m_window.get_framebuffer_size())();
				break;

			case hashof_v<begin_gui_event>:
				m_gui.new_frame();
				break;

			case hashof_v<draw_gui_event>:
				m_gui.draw_default();
				break;

			case hashof_v<end_gui_event>:
				m_gui.render_frame();
				break;

			case hashof_v<end_draw_event>:
				m_window.swap_buffers();
				break;

			case hashof_v<end_loop_event>:
				break;
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::is_initialized() noexcept
	{
		return (g_engine != nullptr);
	}

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
		if (!g_engine->m_window.initialize())
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

		// success
		return is_initialized();
	}

	bool engine::finalize() noexcept
	{
		// not initialized
		if (!is_initialized()) { return debug::error("engine is not initialized"); }

		// need to clear menus before plugins because menu code can live inside plugins
		g_engine->m_gui.main_menu_bar().menus.clear();

		// clear plugins
		g_engine->m_plugins.clear();

		// finalize gui
		(void)g_engine->m_gui.finalize();

		// finalize scripting
		(void)g_engine->m_scripts.finalize();

		// destroy window
		g_engine->m_window.destroy();

		// finalize windows
		g_engine->m_window.finalize();
		
		// destroy context
		delete g_engine; g_engine = nullptr;
		return !is_initialized();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	json & engine::config() noexcept
	{
		ML_assert(g_engine && "engine::config");
		return g_engine->m_config;
	}

	file_manager & engine::fs() noexcept
	{
		ML_assert(g_engine && "engine::fs");
		return g_engine->m_fs;
	}

	gui_manager & engine::gui() noexcept
	{
		ML_assert(g_engine && "engine::gui");
		return g_engine->m_gui;
	}

	plugin_manager & engine::plugins() noexcept
	{
		ML_assert(g_engine && "engine::plugins");
		return g_engine->m_plugins;
	}

	script_manager & engine::scripts() noexcept
	{
		ML_assert(g_engine && "engine::scripts");
		return g_engine->m_scripts;
	}

	time_manager & engine::time() noexcept
	{
		ML_assert(g_engine && "engine::time");
		return g_engine->m_time;
	}

	render_window & engine::window() noexcept
	{
		ML_assert(g_engine && "engine::window");
		return g_engine->m_window;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}