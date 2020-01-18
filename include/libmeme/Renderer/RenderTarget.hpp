#ifndef _ML_RENDER_TARGET_HPP_
#define _ML_RENDER_TARGET_HPP_

#include <libmeme/Renderer/Buffers.hpp>
#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Core/Rect.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API RenderTarget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~RenderTarget() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear_color(Color const & color) const;

		void clear_color(Color const & color, uint32_t flags) const;

		void clear_flags(uint32_t flags) const;
		
		void draw(VAO const & vao, VBO const & vbo) const;

		void draw(VAO const & vao, VBO const & vbo, IBO const & ibo) const;

		template <class T> inline void draw(T const * value) const
		{
			T::draw(*this, value);
		}

		template <class T> inline void draw(T const & value) const
		{
			this->draw(&value);
		}
		
		void viewport(IntRect const & bounds) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TARGET_HPP_