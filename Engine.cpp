#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/System/EventBus.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static engine::engine_context * g_engine{};
	
	struct engine::engine_context final : non_copyable, trackable, event_listener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class		engine		;
		json				m_config	; // config
		time_manager		m_time		; // timers
		file_manager		m_fs		; // files
		render_window		m_window	; // window
		gui_manager			m_gui		; // gui
		plugin_manager		m_plugins	; // plugins
		script_manager		m_scripts	; // scripts

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		engine_context(json const & j, allocator_type alloc) noexcept
			: m_config	{ json{ j } }
			, m_time	{ j, alloc }
			, m_fs		{ j, alloc }
			, m_window	{ j["window"].get<window_settings>() }
			, m_gui		{ m_window, j, alloc }
			, m_scripts	{ j, alloc }
			, m_plugins	{ j, alloc }
		{
			event_bus::add_listener< begin_loop_event	>(this);
			event_bus::add_listener< begin_draw_event	>(this);
			event_bus::add_listener< begin_gui_event	>(this);
			event_bus::add_listener< draw_gui_event		>(this);
			event_bus::add_listener< end_gui_event		>(this);
			event_bus::add_listener< end_draw_event		>(this);
			event_bus::add_listener< end_loop_event		>(this);
		}

		~engine_context() noexcept override
		{
			m_gui.main_menu_bar().menus.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_event(event const & ev) override
		{
			switch (ev.ID)
			{
			case hashof_v<begin_loop_event>: {
				window::poll_events();
			} break;

			case hashof_v<begin_draw_event>: {
				for (auto const & cmd :
				{
					gfx::render_command::set_clear_color(colors::black),

					gfx::render_command::clear(gfx::clear_color),
					
					gfx::render_command::set_viewport(m_window.get_framebuffer_size()),
				})
				{
					std::invoke(cmd, m_window.get_render_context().get());
				}
			} break;

			case hashof_v<begin_gui_event>: {
				m_gui.begin_frame();
			} break;

			case hashof_v<draw_gui_event>: {
				m_gui.draw_default();
			} break;

			case hashof_v<end_gui_event>: {
				m_gui.end_frame();
			} break;

			case hashof_v<end_draw_event>: {
				if (m_window.get_hints() & window_hints_doublebuffer)
				{
					window::swap_buffers(m_window.get_handle());
				}
			} break;

			case hashof_v<end_loop_event>: {
			} break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::is_initialized() noexcept
	{
		return (bool)g_engine;
	}

	bool engine::initialize(json const & j, allocator_type alloc) noexcept
	{
		if (is_initialized()) { return debug::error("engine is already initialized"); }
		else
		{
			debug::info("initializing engine context...");

			ML_assert((g_engine = new engine_context{ j, alloc }));

			for (auto const & scr : j["path"]["scripts"])
			{
				scripts().do_file(fs().path2(scr));
			}

			return is_initialized();
		}
	}

	bool engine::finalize() noexcept
	{
		if (!is_initialized()) { return debug::error("engine is not initialized"); }
		else
		{
			debug::info("finalizing engine context...");

			delete g_engine; g_engine = nullptr;

			return !is_initialized();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	json & engine::config() noexcept
	{
		ML_assert("engine::config" && g_engine);
		return g_engine->m_config;
	}

	file_manager & engine::fs() noexcept
	{
		ML_assert("engine::fs" && g_engine);
		return g_engine->m_fs;
	}

	gui_manager & engine::gui() noexcept
	{
		ML_assert("engine::gui" && g_engine);
		return g_engine->m_gui;
	}

	plugin_manager & engine::plugins() noexcept
	{
		ML_assert("engine::plugins" && g_engine);
		return g_engine->m_plugins;
	}

	script_manager & engine::scripts() noexcept
	{
		ML_assert("engine::scripts" && g_engine);
		return g_engine->m_scripts;
	}

	time_manager & engine::time() noexcept
	{
		ML_assert("engine::time" && g_engine);
		return g_engine->m_time;
	}

	render_window & engine::window() noexcept
	{
		ML_assert("engine::window" && g_engine);
		return g_engine->m_window;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}