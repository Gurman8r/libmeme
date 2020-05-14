#include <libmeme/Renderer/RendererAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_RENDERER_OPENGL)
#include "Impl/Impl_Renderer_OpenGL.hpp"
using impl_vertex_array		= _ML opengl_vertex_array;
using impl_vertex_buffer	= _ML opengl_vertex_buffer;
using impl_index_buffer		= _ML opengl_index_buffer;
using impl_frame_buffer		= _ML opengl_frame_buffer;
using impl_render_buffer	= _ML opengl_render_buffer;

#else
#error Unknown or invalid renderer implementation specified.
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	vertex_array * vertex_array::create(uint32_t mode) noexcept
	{
		return new impl_vertex_array{ mode };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	vertex_buffer * vertex_buffer::create(uint32_t usage) noexcept
	{
		return new impl_vertex_buffer{ usage };
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

	index_buffer * index_buffer::create(uint32_t usage, uint32_t type) noexcept
	{
		return new impl_index_buffer{ usage, type };
	}

	index_buffer * index_buffer::create(uint32_t const * indices, uint32_t count) noexcept
	{
		return new impl_index_buffer{ indices, count };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	frame_buffer * frame_buffer::create() noexcept
	{
		return new impl_frame_buffer{};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_buffer * render_buffer::create(uint32_t format, int32_t width, int32_t height) noexcept
	{
		return new impl_render_buffer{ format, width, height };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}