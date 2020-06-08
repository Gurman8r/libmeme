#include <libmeme/Graphics/Font.hpp>
#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	font::~font() noexcept
	{
		if (m_face) { FT_Done_Face((FT_Face)m_face); }

		if (m_library) { FT_Done_FreeType((FT_Library)m_library); }

		if (m_stroker) { FT_Stroker_Done((FT_Stroker)m_stroker); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool font::load_from_file(fs::path const & path)
	{
		if (m_library) { return false; }

		// load freetype library instance
		if (FT_Init_FreeType((FT_Library *)&m_library) != 0)
		{
			return debug::error("failed initializing FreeType: {0}", path);
		}

		// load the new fonts face from the specified file
		if (FT_New_Face((FT_Library)m_library, path.string().c_str(), 0, (FT_Face *)&m_face) != 0)
		{
			return debug::error("failed creating font face: {0}", path);
		}

		// load the stroker that will be used to outline the fonts
		if (FT_Stroker_New((FT_Library)m_library, (FT_Stroker *)&m_stroker) != 0)
		{
			FT_Done_Face((FT_Face)m_face);
			return debug::error("failed creating font stroker: {0}", path);
		}

		// select the unicode character map
		if (FT_Select_Charmap((FT_Face)m_face, FT_ENCODING_UNICODE) != 0)
		{
			FT_Stroker_Done((FT_Stroker)m_stroker);
			FT_Done_Face((FT_Face)m_face);
			return debug::error("failed selecting font unicode character map: {0}", path);
		}

		// store the font family
		m_family = ((FT_Face)m_face)->family_name;

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	glyph font::load_glyph(uint32_t c, uint32_t size)
	{
		glyph g{};

		// face not loaded
		if (!m_face) { return g; }

		// set size loading glyphs as
		FT_Set_Pixel_Sizes(((FT_Face)m_face), 0, size);

		// load character glyph
		if (FT_Load_Char(((FT_Face)m_face), c, FT_LOAD_RENDER) != 0)
		{
			debug::warning("font failed loading glyph: \'{0}\'", c);
			
			return g;
		}

		// graphic
		g.graphic = gfx::texture2d::allocate({
			vec2i{},
			{ gfx::format_rgba, gfx::format_red },
			gfx::type_unsigned_byte,
			gfx::texture_flags_default });

		// bounds
		g.bounds = float_rect
		{
			((FT_Face)m_face)->glyph->bitmap_left,
			((FT_Face)m_face)->glyph->bitmap_top,
			((FT_Face)m_face)->glyph->bitmap.width,
			((FT_Face)m_face)->glyph->bitmap.rows
		};

		// advance
		g.advance = (uint32_t)((FT_Face)m_face)->glyph->advance.x;

		// only load a texture for characters requiring a graphic
		if (!std::isspace(c, {}) && std::isgraph(c, {}))
		{
			g.graphic->update((vec2i)g.size(), ((FT_Face)m_face)->glyph->bitmap.buffer);
		}

		return g;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}