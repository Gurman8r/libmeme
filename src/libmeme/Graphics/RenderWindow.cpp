#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Graphics/GL.hpp>
#include <libmeme/Renderer/Renderer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool render_window::open(window_settings const & ws, bool install_callbacks)
	{
		if (!window::open(ws, install_callbacks))
		{
			return debug::error("render_window failed initializing platform");
		}

		if (!GL::initialize())
		{
			return debug::error("render_window failed initializing renderer");
		}

		GL::validateVersion(m_settings.context.major, m_settings.context.minor);
		
		render_command::set_enabled(gl::capability_alpha_test, true)();
		render_command::set_alpha_function(gl::predicate_greater, 0.001f)();
		
		render_command::set_enabled(gl::capability_blend, true)();
		render_command::set_blend_function(
			gl::factor_src_alpha, gl::factor_one_minus_src_alpha,
			gl::factor_src_alpha, gl::factor_one_minus_src_alpha)();
		
		render_command::set_enabled(gl::capability_cull_face, true)();
		render_command::set_cull_mode(gl::facet_back)();
		
		render_command::set_enabled(gl::capability_depth_test, true)();
		render_command::set_depth_function(gl::predicate_less)();
		render_command::set_depth_mask(true)();

		render_command::set_enabled(gl::capability_multisample, m_settings.context.multisample)();
		render_command::set_enabled(gl::capability_framebuffer_srgb, m_settings.context.srgb_capable)();

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}