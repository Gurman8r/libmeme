#include <libmeme/Renderer/RenderAPI.hpp>

#ifdef ML_RENDERER_OPENGL
#include <libmeme/Renderer/GL.hpp>
#endif

namespace ml
{
	bool render_api::is_initialized()
	{
#ifdef ML_RENDERER_OPENGL
		return GL::is_initialized();
#endif
	}

	bool render_api::initialize()
	{
#ifdef ML_RENDERER_OPENGL
		return GL::initialize();
#endif
	}
}