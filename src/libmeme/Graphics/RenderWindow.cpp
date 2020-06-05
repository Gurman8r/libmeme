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

		// initialize device context
		if (auto const & ctx{ gl::device::get_context() })
		{
			// validate version
			m_settings.context.major = ctx->get_info().major_version;
			m_settings.context.minor = ctx->get_info().minor_version;
		}
		else
		{
			return debug::error("render_window failed initializing device");
		}


		// setup states
		for (auto const & cmd :
		{
			// alpha
			gl::command::set_alpha_enabled(true),
			gl::command::set_alpha_fn({ gl::predicate_greater, 0.001f }),
			
			// blend
			gl::command::set_blend_enabled(true),
			gl::command::set_blend_color(colors::white),
			gl::command::set_blend_eq({ gl::function_add }),
			gl::command::set_blend_fn({ gl::factor_src_alpha, gl::factor_one_minus_src_alpha }),
			
			// cull
			gl::command::set_cull_enabled(true),
			gl::command::set_cull_facet(gl::facet_back),
			gl::command::set_cull_order(gl::order_ccw),

			// depth
			gl::command::set_depth_enabled(true),
			gl::command::set_depth_pr(gl::predicate_less),
			gl::command::set_depth_mask(true),
			gl::command::set_depth_range({ 0.f, 1.f }),

			// stencil
			gl::command::set_stencil_enabled(true),
			gl::command::set_stencil_fn({})
		})
		{
			std::invoke(cmd);
		}

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}