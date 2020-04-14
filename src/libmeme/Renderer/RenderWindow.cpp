#include <libmeme/Renderer/RenderWindow.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/RenderStates.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool render_window::create(window_settings const & settings, bool install_callbacks)
	{
		if (!window::create(settings, install_callbacks))
		{
			return debug::log::error("render_window failed initializing platform");
		}

		if (!GL::initialize())
		{
			return debug::log::error("render_window failed initializing renderer");
		}

		GL::validateVersion(m_settings.context.major, m_settings.context.minor);

		constexpr render_states states{
		}; states();

		GL::enable(GL::Multisample, m_settings.context.multisample);

		GL::enable(GL::FramebufferSRGB, m_settings.context.srgb_capable);

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}