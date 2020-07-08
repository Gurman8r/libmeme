#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_window::render_window() noexcept : window{}, m_device{}
	{
	}

	render_window::render_window(window_settings const & ws) noexcept : render_window{}
	{
		ML_assert(open(ws));
	}

	render_window::~render_window() noexcept
	{
		gfx::device::destroy(m_device.release());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool render_window::open(window_settings const & ws)
	{
		// check already open
		if (is_open()) { return debug::error("render_window is already open"); }

		// open render_window
		if (!window::open(ws)) { return debug::error("failed opening render_window"); }

		// create device
		if (m_device.reset(gfx::device::create()); !m_device)
		{
			return debug::error("failed creating device");
		}

		// validate version
		m_ws.ctxconfig.major = m_device->get_info().major_version;
		m_ws.ctxconfig.minor = m_device->get_info().minor_version;
		debug::info("using renderer version: {0}.{1}", m_ws.ctxconfig.major, m_ws.ctxconfig.minor);

		// create context
		m_device->set_context(m_device->create_context(m_ws.ctxconfig));
		
		// setup states
		for (auto const & cmd :
		{
			// alpha
			gfx::render_command::set_alpha_enabled(true),
			gfx::render_command::set_alpha_mode({ gfx::predicate_greater, 0.001f }),

			// blend
			gfx::render_command::set_blend_enabled(true),
			gfx::render_command::set_blend_color(colors::white),
			gfx::render_command::set_blend_mode({ gfx::equation_add, gfx::factor_src_alpha, gfx::factor_one_minus_src_alpha }),

			// cull
			gfx::render_command::set_cull_enabled(true),
			gfx::render_command::set_cull_mode({ gfx::facet_back, gfx::order_ccw }),

			// depth
			gfx::render_command::set_depth_enabled(true),
			gfx::render_command::set_depth_mode({ gfx::predicate_less, { 0.f, 1.f } }),

			// stencil
			gfx::render_command::set_stencil_enabled(true),
			gfx::render_command::set_stencil_mode({ gfx::predicate_always, 0, 0xffffffff }),
		})
		{
			std::invoke(cmd, m_device->get_context().get());
		}

		return true;
	}

	void render_window::close()
	{
		gfx::device::destroy(m_device.release());

		window::close();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}