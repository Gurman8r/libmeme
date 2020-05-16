#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Renderer/Renderer.hpp>

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
		auto const & api{ gl::render_context::api() };

		// initialize
		if (!api->initialize())
		{
			return debug::error("render_window failed initializing render_api");
		}

		// validate version
		m_settings.context.major = api->get_major_version();
		m_settings.context.minor = api->get_minor_version();

		// run setup commands
		for (auto const & cmd : std::initializer_list<gl::render_command>
		{
			gl::command::set_alpha_enabled(true),
			gl::command::set_alpha_function({ gl::predicate_greater, 0.001f }),
			
			gl::command::set_blend_enabled(true),
			gl::command::set_blend_color(colors::white),
			gl::command::set_blend_equation({ gl::function_add }),
			gl::command::set_blend_function({ gl::factor_src_alpha, gl::factor_one_minus_src_alpha }),
			
			gl::command::set_cull_enabled(true),
			gl::command::set_cull_facet(gl::facet_back),
			gl::command::set_cull_order(gl::cull_order_ccw),

			gl::command::set_depth_enabled(true),
			gl::command::set_depth_function(gl::predicate_less),
			gl::command::set_depth_mask(true),
			gl::command::set_depth_range({ 0.f, 1.f }),

			gl::command::set_multisample_enabled(ws.context.multisample),
			//gl::command::set_enabled(gl::capability_framebuffer_srgb, ws.context.srgb_capable),

			gl::command::flush(),
		})
		{
			std::invoke(cmd);
		}

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}