#include <libmeme/Graphics/RendererAPI.hpp>

#if defined(ML_RENDERER_OPENGL)
//	OpenGL
#	include "Impl/Impl_Renderer_OpenGL.hpp"
	using ml_impl_renderer = _ML impl_renderer_opengl;
#else
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	renderer_api::renderer_api() noexcept { ML_assert(m_impl = new ml_impl_renderer{}); }

	renderer_api::~renderer_api() noexcept { delete m_impl; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}