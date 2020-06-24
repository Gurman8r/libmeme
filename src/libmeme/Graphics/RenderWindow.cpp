#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_window::render_window() noexcept : window{}, m_context{}
	{
	}

	render_window::render_window(window_settings const & ws, bool install_callbacks) noexcept : render_window{}
	{
		(void)this->open(ws, install_callbacks);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool render_window::open(window_settings const & ws, bool install_callbacks)
	{
		// open window
		if (!window::open(ws, install_callbacks))
		{
			return debug::error("failed opening window");
		}

		// create device context
		if (!m_context.reset(gfx::device::create_context(ws.context)))
		{
			return debug::error("failed creating device context");
		}

		// validate version
		m_settings.context.major = get_device_info().major_version;
		m_settings.context.minor = get_device_info().minor_version;
		debug::info("using renderer version: {0}.{1}", m_settings.context.major, m_settings.context.minor);
		
		// setup render states
		for (gfx::command const & cmd :
		{
			// alpha
			gfx::render_command::set_alpha_enabled(true),
			gfx::render_command::set_alpha_mode({ gfx::predicate_greater, 0.001f }),

			// blend
			gfx::render_command::set_blend_enabled(true),
			gfx::render_command::set_blend_color(colors::white),
			gfx::render_command::set_blend_mode({ gfx::function_add, gfx::factor_src_alpha, gfx::factor_one_minus_src_alpha }),

			// cull
			gfx::render_command::set_cull_enabled(true),
			gfx::render_command::set_cull_mode({ gfx::facet_back, gfx::order_ccw }),

			// depth
			gfx::render_command::set_depth_enabled(true),
			gfx::render_command::set_depth_mode({ gfx::predicate_less, { 0.f, 1.f } }),
			gfx::render_command::set_depth_write(true),

			// stencil
			gfx::render_command::set_stencil_enabled(true),
			gfx::render_command::set_stencil_mode({ gfx::predicate_always, 0, 0xffffffff }),
		})
		{
			std::invoke(cmd);
		}

		return true;
	}

	void render_window::close()
	{
		m_context.reset();

		window::close();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}