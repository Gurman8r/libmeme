#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_Renderer_OpenGL.hpp"
using impl_render_api		= _ML opengl_render_api		;
using impl_vertex_array		= _ML opengl_vertexarray	;
using impl_vertex_buffer	= _ML opengl_vertexbuffer	;
using impl_index_buffer		= _ML opengl_indexbuffer	;
using impl_frame_buffer		= _ML opengl_framebuffer	;
using impl_shader_object	= _ML opengl_shader_object	;
using impl_texture2d		= _ML opengl_texture2d		;

#elif defined(ML_IMPL_RENDERER_DIRECTX)
#elif defined(ML_IMPL_RENDERER_VULKAN)
// etc...

#else
#error "Unknown or invalid renderer implementation specified."
#endif

// api
namespace ml
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
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// VERTEX ARRAY
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<vertexarray> vertexarray::create()
	{
		return std::allocate_shared<impl_vertex_array>(memory_manager::get_allocator());
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// VERTEX BUFFER
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<vertexbuffer> vertexbuffer::create(gl::buffer vertices, uint32_t size, uint32_t usage)
	{
		return std::allocate_shared<impl_vertex_buffer>(memory_manager::get_allocator()
			, vertices, size, usage);
	}

	shared<vertexbuffer> vertexbuffer::create(uint32_t size, uint32_t usage)
	{
		return std::allocate_shared<impl_vertex_buffer>(memory_manager::get_allocator()
			, size, usage);
	}

	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// INDEX BUFFER
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<indexbuffer> indexbuffer::create(gl::buffer indices, uint32_t count)
	{
		return std::allocate_shared<impl_index_buffer>(memory_manager::get_allocator()
			, indices, count);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// FRAME BUFFER
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<framebuffer> framebuffer::create(uint32_t format, vec2i const & size)
	{
		return std::allocate_shared<impl_frame_buffer>(memory_manager::get_allocator()
			, format, size);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// SHADER
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<shader_object> shader_object::create(cstring v_src, cstring f_src, cstring g_src)
	{
		return std::allocate_shared<impl_shader_object>(memory_manager::get_allocator()
			, v_src, f_src, g_src);
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// TEXTURE2D
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<texture2d> texture2d::create(vec2i const & size, uint32_t iformat, uint32_t cformat, uint32_t ptype, int32_t flags, gl::buffer pixels)
	{
		return std::allocate_shared<impl_texture2d>(memory_manager::get_allocator()
			, size, iformat, cformat, ptype, flags, pixels);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}