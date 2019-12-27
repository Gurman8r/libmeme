#include <libmeme/Renderer/BufferLayout.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	BufferLayout::Attribute const & BufferLayout::Attribute::operator()() const
	{
		GL::vertexAttribPointer(index, size, type, normalize, stride, offset, width);
		GL::enableVertexAttribArray(index);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}