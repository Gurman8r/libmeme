#ifndef _ML_GLYPH_HPP_
#define _ML_GLYPH_HPP_

#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct ML_RENDERER_API glyph final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		texture		graphic	{};
		float_rect	bounds	{};
		uint32_t	advance	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto bearing() const noexcept -> vec2 { return bounds.position(); }
		
		auto size() const noexcept -> vec2 { return bounds.size(); }
		
		auto left() const noexcept -> float_t { return bearing()[0]; }
		
		auto top() const noexcept -> float_t { return bearing()[1]; }
		
		auto width() const noexcept -> float_t { return size()[0]; }
		
		auto height() const noexcept -> float_t { return size()[1]; }
		
		auto offset() const noexcept -> vec2 { return { left(), -top() }; }
		
		auto step() const noexcept -> float_t { return (float_t)(advance >> 6); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Bounds, class Advance
	> ML_NODISCARD inline auto make_glyph(texture const & t, Bounds && b, Advance && a)
	{
		glyph g{};
		g.graphic	= t;
		g.bounds	= ML_forward(b);
		g.advance	= ML_forward(a);
		return g;
	}

	template <class Bounds, class Advance
	> ML_NODISCARD inline auto make_glyph(texture && t, Bounds && b, Advance && a)
	{
		glyph g{};
		g.graphic	= std::move(t);
		g.bounds	= ML_forward(b);
		g.advance	= ML_forward(a);
		return g;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GLYPH_HPP_