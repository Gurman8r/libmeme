#include <libmeme/Graphics/BufferLayout.hpp>
#include <libmeme/Graphics/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void buffer_layout::attribute::apply() const noexcept
	{
		GL::vertexAttribPointer(index, size, type, normalize, stride, offset, width);
		
		GL::enableVertexAttribArray(index);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	buffer_layout const & buffer_layout::apply() const noexcept
	{
		for (auto const & e : (*this))
		{
			e.apply();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}