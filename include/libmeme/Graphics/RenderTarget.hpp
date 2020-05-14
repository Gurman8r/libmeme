#ifndef _ML_RENDER_TARGET_HPP_
#define _ML_RENDER_TARGET_HPP_

// WIP

#include <libmeme/Graphics/Buffers.hpp>
#include <libmeme/Core/Color.hpp>
#include <libmeme/Core/Rect.hpp>

namespace ml
{
	struct ML_GRAPHICS_API render_target
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_target() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		void draw(VAO const & vao, VBO const & vbo);

		void draw(VAO const & vao, VBO const & vbo, IBO const & ibo);

		template <class T> void draw(T const * value)
		{
			T::draw(*this, value);
		}

		template <class T> void draw(T const & value)
		{
			this->draw(&value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_TARGET_HPP_