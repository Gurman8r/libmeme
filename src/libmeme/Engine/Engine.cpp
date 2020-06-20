#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>

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
			case hashof_v<begin_loop_event>: {
				m_window.poll_events();
			} break;

			case hashof_v<begin_draw_event>: {
				for (auto const & cmd :
				{
					gfx::render_command::set_clear_color(colors::black),
					gfx::render_command::clear(gfx::buffer_bit_all),
					gfx::render_command::set_viewport(m_window.get_framebuffer_size()),
				})
				{
					std::invoke(cmd);
				}
			} break;

			case hashof_v<begin_gui_event>: {
				m_gui.new_frame();
			} break;

			case hashof_v<draw_gui_event>: {
				m_gui.draw_default();
			} break;

			case hashof_v<end_gui_event>: {
				m_gui.render_frame();
			} break;

			case hashof_v<end_draw_event>: {
				if ML_UNLIKELY(m_window.get_hint(window_hints_doublebuffer))
				{
					m_window.swap_buffers();
				}
			} break;

			case hashof_v<end_loop_event>: {
			} break;
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool engine::is_initialized() noexcept { return (bool)g_engine; }

	bool engine::initialize(json const & j, allocator_type alloc) noexcept
	{
		if (g_engine) { return debug::error("engine is already initialized"); }
		else
		{
			if (debug::info("creating engine context...")
			; !(g_engine = new engine_context{ j, alloc }))
			{
				return debug::error("failed creating engine context");
			}

			return g_engine;
		}
	}

	bool engine::finalize() noexcept
	{
		if (!g_engine) { return debug::error("engine is not initialized"); }
		else
		{
			// FIXME: need to manually clear gui callbacks before plugins are destroyed
			// callbacks can live inside plugins' memory and will crash if not cleared first
			g_engine->m_gui.main_menu_bar().menus.clear();

			delete g_engine;

			return !(g_engine = nullptr);
		}
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