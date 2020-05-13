#include <libmeme/Renderer/Renderer.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_RENDERER_DIRECTX)
#include "Impl/Impl_Renderer_DirectX.hpp"
using ml_impl_renderer = _ML impl_renderer_directx;

#elif defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_Renderer_OpenGL.hpp"
using ml_impl_renderer = _ML impl_renderer_opengl;

#elif defined(ML_RENDERER_VULKAN)
#include "Impl/Impl_Renderer_Vulkan.hpp"
using ml_impl_renderer = _ML impl_renderer_vulkan;

// etc...

#else
#error Unknown or invalid renderer implementation specified.
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	renderer::renderer() noexcept { ML_assert(m_impl = new ml_impl_renderer{}); }

	renderer::~renderer() noexcept { delete m_impl; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool renderer::initialize()
	{
		return ml_impl_renderer::initialize();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}