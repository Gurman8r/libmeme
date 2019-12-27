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
		virtual ~RenderTarget() = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline void draw(T const * value) const
		{
			T::draw((*this), value);
		}

		template <class T> inline void draw(T const & value) const
		{
			this->draw(&value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(VAO const & vao, VBO const & vbo) const;

		void draw(VAO const & vao, VBO const & vbo, IBO const & ibo) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TARGET_HPP_