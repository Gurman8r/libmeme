#include <libmeme/Renderer/RenderWindow.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/RenderStates.hpp>
#include <libmeme/Renderer/GraphicsEvents.hpp>
#include <libmeme/Platform/WindowEvents.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderWindow::RenderWindow()
		: Window{}
		, RenderTarget{}
	{
		ML_EventSystem.addListener<RenderErrorEvent>(this);
	}

	RenderWindow::~RenderWindow() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool RenderWindow::create(std::string const & title, DisplayMode const & video, Style const & style, Context const & context)
	{
		if (Window::create(title, video, style, context))
		{
			if (!ML_GL.init())
			{
				return Debug::logError("Failed initializing OpenGL");
			}

			ML_GL.validateVersion(m_context.major, m_context.minor);

			RenderStates{}(); // default states

			ML_GL.enable(GL::Multisample, m_context.multisample);

			ML_GL.enable(GL::FramebufferSRGB, m_context.srgbCapable);

			return true;
		}
		return Debug::logError("Failed initializing RenderWindow");
	}

	void RenderWindow::onEvent(Event const & value)
	{
		Window::onEvent(value);
		switch (*value)
		{
		case RenderErrorEvent::ID: if (auto ev = value.as<RenderErrorEvent>())
		{
			// Error location
			std::string filename{ ev.file };
			filename = filename.substr(filename.find_last_of("\\/") + 1);

			// Decode the error
			std::cout
				<< FG::Red << "An OpenGL call failed in \'" << ev.file << "\' (" << ev.line << ")"
				<< FG::Yellow << "\nCode: "
				<< FG::White << "\n\t" << ev.code
				<< FG::Yellow << "\nExpression: "
				<< FG::White << "\n\t" << ev.expr
				<< FG::Yellow << "\nDescription:"
				<< FG::White << "\n\t" << GL::name_of((GL::Err)ev.code)
				<< FG::White << "\n\t" << GL::desc_of((GL::Err)ev.code)
				<< endcol << '\n';
		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}