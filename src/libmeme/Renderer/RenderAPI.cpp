#include <libmeme/Renderer/RenderAPI.hpp>

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

	render_api * const render_api::get() noexcept
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

	shared<vertex_array> vertex_array::create(
	)
	{
		return memory_manager::allocate_shared<impl_vertex_array>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<vertex_buffer> vertex_buffer::create(
		buffer_t vertices,
		uint32_t size,
		uint32_t usage
	)
	{
		return memory_manager::allocate_shared<impl_vertex_buffer>(vertices, size, usage);
	}

	shared<vertex_buffer> vertex_buffer::create(
		uint32_t size,
		uint32_t usage
	)
	{
		return memory_manager::allocate_shared<impl_vertex_buffer>(size, usage);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<index_buffer> index_buffer::create(
		buffer_t indices,
		uint32_t count
	)
	{
		return memory_manager::allocate_shared<impl_index_buffer>(indices, count);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<frame_buffer> frame_buffer::create(
		uint32_t format,
		vec2i const & size
	)
	{
		return memory_manager::allocate_shared<impl_frame_buffer>(format, size);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<shader_object> shader_object::create(
		cstring v_src,
		cstring f_src
	)
	{
		return memory_manager::allocate_shared<impl_shader_object>(v_src, f_src);
	}

	shared<shader_object> shader_object::create(
		cstring v_src,
		cstring g_src,
		cstring f_src
	)
	{
		return memory_manager::allocate_shared<impl_shader_object>(v_src, g_src, f_src);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<texture_object> texture_object::create(
		uint32_t type
	)
	{
		return memory_manager::allocate_shared<impl_texture_object>();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}