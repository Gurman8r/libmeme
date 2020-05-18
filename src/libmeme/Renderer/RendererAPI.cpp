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

#elif defined(ML_IMPL_RENDERER_DIRECTX)
#elif defined(ML_IMPL_RENDERER_VULKAN)
// etc...

#else
#error "Unknown or invalid renderer implementation specified."
#endif

// api
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_api * render_api::get_instance() noexcept
	{
		static impl_render_api inst{};
		return std::addressof(inst);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// objects
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<vertex_array> vertex_array::create()
	{
		return std::allocate_shared<impl_vertex_array>(pmr::polymorphic_allocator<byte_t>{}
			);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<vertex_buffer> vertex_buffer::create(buffer vertices, uint32_t size, uint32_t usage)
	{
		return std::allocate_shared<impl_vertex_buffer>(pmr::polymorphic_allocator<byte_t>{}
			, vertices, size, usage);
	}

	shared<vertex_buffer> vertex_buffer::create(uint32_t size, uint32_t usage)
	{
		return std::allocate_shared<impl_vertex_buffer>(pmr::polymorphic_allocator<byte_t>{}
			, size, usage);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<index_buffer> index_buffer::create(buffer indices, uint32_t count)
	{
		return std::allocate_shared<impl_index_buffer>(pmr::polymorphic_allocator<byte_t>{}
			, indices, count);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<frame_buffer> frame_buffer::create(uint32_t format, vec2i const & size)
	{
		return std::allocate_shared<impl_frame_buffer>(pmr::polymorphic_allocator<byte_t>{}
			, format, size);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<shader_object> shader_object::create()
	{
		return std::allocate_shared<impl_shader_object>(pmr::polymorphic_allocator<byte_t>{}
			);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<texture_object> texture_object::create()
	{
		return std::allocate_shared<impl_texture_object>(pmr::polymorphic_allocator<byte_t>{}
			);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}