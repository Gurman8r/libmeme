#include <libmeme/Renderer/Glyph.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Glyph::Glyph() : Glyph{ std::make_tuple(make_texture(
		GL::Texture2D, GL::RGBA, GL::Red, TextureFlags_Smooth | TextureFlags_Repeated),
		float_rect::zero(),
		0u
	) }
	{
	}

	Glyph::Glyph(storage_type const & storage)
		: m_storage{ storage }
	{
	}

	Glyph::Glyph(storage_type && storage) noexcept
		: m_storage{ std::move(storage) }
	{
	}

	Glyph::Glyph(Glyph const & other)
		: m_storage{ other.m_storage }
	{
	}

	Glyph::Glyph(Glyph && other) noexcept
		: m_storage{ std::move(other.m_storage) }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}