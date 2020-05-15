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

		// initialize api
		if (!render_context::api()->initialize())
		{
			return debug::error("render_window failed initializing renderer");
		}

		// validate version
		m_settings.context.major = render_context::api()->get_major_version();
		m_settings.context.minor = render_context::api()->get_minor_version();

		for (auto const & fn : // commands
		{
			render_command::set_active_texture(nullptr),

			render_command::set_enabled(gl::capability_alpha_test, true),
			render_command::set_alpha_function(gl::predicate_greater, 0.001f),
			
			render_command::set_enabled(gl::capability_blend, true),
			render_command::set_blend_equation(gl::function_add),
			render_command::set_blend_function(gl::factor_src_alpha, gl::factor_one_minus_src_alpha),
			
			render_command::set_enabled(gl::capability_cull_face, true),
			render_command::set_cull_mode(gl::facet_back),
			
			render_command::set_enabled(gl::capability_depth_test, true),
			render_command::set_depth_function(gl::predicate_less),
			render_command::set_depth_mask(true),

			render_command::set_enabled(gl::capability_multisample, ws.context.multisample),
			render_command::set_enabled(gl::capability_framebuffer_srgb, ws.context.srgb_capable),

			render_command::flush(),
		})
		{
			std::invoke(fn);
		}

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}