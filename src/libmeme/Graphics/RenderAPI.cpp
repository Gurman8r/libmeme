#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_RenderAPI_OpenGL.hpp"
using impl_device		= _ML_GFX opengl_device			;
using impl_devctx		= _ML_GFX opengl_context		;
using impl_vertexarray	= _ML_GFX opengl_vertexarray	;
using impl_vertexbuffer	= _ML_GFX opengl_vertexbuffer	;
using impl_indexbuffer	= _ML_GFX opengl_indexbuffer	;
using impl_texture2d	= _ML_GFX opengl_texture2d		;
using impl_texturecube	= _ML_GFX opengl_texturecube	;
using impl_framebuffer	= _ML_GFX opengl_framebuffer	;
using impl_shader		= _ML_GFX opengl_shader			;
using impl_program		= _ML_GFX opengl_program		;

#elif defined(ML_IMPL_RENDERER_DIRECTX)
#elif defined(ML_IMPL_RENDERER_VULKAN)
// etc...

#else
#	error "Unknown or invalid renderer implementation specified."
#endif

namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	device * device::g_device{};

	device * device::create() noexcept
	{
		device * temp{ new impl_device{} };

		if (!g_device) { set_default(temp); }

		return temp;
	}

	void device::destroy(device * value) noexcept
	{
		if (!value) { value = g_device; }

		if (g_device == value) { set_default(nullptr); }

		delete value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<context> context::create(context_settings const & cs) noexcept
	{
		return make_shared<impl_devctx>(device::get_default(), cs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<vertexarray> vertexarray::create(uint32_t primitive) noexcept
	{
		return make_shared<impl_vertexarray>(device::get_default(), primitive);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<vertexbuffer> vertexbuffer::create(uint32_t usage, size_t count, address_t data) noexcept
	{
		return make_shared<impl_vertexbuffer>(device::get_default(), usage, count, data);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<indexbuffer> indexbuffer::create(uint32_t usage, size_t count, address_t data) noexcept
	{
		return make_shared<impl_indexbuffer>(device::get_default(), usage, count, data);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<texture2d> texture2d::create(texopts const & opts, address_t data) noexcept
	{
		return make_shared<impl_texture2d>(device::get_default(), opts, data);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<texturecube> texturecube::create(texopts const & opts) noexcept
	{
		return make_shared<impl_texturecube>(device::get_default(), opts);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<framebuffer> framebuffer::create(texopts const & opts) noexcept
	{
		return make_shared<impl_framebuffer>(device::get_default(), opts);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<shader> shader::create(uint32_t type, int32_t flags) noexcept
	{
		return make_shared<impl_shader>(device::get_default(), type, flags);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<program> program::create(int32_t flags) noexcept
	{
		return make_shared<impl_program>(device::get_default(), flags);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}