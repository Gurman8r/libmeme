#include <libmeme/Renderer/RenderTarget.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void RenderTarget::draw(VertexArrayObject const & vao, VertexBufferObject const & vbo) const
	{
		if (ML_BIND(VertexArrayObject, vao))
		{
			if (ML_BIND(VertexBufferObject, vbo))
			{
				GL::drawArrays(vao.mode(), 0, vbo.size());
		
				GL::flush();
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void RenderTarget::draw(VertexArrayObject const & vao, VertexBufferObject const & vbo, IndexBufferObject const & ibo) const
	{
		if (ML_BIND(VertexArrayObject, vao))
		{
			if (ML_BIND(VertexBufferObject, vbo))
			{
				if (ML_BIND(IndexBufferObject, ibo))
				{
					GL::drawElements(vao.mode(), ibo.count(), ibo.type(), nullptr);

					GL::flush();
				}
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}