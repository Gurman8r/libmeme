#include <libmeme/Renderer/Glyph.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Glyph::Glyph() : Glyph{ std::make_tuple(make_texture(
		GL::Texture2D, GL::RGBA, GL::Red, TextureFlags_Smooth | TextureFlags_Repeated),
		FloatRect::zero(),
		0U
	) }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}