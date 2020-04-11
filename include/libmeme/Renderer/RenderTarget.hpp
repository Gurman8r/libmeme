#ifndef _ML_RENDER_TARGET_HPP_
#define _ML_RENDER_TARGET_HPP_

#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Core/Color.hpp>
#include <libmeme/Core/Rect.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API render_target
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_target() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear_color(color const & col) const;

		void clear_color(color const & col, uint32_t flags) const;

		void clear_flags(uint32_t flags) const;
		
		void draw(VAO const & vao, VBO const & vbo) const;

		void draw(VAO const & vao, VBO const & vbo, IBO const & ibo) const;
		
		void viewport(int_rect const & bounds) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ... Args
		> void draw(T const * value, Args && ... args) const noexcept
		{
			T::draw(*this, value, ML_forward(args)...);
		}

		template <class T, class ... Args
		> void draw(T const & value, Args && ... args) const noexcept
		{
			this->draw(&value, ML_forward(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TARGET_HPP_