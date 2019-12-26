#include <libmeme/Renderer/RenderTarget.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void RenderTarget::draw(VAO const & vao, VBO const & vbo) const
	{
		if (ML_BIND(VAO, vao))
		{
			if (ML_BIND(VBO, vbo))
			{
				GL::drawArrays(vao.mode(), 0, vbo.size());

				GL::flush();
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void RenderTarget::draw(VAO const & vao, VBO const & vbo, IBO const & ibo) const
	{
		if (ML_BIND(VAO, vao))
		{
			if (ML_BIND(VBO, vbo))
			{
				if (ML_BIND(IBO, ibo))
				{
					GL::drawElements(vao.mode(), ibo.count(), ibo.type(), nullptr);

					GL::flush();
				}
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}