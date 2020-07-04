#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_window::render_window() noexcept : window{}, m_dev{}
	{
	}

	render_window::render_window(window_settings const & ws, bool ic) noexcept : render_window{}
	{
		ML_assert(this->open(ws, ic));
	}

	render_window::~render_window() noexcept
	{
		gfx::device::destroy(m_dev.release());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool render_window::open(window_settings const & ws, bool ic)
	{
		// check already open
		if (is_open()) { return debug::error("render_window is already open"); }

		// open window
		if (!window::open(ws, ic))
		{
			return debug::error("failed opening render_window");
		}

		// create device
		if (m_dev.reset(gfx::device::create()); !m_dev)
		{
			return debug::error("failed creating device");
		}

		// validate version
		m_settings.ctxconfig.major = m_dev->get_info().major_version;
		m_settings.ctxconfig.minor = m_dev->get_info().minor_version;
		debug::info("using renderer version: {0}.{1}", m_settings.ctxconfig.major, m_settings.ctxconfig.minor);

		// create context
		m_dev->set_context(gfx::context::create(m_settings.ctxconfig));
		
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
			std::invoke(cmd);
		}

		return true;
	}

	void render_window::close()
	{
		gfx::device::destroy(m_dev.release());

		window::close();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}