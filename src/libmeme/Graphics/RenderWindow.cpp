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

		// get api
		auto api{ render_api::get() };

		// initialize
		if (!api->initialize())
		{
			return debug::error("render_window failed initializing render_api");
		}

		// validate version
		m_settings.context.major = api->get_info().major_version;
		m_settings.context.minor = api->get_info().minor_version;

		for (auto const & cmd : // setup states
		{
			// alpha
			render_command::set_alpha_enabled(true),
			render_command::set_alpha_fn({ gl::predicate_greater, 0.001f }),
			
			// blend
			render_command::set_blend_enabled(true),
			render_command::set_blend_color(colors::white),
			render_command::set_blend_eq({ gl::function_add }),
			render_command::set_blend_fn({ gl::factor_src_alpha, gl::factor_one_minus_src_alpha }),
			
			// cull
			render_command::set_cull_enabled(true),
			render_command::set_cull_facet(gl::facet_back),
			render_command::set_cull_order(gl::order_ccw),

			// depth
			render_command::set_depth_enabled(true),
			render_command::set_depth_pr(gl::predicate_less),
			render_command::set_depth_mask(true),
			render_command::set_depth_range({ 0.f, 1.f }),

			// stencil
			render_command::set_stencil_enabled(true),
			render_command::set_stencil_fn({})
		})
		{
			std::invoke(cmd);
		}

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}