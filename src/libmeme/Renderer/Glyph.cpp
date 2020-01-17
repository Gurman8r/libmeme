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
		: m_storage{}
	{
		swap(std::move(other));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Glyph Glyph::operator=(Glyph const & other)
	{
		Glyph temp{ other };
		swap(temp);
		return (*this);
	}

	Glyph Glyph::operator=(Glyph && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void Glyph::swap(Glyph & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_storage, other.m_storage);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}