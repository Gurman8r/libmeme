#include <libmeme/Renderer/Glyph.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Glyph::Glyph() : Glyph{ std::make_tuple(
		make_texture(GL::Texture2D, GL::RGBA, GL::Red, TextureFlags_Smooth | TextureFlags_Repeated),
		FloatRect::zero(),
		0
	) }
	{
	}

	Glyph::Glyph(storage_t const & storage)
		: m_storage{ storage }
	{
	}

	Glyph::Glyph(storage_t && storage) noexcept
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

	Glyph::~Glyph() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Glyph Glyph::operator=(Glyph const & other)
	{
		assign(other);
		return (*this);
	}

	Glyph Glyph::operator=(Glyph && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Glyph::assign(Glyph const & other)
	{
		if (this != std::addressof(other))
		{
			m_storage = other.m_storage;
		}
	}

	void Glyph::swap(Glyph & other) noexcept
	{
		if (this != std::addressof(other))
		{
			m_storage.swap(other.m_storage);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}