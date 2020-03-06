#ifndef _ML_GLYPH_HPP_
#define _ML_GLYPH_HPP_

#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct ML_RENDERER_API glyph final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		texture		graphic	{};
		float_rect	bounds	{ 0 };
		uint32_t	advance	{ 0 };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto bearing() const noexcept -> vec2 { return bounds.position(); }
		
		inline auto size() const noexcept -> vec2 { return bounds.size(); }
		
		inline auto left() const noexcept -> float_t { return bearing()[0]; }
		
		inline auto top() const noexcept -> float_t { return bearing()[1]; }
		
		inline auto width() const noexcept -> float_t { return size()[0]; }
		
		inline auto height() const noexcept -> float_t { return size()[1]; }
		
		inline auto offset() const noexcept -> vec2 { return { left(), -top() }; }
		
		inline auto step() const noexcept -> float_t { return (float_t)(advance >> 6); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Bounds, class Advance
	> ML_NODISCARD static inline auto make_glyph(texture && t, Bounds && b, Advance && a) noexcept
	{
		glyph g;
		g.graphic = ML_forward(t);
		g.bounds = ML_forward(b);
		g.advance = ML_forward(a);
		return g;
	}

	template <class ... Args
	> ML_NODISCARD static inline auto make_glyph(Args && ... args)
	{
		return glyph{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GLYPH_HPP_