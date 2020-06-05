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
using impl_program			= _ML gl::opengl_program		;
using impl_shader			= _ML gl::opengl_shader			;

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
	device::context const & device::get_context() noexcept
	{
		static device::context ctx{ memory::make_unique<impl_device>() };

		static ML_scope{ ML_assert(ctx && ctx->do_initialize()); };
		
		return ctx;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gl
{
	shared<vertexarray> vertexarray::allocate() noexcept
	{
		return memory::make_shared<impl_vertex_array>();
	}
}

// vertexbuffer
namespace ml::gl
{
	shared<vertexbuffer> vertexbuffer::allocate(buffer_t data, size_t count) noexcept
	{
		return memory::make_shared<impl_vertex_buffer>(data, count);
	}

	shared<vertexbuffer> vertexbuffer::allocate(size_t count) noexcept
	{
		return memory::make_shared<impl_vertex_buffer>(count);
	}
}

// indexbuffer
namespace ml::gl
{
	shared<indexbuffer> indexbuffer::allocate(buffer_t data, size_t count) noexcept
	{
		return memory::make_shared<impl_index_buffer>(data, count);
	}
}

// framebuffer
namespace ml::gl
{
	shared<framebuffer> framebuffer::allocate(vec2i const & size, uint32_t format) noexcept
	{
		return memory::make_shared<impl_frame_buffer>(size, format);
	}
}

// texture2d
namespace ml::gl
{
	shared<texture2d> texture2d::allocate(vec2i const & size, uint32_t iformat, uint32_t cformat, uint32_t ptype, int32_t flags, buffer_t pixels) noexcept
	{
		return memory::make_shared<impl_texture2d>(size, iformat, cformat, ptype, flags, pixels);
	}
}

// program
namespace ml::gl
{
	shared<program> program::allocate(uint32_t type) noexcept
	{
		return memory::make_shared<impl_program>(type);
	}
}

// shader
namespace ml::gl
{
	shared<shader> shader::allocate() noexcept
	{
		return memory::make_shared<impl_shader>();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */