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
		if (auto const & ctx{ gl::device::get_context() })
		{
			m_settings.context.major = ctx->get_devinfo().major_version;
			m_settings.context.minor = ctx->get_devinfo().minor_version;
			debug::info("using device version {0}.{1}", m_settings.context.major, m_settings.context.minor);
		}
		else
		{
			debug::error("failed initializing device context");
		}


		// setup states
		for (auto const & cmd :
		{
			// alpha
			gl::render_command::set_alpha_enabled(true),
			gl::render_command::set_alpha_fn({ gl::predicate_greater, 0.001f }),
			
			// blend
			gl::render_command::set_blend_enabled(true),
			gl::render_command::set_blend_color(colors::white),
			gl::render_command::set_blend_eq({ gl::function_add }),
			gl::render_command::set_blend_fn({ gl::factor_src_alpha, gl::factor_one_minus_src_alpha }),
			
			// cull
			gl::render_command::set_cull_enabled(true),
			gl::render_command::set_cull_facet(gl::facet_back),
			gl::render_command::set_cull_order(gl::order_ccw),

			// depth
			gl::render_command::set_depth_enabled(true),
			gl::render_command::set_depth_pr(gl::predicate_less),
			gl::render_command::set_depth_mask(true),
			gl::render_command::set_depth_range({ 0.f, 1.f }),

			// stencil
			gl::render_command::set_stencil_enabled(true),
			gl::render_command::set_stencil_fn({})
		})
		{
			std::invoke(cmd);
		}

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}