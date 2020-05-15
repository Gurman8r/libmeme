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
		
		render_command::set_enabled(GL::AlphaTest, true)();
		render_command::set_alpha_function(GL::Greater, 0.001f)();
		
		render_command::set_enabled(GL::Blend, true)();
		render_command::set_blend_function_separate(
			GL::SrcAlpha, GL::OneMinusSrcAlpha,
			GL::SrcAlpha, GL::OneMinusSrcAlpha)();
		
		render_command::set_enabled(GL::CullFace, true)();
		render_command::set_cull_mode(GL::Back)();
		
		render_command::set_enabled(GL::DepthTest, true)();
		render_command::set_depth_function(GL::Less)();
		render_command::set_depth_mask(true)();

		render_command::set_enabled(GL::Multisample, m_settings.context.multisample)();
		render_command::set_enabled(GL::FramebufferSRGB, m_settings.context.srgb_capable)();

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}