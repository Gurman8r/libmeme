#if defined(ML_RENDERER_OPENGL)
#ifndef _ML_IMPL_RENDERER_OPENGL_HPP_
#define _ML_IMPL_RENDERER_OPENGL_HPP_

// WIP

#include <libmeme/Graphics/RendererImpl.hpp>

namespace ml
{
	struct impl_renderer_opengl final : renderer_impl
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~impl_renderer_opengl() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_IMPL_RENDERER_OPENGL_HPP_
#endif // ML_RENDERER_OPENGL