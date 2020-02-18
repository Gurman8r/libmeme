#include <libmeme/Renderer/RenderWindow.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/RenderStates.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_window::render_window() noexcept : window{}, render_target{}
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool render_window::create(cstring title, video_mode const & display, context_settings const & context, int32_t flags)
	{
		if (!window::create(title, display, context, flags))
		{
			return debug::log_error("failed initializing ml::render_window");
		}

		if (!GL::startup())
		{
			return debug::log_error("failed initializing OpenGL");
		}

		GL::validateVersion(m_context.major, m_context.minor);

		constexpr render_states states{
		}; states();

		GL::enable(GL::Multisample, m_context.multisample);

		GL::enable(GL::FramebufferSRGB, m_context.sRGB_capable);

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void render_window::on_event(event const & value)
	{
		window::on_event(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}