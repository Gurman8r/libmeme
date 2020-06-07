#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_Renderer_OpenGL.hpp"
using impl_device		= _ML gl::opengl_device			;
using impl_vertexarray	= _ML gl::opengl_vertexarray	;
using impl_vertexbuffer	= _ML gl::opengl_vertexbuffer	;
using impl_indexbuffer	= _ML gl::opengl_indexbuffer	;
using impl_framebuffer	= _ML gl::opengl_framebuffer	;
using impl_texture2d	= _ML gl::opengl_texture2d		;
using impl_texturecube	= _ML gl::opengl_texturecube	;
using impl_program		= _ML gl::opengl_program		;
using impl_shader		= _ML gl::opengl_shader			;

#elif defined(ML_IMPL_RENDERER_DIRECTX)
#elif defined(ML_IMPL_RENDERER_VULKAN)
// etc...

#else
#error "Unknown or invalid renderer implementation specified."
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device
namespace ml::gl
{
	unique<device> const & device::get_context() noexcept
	{
		static unique<device> ctx{ _ML make_unique<impl_device>() };

		static ML_scope{ if (!ctx->do_initialize()) { ctx.reset(nullptr); } };

		return ctx;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gl
{
	shared<vertexarray> vertexarray::allocate(uint32_t mode) noexcept
	{
		return _ML make_shared<impl_vertexarray>(mode);
	}

	void vertexarray::bind(vertexarray const * value) noexcept
	{
		impl_vertexarray::do_bind(static_cast<impl_vertexarray const *>(value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gl
{
	shared<vertexbuffer> vertexbuffer::allocate(uint32_t usage, size_t count, address_t data) noexcept
	{
		return _ML make_shared<impl_vertexbuffer>(usage, count, data);
	}

	void vertexbuffer::bind(vertexbuffer const * value) noexcept
	{
		impl_vertexbuffer::do_bind(static_cast<impl_vertexbuffer const *>(value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gl
{
	shared<indexbuffer> indexbuffer::allocate(uint32_t usage, size_t count, address_t data) noexcept
	{
		return _ML make_shared<impl_indexbuffer>(usage, count, data);
	}
	
	void indexbuffer::bind(indexbuffer const * value) noexcept
	{
		impl_indexbuffer::do_bind(static_cast<impl_indexbuffer const *>(value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture
namespace ml::gl
{
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gl
{
	shared<texture2d> texture2d::allocate(texopts const & opts, address_t data) noexcept
	{
		return _ML make_shared<impl_texture2d>(opts, data);
	}

	void texture2d::bind(texture2d const * value, uint32_t slot) noexcept
	{
		impl_texture2d::do_bind(static_cast<impl_texture2d const *>(value), slot);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texturecube
namespace ml::gl
{
	shared<texturecube> texturecube::allocate(texopts const & opts) noexcept
	{
		return _ML make_shared<impl_texturecube>(opts);
	}
	
	void texturecube::bind(texturecube const * value, uint32_t slot) noexcept
	{
		impl_texturecube::do_bind(static_cast<impl_texturecube const *>(value), slot);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gl
{
	shared<framebuffer> framebuffer::allocate(vec2i const & size, uint32_t format) noexcept
	{
		return _ML make_shared<impl_framebuffer>(size, format);
	}

	void framebuffer::bind(framebuffer const * value) noexcept
	{
		impl_framebuffer::do_bind(static_cast<impl_framebuffer const *>(value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gl
{
	shared<program> program::allocate(uint32_t type) noexcept
	{
		return _ML make_shared<impl_program>(type);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gl
{
	shared<shader> shader::allocate() noexcept
	{
		return _ML make_shared<impl_shader>();
	}

	void shader::bind(shader const * value) noexcept
	{
		impl_shader::do_bind(static_cast<impl_shader const *>(value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */