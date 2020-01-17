#include <libmeme/Renderer/RenderWindow.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/RenderStates.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderWindow::RenderWindow() noexcept : Window{}, RenderTarget{}
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool RenderWindow::create(std::string const & title, VideoMode const & display, ContextSettings const & context, int32_t flags)
	{
		if (!Window::create(title, display, context, flags))
		{
			return debug::log_error("Failed initializing RenderWindow");
		}

		if (!GL::init())
		{
			return debug::log_error("Failed initializing OpenGL");
		}

		GL::validateVersion(m_context.major, m_context.minor);

		constexpr RenderStates states{ // default states
			{}, {}, {}, {}
		}; states();

		GL::enable(GL::Multisample, m_context.multisample);

		GL::enable(GL::FramebufferSRGB, m_context.sRGB_capable);

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}