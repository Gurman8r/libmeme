#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_window::render_window() noexcept : window{}, m_dev{}, m_ctx{}
	{
	}

	render_window::render_window(window_settings const & ws) noexcept : render_window{}
	{
		ML_assert(open(ws));
	}

	render_window::~render_window() noexcept
	{
		gfx::render_device::destroy(m_dev.release());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool render_window::open(window_settings const & ws)
	{
		// check already open
		if (is_open()) { return debug::error("render_window is already open"); }

		// open render_window
		if (!window::open(ws)) { return debug::error("failed opening render_window"); }

		// create device
		if (m_dev.reset(gfx::render_device::create()); !m_dev)
		{
			return debug::error("failed creating device");
		}

		// create context
		m_dev->set_context(m_ctx = m_dev->create_context(ws.context));
		
		// setup states
		for (auto const & cmd :
		{
			// alpha state
			gfx::render_command::set_alpha_state
			({
				true,
				gfx::predicate_greater,
				0.001f
			}),

			// blend state
			gfx::render_command::set_blend_state
			({
				true,
				colors::white,
				gfx::equation_add,
				gfx::factor_src_alpha,
				gfx::factor_one_minus_src_alpha
			}),

			// cull state
			gfx::render_command::set_cull_state
			({
				false,
				gfx::facet_back,
				gfx::order_ccw
			}),

			// depth state
			gfx::render_command::set_depth_state
			({
				true,
				gfx::predicate_less,
				{ 0.f, 1.f }
			}),

			// stencil state
			gfx::render_command::set_stencil_state
			({
				true,
				gfx::predicate_always,
				0,
				0xffffffff
			}),
		})
		gfx::execute(cmd, m_ctx);

		return true;
	}

	void render_window::close()
	{
		gfx::render_device::destroy(m_dev.release());

		window::close();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}