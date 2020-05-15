#include <libmeme/Renderer/RendererAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_Renderer_OpenGL.hpp"
using impl_renderer_api		= _ML opengl_render_api;
using impl_vertex_array		= _ML opengl_vertex_array;
using impl_vertex_buffer	= _ML opengl_vertex_buffer;
using impl_index_buffer		= _ML opengl_index_buffer;
using impl_frame_buffer		= _ML opengl_frame_buffer;
using impl_render_buffer	= _ML opengl_render_buffer;

#else
#error Unknown or invalid renderer implementation specified.
#endif

// context
ml::render_context::render_context() noexcept { m_api = std::make_unique<impl_renderer_api>(); }
ml::render_context::~render_context() noexcept {}

// buffers
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void vertex_array::bind(vertex_array const * value) noexcept
	{
		impl_vertex_array::bind(static_cast<impl_vertex_array const *>(value));
	}

	vertex_array * vertex_array::create(uint32_t mode) noexcept
	{
		return new impl_vertex_array{ mode };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void vertex_buffer::bind(vertex_buffer const * value) noexcept
	{
		impl_vertex_buffer::bind(static_cast<impl_vertex_buffer const *>(value));
	}

	vertex_buffer * vertex_buffer::create(float_t const * vertices, uint32_t size) noexcept
	{
		return new impl_vertex_buffer{ vertices, size };
	}

	vertex_buffer * vertex_buffer::create(float_t const * vertices, uint32_t size, uint32_t offset) noexcept
	{
		return new impl_vertex_buffer{ vertices, size, offset };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void index_buffer::bind(index_buffer const * value) noexcept
	{
		impl_index_buffer::bind(static_cast<impl_index_buffer const *>(value));
	}

	index_buffer * index_buffer::create(uint32_t const * indices, uint32_t count) noexcept
	{
		return new impl_index_buffer{ indices, count };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void frame_buffer::bind(frame_buffer const * value) noexcept
	{
		impl_frame_buffer::bind(static_cast<impl_frame_buffer const *>(value));
	}

	frame_buffer * frame_buffer::create() noexcept
	{
		return new impl_frame_buffer{};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void render_buffer::bind(render_buffer const * value) noexcept
	{
		impl_render_buffer::bind(static_cast<impl_render_buffer const *>(value));
	}

	render_buffer * render_buffer::create(uint32_t format, int32_t width, int32_t height) noexcept
	{
		return new impl_render_buffer{ format, width, height };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}