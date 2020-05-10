#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Graphics/GL.hpp>

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

		GL::enable(GL::AlphaTest);
		GL::alphaFunc(GL::Greater, 0.001f);

		GL::enable(GL::Blend);
		GL::blendFuncSeparate(
			GL::SrcAlpha, GL::OneMinusSrcAlpha,
			GL::SrcAlpha, GL::OneMinusSrcAlpha);

		GL::enable(GL::CullFace);
		GL::cullFace(GL::Back);

		GL::enable(GL::DepthTest);
		GL::depthFunc(GL::Less);
		GL::depthMask(true);

		GL::enable(GL::Multisample, m_settings.context.multisample);
		GL::enable(GL::FramebufferSRGB, m_settings.context.srgb_capable);

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}