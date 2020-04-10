#ifndef _ML_RENDER_WINDOW_HPP_
#define _ML_RENDER_WINDOW_HPP_

#include <libmeme/Platform/BaseWindow.hpp>
#include <libmeme/Renderer/RenderTarget.hpp>

namespace ml
{
	struct ML_RENDERER_API render_window : base_window, render_target
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_window() noexcept = default;

		ML_NODISCARD virtual bool open() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_WINDOW_HPP_