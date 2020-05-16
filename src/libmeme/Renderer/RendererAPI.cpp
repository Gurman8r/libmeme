#include <libmeme/Renderer/RendererAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_Renderer_OpenGL.hpp"
using impl_render_api		= _ML gl::opengl_render_api;
using impl_vertex_array		= _ML gl::opengl_vertex_array;
using impl_vertex_buffer	= _ML gl::opengl_vertex_buffer;
using impl_index_buffer		= _ML gl::opengl_index_buffer;
using impl_frame_buffer		= _ML gl::opengl_frame_buffer;
using impl_render_buffer	= _ML gl::opengl_render_buffer;
using impl_shader			= _ML gl::opengl_shader;
using impl_texture			= _ML gl::opengl_texture;

#elif defined(ML_RENDERER_DIRECTX)
#include "Impl/Impl_Renderer_DirectX.hpp"

#elif defined(ML_RENDERER_VULKAN)
#include "Impl/Impl_Renderer_Vulkan.hpp"

#else
#error "Unknown or invalid renderer implementation specified."
#endif

// context
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_context::render_context() noexcept { m_api = std::make_unique<impl_render_api>(); }

	render_context::~render_context() noexcept {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// resources
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void vertex_array::bind(vertex_array const * value)
	{
		impl_vertex_array::bind(static_cast<impl_vertex_array const *>(value));
	}

	vertex_array * vertex_array::create(uint32_t mode)
	{
		return new impl_vertex_array{ mode };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void vertex_buffer::bind(vertex_buffer const * value)
	{
		impl_vertex_buffer::bind(static_cast<impl_vertex_buffer const *>(value));
	}

	vertex_buffer * vertex_buffer::create(float_t const * vertices, uint32_t size)
	{
		return new impl_vertex_buffer{ vertices, size };
	}

	vertex_buffer * vertex_buffer::create(float_t const * vertices, uint32_t size, uint32_t offset)
	{
		return new impl_vertex_buffer{ vertices, size, offset };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void index_buffer::bind(index_buffer const * value)
	{
		impl_index_buffer::bind(static_cast<impl_index_buffer const *>(value));
	}

	index_buffer * index_buffer::create(uint32_t const * indices, uint32_t count)
	{
		return new impl_index_buffer{ indices, count };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void frame_buffer::bind(frame_buffer const * value)
	{
		impl_frame_buffer::bind(static_cast<impl_frame_buffer const *>(value));
	}

	frame_buffer * frame_buffer::create()
	{
		return new impl_frame_buffer{};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void render_buffer::bind(render_buffer const * value)
	{
		impl_render_buffer::bind(static_cast<impl_render_buffer const *>(value));
	}

	render_buffer * render_buffer::create(uint32_t format, vec2i const & size)
	{
		return new impl_render_buffer{ format, size };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void shader_object::bind(shader_object const * value)
	{
		impl_shader::bind(static_cast<impl_shader const *>(value));
	}

	shader_object * shader_object::create()
	{
		return new impl_shader{};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	void texture_object::bind(texture_object const * value)
	{
		impl_texture::bind(static_cast<impl_texture const *>(value));
	}

	texture_object * texture_object::create()
	{
		return new impl_texture{};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}