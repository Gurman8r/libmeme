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

	render_device * render_device::g_dev{};

	render_device * render_device::create(allocator_type alloc) noexcept
	{
		render_device * temp{ new opengl_render_device{ alloc } };

		if (!g_dev) { set_default(temp); }

		return temp;
	}

	void render_device::destroy(render_device * value) noexcept
	{
		if (!value) { value = g_dev; }

		if (g_dev == value) { set_default(nullptr); }

		delete value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}