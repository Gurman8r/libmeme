#ifndef _ML_GLYPH_HPP_
#define _ML_GLYPH_HPP_

#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct ML_RENDERER_API Glyph final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture		texture	{};
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

	template <class ... Args
	> ML_NODISCARD static inline auto make_glyph(Args && ... args)
	{
		return Glyph{ std::forward<Args>(args)... };
	}

	template <class Bounds, class Advance
	> ML_NODISCARD static inline auto make_glyph(Texture const & t, Bounds && b, Advance && a)
	{
		Glyph g;
		g.texture = t;
		g.bounds = std::move(b);
		g.advance = std::move(a);
		return g;
	}

	template <class Bounds, class Advance
	> ML_NODISCARD static inline auto make_glyph(Texture && t, Bounds && b, Advance && a) noexcept
	{
		Glyph g;
		g.texture = std::move(t);
		g.bounds = std::move(b);
		g.advance = std::move(a);
		return g;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GLYPH_HPP_