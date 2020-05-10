#include <libmeme/Graphics/Renderer.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_RENDERER_OPENGL)
#include "Impl/Impl_Renderer_OpenGL.hpp"
using ml_impl_renderer = _ML impl_renderer_opengl;

#elif defined(ML_RENDERER_VULKAN)
#include "Impl/Impl_Renderer_Vulkan.hpp"
using ml_impl_renderer = _ML impl_renderer_vulkan;

#elif defined(ML_RENDERER_DIRECTX)
#include "Impl/Impl_Renderer_DirectX.hpp"
using ml_impl_renderer = _ML impl_renderer_directx;

// etc...

#else
#error Unknown renderer implementation.
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	renderer::renderer() noexcept { ML_assert(m_impl = new ml_impl_renderer{}); }

	renderer::~renderer() noexcept { delete m_impl; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool renderer::is_initialized() const
	{
		return m_impl->is_initialized();
	}

	bool renderer::initialize()
	{
		return m_impl->initialize();
	}

	bool renderer::finalize()
	{
		return m_impl->finalize();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}