#include <libmeme/Renderer/RenderWindow.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/RenderStates.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool render_window::open(window_settings const & ws)
	{
		if (!window::open(ws))
		{
			return debug::log::error("render_window failed initializing platform");
		}

		if (!GL::initialize())
		{
			return debug::log::error("render_window failed initializing renderer");
		}

		GL::validateVersion(m_settings.context.major, m_settings.context.minor);

		ML_anon_v(render_states) {}();

		GL::enable(GL::Multisample, m_settings.context.multisample);

		GL::enable(GL::FramebufferSRGB, m_settings.context.srgb_capable);

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}