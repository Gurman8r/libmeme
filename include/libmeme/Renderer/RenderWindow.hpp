#ifndef _ML_RENDER_WINDOW_HPP_
#define _ML_RENDER_WINDOW_HPP_

#include <libmeme/Renderer/RenderTarget.hpp>
#include <libmeme/Platform/Window.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API RenderWindow : public Window, public RenderTarget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderWindow();

		virtual ~RenderWindow();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool create(
			std::string const & title,
			DisplayMode const & videoMode,
			Style const & style,
			Context const & context
		) override;

		virtual void onEvent(Event const & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_WINDOW_HPP_