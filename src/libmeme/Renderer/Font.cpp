#include <libmeme/Renderer/Font.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Font::Font()
		: m_pages	{}
		, m_library	{ nullptr }
		, m_face	{ nullptr }
	{
	}

	Font::Font(path_t const & path)
		: Font{}
	{
		load_from_file(path);
	}

	Font::Font(Font const & other)
		: m_pages	{ other.m_pages }
		, m_library	{ other.m_library }
		, m_face	{ other.m_face }
	{
	}

	Font::Font(Font && other) noexcept
		: Font{}
	{
		swap(std::move(other));
	}

	Font::~Font()
	{
		if (!m_pages.empty()) { m_pages.clear(); }

		if (m_face) { FT_Done_Face(static_cast<FT_Face>(m_face)); }

		if (m_library) { FT_Done_FreeType(static_cast<FT_Library>(m_library)); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Font & Font::operator=(Font const & other)
	{
		Font temp{ other };
		swap(temp);
		return (*this);
	}

	Font & Font::operator=(Font && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void Font::swap(Font & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_pages, other.m_pages);
			std::swap(m_library, other.m_library);
			std::swap(m_face, other.m_face);
			std::swap(m_info, other.m_info);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Font::load_from_file(path_t const & path)
	{
		if (m_library) return false;

		FT_Library library;
		if (FT_Init_FreeType(&library) != 0)
		{
			return Debug::logError(
				"Failed loading font \"{0}\" (failed to open FreeType)",
				path
			);
		}
		m_library = library;

		// Load the new fonts face from the specified file
		FT_Face face;
		if (FT_New_Face(static_cast<FT_Library>(m_library), path.string().c_str(), 0, &face) != 0)
		{
			return Debug::logError(
				"Failed loading font \"{0}\" (failed to create the font face)",
				path
			);
		}

		// Load the stroker that will be used to outline the fonts
		FT_Stroker stroker;
		if (FT_Stroker_New(static_cast<FT_Library>(m_library), &stroker) != 0)
		{
			FT_Done_Face(face);
			return Debug::logError(
				"Failed loading font \"{0}\" (failed to create the stroker)",
				path
			);
		}

		// Select the unicode character map
		if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0)
		{
			FT_Stroker_Done(stroker);
			FT_Done_Face(face);
			return Debug::logError(
				"Failed loading font \"{0}\" (failed to set the Unicode character set)",
				path
			);
		}

		// Store the loaded fonts
		m_face = face;

		// Store the fonts information
		m_info.family = face->family_name ? face->family_name : std::string{};

		return m_library;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Glyph const & Font::get_glyph(uint32_t c, uint32_t characterSize) const
	{
		glyph_page & page{ m_pages[characterSize] };
		
		if (auto it{ page.find(c) })
		{
			return (**it);
		}
		else
		{
			return (*page.try_emplace(c, load_glyph(c, characterSize)).first.second);
		}
	}

	Glyph Font::load_glyph(uint32_t c, uint32_t characterSize) const
	{
		Glyph glyph;

		FT_Face face;
		if (!(face = static_cast<FT_Face>(m_face)))
		{
			return glyph;
		}

		// Set size loading glyphs as
		FT_Set_Pixel_Sizes(face, 0, characterSize);

		// Disable byte-alignment restriction
		GL::pixelStore(GL::UnpackAlignment, 1);

		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
		{
			Debug::logWarning("Failed loading Glyph \'{0}\'", (char)c);
			return glyph;
		}

		glyph.bounds() = FloatRect{
			(float_t)face->glyph->bitmap_left,
			(float_t)face->glyph->bitmap_top,
			(float_t)face->glyph->bitmap.width,
			(float_t)face->glyph->bitmap.rows
		};

		glyph.advance() = (uint32_t)face->glyph->advance.x;

		// Only load a texture for characters requiring a graphic
		if ((c != ' ') && std::isgraph(c))
		{
			if (!glyph.texture().create(face->glyph->bitmap.buffer, (vec2u)glyph.size()))
			{
				Debug::logWarning("Failed Loading Glyph Texture: \'{0}\'", (char)c);
			}
		}

		return glyph;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}