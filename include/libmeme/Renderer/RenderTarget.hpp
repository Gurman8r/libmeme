#ifndef _ML_RENDER_TARGET_HPP_
#define _ML_RENDER_TARGET_HPP_

#include <libmeme/Renderer/Buffers.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API RenderTarget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderTarget() noexcept = default;

		virtual ~RenderTarget() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(VAO const & vao, VBO const & vbo) const;

		void draw(VAO const & vao, VBO const & vbo, IBO const & ibo) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TARGET_HPP_