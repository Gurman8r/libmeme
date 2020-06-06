#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_Renderer_OpenGL.hpp"
using impl_device			= _ML gl::opengl_device			;
using impl_vertex_array		= _ML gl::opengl_vertexarray	;
using impl_vertex_buffer	= _ML gl::opengl_vertexbuffer	;
using impl_index_buffer		= _ML gl::opengl_indexbuffer	;
using impl_frame_buffer		= _ML gl::opengl_framebuffer	;
using impl_texture2d		= _ML gl::opengl_texture2d		;
using impl_texture_cube		= int							;
using impl_shader			= _ML gl::opengl_shader			;
using impl_program			= _ML gl::opengl_program		;

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

		static ML_scope // once
		{
			if (ctx && !ctx->do_initialize())
			{
				ctx.reset(nullptr);
			}
		};
		
		return ctx;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gl
{
	shared<vertexarray> vertexarray::allocate() noexcept
	{
		return _ML make_shared<impl_vertex_array>();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gl
{
	shared<vertexbuffer> vertexbuffer::allocate(address_t data, size_t count) noexcept
	{
		return _ML make_shared<impl_vertex_buffer>(data, count);
	}

	shared<vertexbuffer> vertexbuffer::allocate(size_t count) noexcept
	{
		return _ML make_shared<impl_vertex_buffer>(count);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gl
{
	shared<indexbuffer> indexbuffer::allocate(address_t data, size_t count) noexcept
	{
		return _ML make_shared<impl_index_buffer>(data, count);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gl
{
	shared<framebuffer> framebuffer::allocate(vec2i const & size, uint32_t format) noexcept
	{
		return _ML make_shared<impl_frame_buffer>(size, format);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gl
{
	shared<texture2d> texture2d::allocate(vec2i const & size, uint32_t i_format, uint32_t c_format, uint32_t p_type, int32_t flags, address_t data) noexcept
	{
		return _ML make_shared<impl_texture2d>(size, i_format, c_format, p_type, flags, data);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture_cube
namespace ml::gl
{
	shared<texture_cube> texture_cube::allocate() noexcept
	{
		return impl_texture_cube{};
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gl
{
	shared<shader> shader::allocate(uint32_t type) noexcept
	{
		return _ML make_shared<impl_shader>(type);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gl
{
	shared<program> program::allocate() noexcept
	{
		return _ML make_shared<impl_program>();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */