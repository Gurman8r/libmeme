#ifndef _ML_RENDER_TARGET_HPP_
#define _ML_RENDER_TARGET_HPP_

#include <libmeme/Renderer/Renderer.hpp>

namespace ml
{
	struct ML_RENDERER_API render_target
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_target() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
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