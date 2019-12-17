#ifndef _ML_GLYPH_HPP_
#define _ML_GLYPH_HPP_

#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct ML_RENDERER_API Glyph final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Size = 6 };

		Texture		texture { GL::Texture2D, GL::RGBA, GL::Red, TextureFlags_Smooth | TextureFlags_Repeated };
		FloatRect	bounds	{ FloatRect::zero() };
		uint32_t	advance { 0 };

		Glyph() = default;

		inline auto bearing()	const -> vec2		{ return bounds.position(); }
		inline auto size()		const -> vec2		{ return bounds.size(); }
		inline auto x()			const -> float_t	{ return bearing()[0]; }
		inline auto y()			const -> float_t	{ return bearing()[1]; }
		inline auto width()		const -> float_t	{ return size()[0]; }
		inline auto height()	const -> float_t	{ return size()[1]; }
		inline auto offset()	const -> vec2		{ return { x(), -y() }; }
		inline auto step()		const -> float_t	{ return (float_t)(advance >> Size); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> static inline auto make_glyph(Args && ... args)
	{
		return Glyph{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GLYPH_HPP_