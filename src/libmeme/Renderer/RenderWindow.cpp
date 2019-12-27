#include <libmeme/Renderer/RenderWindow.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/RenderStates.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderWindow::RenderWindow() : Window{}, RenderTarget{}
	{
	}

	RenderWindow::~RenderWindow() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool RenderWindow::create(std::string const & title, DisplayMode const & video, WindowStyle const & style, ContextSettings const & context)
	{
		if (Window::create(title, video, style, context))
		{
			if (!GL::init())
			{
				return Debug::logError("Failed initializing OpenGL");
			}

			GL::validateVersion(m_context.major, m_context.minor);

			RenderStates{}(); // default states

			GL::enable(GL::Multisample, m_context.multisample);

			GL::enable(GL::FramebufferSRGB, m_context.srgbCapable);

			return true;
		}
		return Debug::logError("Failed initializing RenderWindow");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}