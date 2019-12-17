#ifndef _ML_FONT_HPP_
#define _ML_FONT_HPP_

#include <libmeme/Renderer/Glyph.hpp>

namespace ml
{
	struct ML_RENDERER_API Font final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Font();
		~Font();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> static inline auto make_font(Args && ... args)
	{
		return Font{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FONT_HPP_