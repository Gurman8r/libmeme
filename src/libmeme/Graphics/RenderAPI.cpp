#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_Renderer_OpenGL.hpp"
using impl_render_api		= _ML gl::opengl_render_api		;
using impl_vertex_array		= _ML gl::opengl_vertexarray	;
using impl_vertex_buffer	= _ML gl::opengl_vertexbuffer	;
using impl_index_buffer		= _ML gl::opengl_indexbuffer	;
using impl_frame_buffer		= _ML gl::opengl_framebuffer	;
using impl_shader			= _ML gl::opengl_shader			;
using impl_texture2d		= _ML gl::opengl_texture2d		;

#elif defined(ML_IMPL_RENDERER_DIRECTX)
#elif defined(ML_IMPL_RENDERER_VULKAN)
// etc...

#else
#error "Unknown or invalid renderer implementation specified."
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// api
namespace ml::gl
{
	render_api * const render_api::get() noexcept
	{
		static impl_render_api inst{};
		return std::addressof(inst);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gl
{
	shared<vertexarray> vertexarray::create() noexcept
	{
		return memory_manager::allocate_shared<impl_vertex_array>();
	}
}

// vertexbuffer
namespace ml::gl
{
	shared<vertexbuffer> vertexbuffer::create(buffer verts, uint32_t count) noexcept
	{
		return memory_manager::allocate_shared<impl_vertex_buffer>(verts, count);
	}

	shared<vertexbuffer> vertexbuffer::create(uint32_t count) noexcept
	{
		return memory_manager::allocate_shared<impl_vertex_buffer>(count);
	}
}

// indexbuffer
namespace ml::gl
{
	shared<indexbuffer> indexbuffer::create(buffer inds, uint32_t count) noexcept
	{
		return memory_manager::allocate_shared<impl_index_buffer>(inds, count);
	}
}

// framebuffer
namespace ml::gl
{
	shared<framebuffer> framebuffer::create(uint32_t format, vec2i const & size) noexcept
	{
		return memory_manager::allocate_shared<impl_frame_buffer>(format, size);
	}
}

// texture2d
namespace ml::gl
{
	shared<texture2d> texture2d::create(vec2i const & size, uint32_t iformat, uint32_t cformat, uint32_t ptype, int32_t flags, buffer pixels) noexcept
	{
		return memory_manager::allocate_shared<impl_texture2d>(size, iformat, cformat, ptype, flags, pixels);
	}
}

// shader
namespace ml::gl
{
	shared<shader> shader::create() noexcept
	{
		return memory_manager::allocate_shared<impl_shader>();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */