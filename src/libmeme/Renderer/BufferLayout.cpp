#include <libmeme/Renderer/BufferLayout.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	buffer_layout::attribute const & buffer_layout::attribute::operator()() const noexcept
	{
		GL::vertexAttribPointer(index, size, type, normalize, stride, offset, width);
		GL::enableVertexAttribArray(index);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}