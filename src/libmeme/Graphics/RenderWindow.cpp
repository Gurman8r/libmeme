#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool render_window::open(window_settings const & ws, bool install_callbacks)
	{
		// open window
		if (!window::open(ws, install_callbacks))
		{
			return debug::error("render_window failed opening window");
		}

		// validate version
		if (auto const & ctx{ gfx::device::get_context() })
		{
			m_settings.context.major = ctx->get_devinfo().major_version;
			m_settings.context.minor = ctx->get_devinfo().minor_version;
			debug::info("device api version: {0}.{1}", m_settings.context.major, m_settings.context.minor);
		}
		else
		{
			return debug::error("failed initializing device context");
		}


		// setup states
		for (auto const & cmd :
		{
			// alpha
			gfx::render_command::set_alpha_enabled(true),
			gfx::render_command::set_alpha_fn({ gfx::predicate_greater, 0.001f }),
			
			// blend
			gfx::render_command::set_blend_enabled(true),
			gfx::render_command::set_blend_color(colors::white),
			gfx::render_command::set_blend_eq({ gfx::function_add }),
			gfx::render_command::set_blend_fn({ gfx::factor_src_alpha, gfx::factor_one_minus_src_alpha }),
			
			// cull
			gfx::render_command::set_cull_enabled(true),
			gfx::render_command::set_cull_facet(gfx::facet_back),
			gfx::render_command::set_cull_order(gfx::order_ccw),

			// depth
			gfx::render_command::set_depth_enabled(true),
			gfx::render_command::set_depth_pr(gfx::predicate_less),
			gfx::render_command::set_depth_mask(true),
			gfx::render_command::set_depth_range({ 0.f, 1.f }),

			// stencil
			gfx::render_command::set_stencil_enabled(true),
			gfx::render_command::set_stencil_fn({ gfx::predicate_always, 0, 0xffffffff })
		})
		{
			std::invoke(cmd);
		}

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}