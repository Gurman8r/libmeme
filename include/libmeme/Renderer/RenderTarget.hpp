#ifndef _ML_RENDER_TARGET_HPP_
#define _ML_RENDER_TARGET_HPP_

#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Renderer/Color.hpp>
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
		
		void draw(vertex_array const & vao, vertex_buffer const & vbo) const;

		void draw(vertex_array const & vao, vertex_buffer const & vbo, index_buffer const & ibo) const;

		template <class T> inline void draw(T const * value) const
		{
			T::draw(*this, value);
		}

		template <class T> inline void draw(T const & value) const
		{
			this->draw(&value);
		}
		
		void viewport(int_rect const & bounds) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TARGET_HPP_