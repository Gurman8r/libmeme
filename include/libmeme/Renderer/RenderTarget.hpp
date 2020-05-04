#ifndef _ML_RENDER_TARGET_HPP_
#define _ML_RENDER_TARGET_HPP_

#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Core/Color.hpp>
#include <libmeme/Core/Rect.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// object-oriented interface to renderer api
	struct ML_RENDERER_API render_target
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_target() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear_color(color const & col);

		void clear_color(color const & col, uint32_t flags);

		void clear_flags(uint32_t flags);

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

		void flush();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void viewport(vec2i const & size);

		void viewport(int_rect const & bounds);

		void viewport(vec2i const & pos, vec2i const & size);

		void viewport(int32_t x, int32_t y, int32_t w, int32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TARGET_HPP_