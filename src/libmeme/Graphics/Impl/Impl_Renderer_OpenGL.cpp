#if defined(ML_RENDERER_OPENGL)

#include "Impl_Renderer_OpenGL.hpp"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool impl_renderer_opengl::is_initialized() const
	{
		return false;
	}

	bool impl_renderer_opengl::initialize()
	{
		return false;
	}

	bool impl_renderer_opengl::finalize()
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_RENDERER_OPENGL