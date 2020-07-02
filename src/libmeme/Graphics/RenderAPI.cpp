#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_RenderAPI_OpenGL.hpp"
using impl_device		= _ML_GFX opengl_device			;
using impl_devctx		= _ML_GFX opengl_devctx			;
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
		auto * temp{ new impl_device{} };

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

	ML_NODISCARD shared<devctx> devctx::create(context_settings const & cs) noexcept
	{
		return make_shared<impl_devctx>(device::get_default(), cs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<vertexarray> vertexarray::create(uint32_t primitive) noexcept
	{
		return make_shared<impl_vertexarray>(device::get_default(), primitive);
	}

	void vertexarray::bind(vertexarray const * value) noexcept
	{
		impl_vertexarray::do_bind(static_cast<impl_vertexarray const *>(value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<vertexbuffer> vertexbuffer::create(uint32_t usage, size_t count, address_t data) noexcept
	{
		return make_shared<impl_vertexbuffer>(device::get_default(), usage, count, data);
	}

	void vertexbuffer::bind(vertexbuffer const * value) noexcept
	{
		impl_vertexbuffer::do_bind(static_cast<impl_vertexbuffer const *>(value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<indexbuffer> indexbuffer::create(uint32_t usage, size_t count, address_t data) noexcept
	{
		return make_shared<impl_indexbuffer>(device::get_default(), usage, count, data);
	}

	void indexbuffer::bind(indexbuffer const * value) noexcept
	{
		impl_indexbuffer::do_bind(static_cast<impl_indexbuffer const *>(value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void texture::bind(texture const * value, uint32_t slot) noexcept
	{
		switch (value ? value->get_texture_type() : texture_type_2d)
		{
		case texture_type_2d:
			return texture2d::bind(static_cast<texture2d const *>(value), slot);

		case texture_type_cubemap:
			return texturecube::bind(static_cast<texturecube const *>(value), slot);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<texture2d> texture2d::create(texopts const & opts, address_t data) noexcept
	{
		return make_shared<impl_texture2d>(device::get_default(), opts, data);
	}

	void texture2d::bind(texture2d const * value, uint32_t slot) noexcept
	{
		impl_texture2d::do_bind(static_cast<impl_texture2d const *>(value), slot);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<texturecube> texturecube::create(texopts const & opts) noexcept
	{
		return make_shared<impl_texturecube>(device::get_default(), opts);
	}

	void texturecube::bind(texturecube const * value, uint32_t slot) noexcept
	{
		impl_texturecube::do_bind(static_cast<impl_texturecube const *>(value), slot);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<framebuffer> framebuffer::create(texopts const & opts) noexcept
	{
		return make_shared<impl_framebuffer>(device::get_default(), opts);
	}

	void framebuffer::bind(framebuffer const * value) noexcept
	{
		impl_framebuffer::do_bind(static_cast<impl_framebuffer const *>(value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<shader> shader::create(uint32_t type, int32_t flags) noexcept
	{
		return make_shared<impl_shader>(nullptr, type, flags);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<program> program::create(int32_t flags) noexcept
	{
		return make_shared<impl_program>(nullptr, flags);
	}

	void program::bind(program const * value) noexcept
	{
		impl_program::do_bind(static_cast<impl_program const *>(value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}