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
		
		static void draw(VAO const & vao, VBO const & vbo);

		static void draw(VAO const & vao, VBO const & vbo, IBO const & ibo);

		template <class T
		> static void draw(T const * value)
		{
			T::draw(value);
		}

		template <class T
		> static void draw(T const & value)
		{
			render_target::draw(&value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_TARGET_HPP_