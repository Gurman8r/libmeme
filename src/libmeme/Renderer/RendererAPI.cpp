#include <libmeme/Renderer/RendererAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_Renderer_OpenGL.hpp"
using impl_render_api		= _ML gl::opengl_render_api;
using impl_vertex_array		= _ML gl::opengl_vertex_array;
using impl_vertex_buffer	= _ML gl::opengl_vertex_buffer;
using impl_index_buffer		= _ML gl::opengl_index_buffer;
using impl_frame_buffer		= _ML gl::opengl_frame_buffer;
using impl_shader_object	= _ML gl::opengl_shader_object;
using impl_texture_object	= _ML gl::opengl_texture_object;

#else
#error "Unknown or invalid renderer implementation specified."
#endif

// context
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_context::render_context() noexcept : m_api{ std::make_unique<impl_render_api>() } {}

	render_context::~render_context() noexcept {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// objects
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<vertex_array> vertex_array::create(uint32_t mode, allocator alloc)
	{
		return std::allocate_shared<impl_vertex_array>(alloc, mode);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<vertex_buffer> vertex_buffer::create(buffer vertices, uint32_t size, allocator alloc)
	{
		return std::allocate_shared<impl_vertex_buffer>(alloc, vertices, size);
	}

	shared<vertex_buffer> vertex_buffer::create(buffer vertices, uint32_t size, uint32_t offset, allocator alloc)
	{
		return std::allocate_shared<impl_vertex_buffer>(alloc, vertices, size, offset);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<index_buffer> index_buffer::create(buffer indices, uint32_t count, allocator alloc)
	{
		return std::allocate_shared<impl_index_buffer>(alloc, indices, count);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<frame_buffer> frame_buffer::create(allocator alloc)
	{
		return std::allocate_shared<impl_frame_buffer>(alloc);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<shader_object> shader_object::create(allocator alloc)
	{
		return std::allocate_shared<impl_shader_object>(alloc);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<texture_object> texture_object::create(allocator alloc)
	{
		return std::allocate_shared<impl_texture_object>(alloc);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}