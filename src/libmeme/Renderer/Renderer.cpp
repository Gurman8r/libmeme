#include <libmeme/Renderer/Renderer.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_RENDERER_DIRECTX)
#include "Impl/Impl_Renderer_DirectX.hpp"

#elif defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_Renderer_OpenGL.hpp"

#elif defined(ML_RENDERER_VULKAN)
#include "Impl/Impl_Renderer_Vulkan.hpp"

// etc...

#else
#error Unknown or invalid renderer implementation specified.
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}