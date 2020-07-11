#include <libmeme/Graphics/RenderAPI.hpp>

#ifdef ML_IMPL_RENDERER_OPENGL
#include "Impl/Impl_RenderAPI_OpenGL.hpp"
#endif

#ifdef ML_IMPL_RENDERER_DIRECTX
#include "Impl/Impl_RenderAPI_DirectX.hpp"
#endif

#ifdef ML_IMPL_RENDERER_VULKAN
#include "Impl/Impl_RenderAPI_Vulkan.hpp"
#endif

namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_device * render_device::g_device{};

	render_device * render_device::create() noexcept
	{
		render_device * temp{ new opengl_render_device{} };

		if (!g_device) { set_default(temp); }

		return temp;
	}

	void render_device::destroy(render_device * value) noexcept
	{
		if (!value) { value = g_device; }

		if (g_device == value) { set_default(nullptr); }

		delete value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}