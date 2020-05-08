#include <libmeme/Graphics/Font.hpp>
#include <libmeme/Graphics/GL.hpp>

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

	font::font(allocator_type const & alloc) noexcept
		: m_pages	{ alloc }
		, m_info	{ pmr::string{ alloc }, {} }
		, m_lib		{}
		, m_face	{}
	{
	}

	font::font(fs::path const & path, allocator_type const & alloc)
		: font{ alloc }
	{
		load_from_file(path);
	}

	font::font(font const & value, allocator_type const & alloc)
		: m_pages	{ value.m_pages, alloc }
		, m_info	{ { value.m_info.family, alloc }, value.m_info.locale }
		, m_lib		{ value.m_lib }
		, m_face	{ value.m_face }
	{
	}

	font::font(font && value, allocator_type const & alloc) noexcept
		: font{ alloc }
	{
		swap(std::move(value));
	}

	font::~font() noexcept
	{
		if (!m_pages.empty()) { m_pages.clear(); }

		if (m_face) { FT_Done_Face(static_cast<FT_Face>(m_face)); }

		if (m_lib) { FT_Done_FreeType(static_cast<FT_Library>(m_lib)); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool font::load_from_file(fs::path const & path)
	{
		if (m_lib) { return false; }

		// load freetype library instance
		FT_Library library;
		if (FT_Init_FreeType(&library) != 0)
		{
			return debug::error("failed initializing FreeType: {0}", path);
		}

		// load the new fonts face from the specified file
		FT_Face face;
		if (FT_New_Face(library, path.string().c_str(), 0, &face) != 0)
		{
			return debug::error("failed creating font face: {0}", path);
		}

		// load the stroker that will be used to outline the fonts
		FT_Stroker stroker;
		if (FT_Stroker_New(library, &stroker) != 0)
		{
			FT_Done_Face(face);
			return debug::error("failed creating font stroker: {0}", path);
		}

		// select the unicode character map
		if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0)
		{
			FT_Stroker_Done(stroker);
			FT_Done_Face(face);
			return debug::error("failed selecting font unicode character map: {0}", path);
		}

		// store loaded font library
		m_lib = library;

		// store the font faces
		m_face = face;

		// store the font information
		m_info.family = face->family_name;

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	glyph font::load_glyph(uint32_t c, uint32_t size)
	{
		glyph g{};

		auto face{ static_cast<FT_Face>(m_face) };
		
		// face not loaded
		if (!face) { return g; }

		// set size loading glyphs as
		FT_Set_Pixel_Sizes(face, 0, size);

		// disable byte-alignment restriction
		GL::pixelStore(GL::UnpackAlignment, 1);

		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
		{
			debug::warning("font failed loading  glyph \'{0}\'", (char)c);
			
			return g;
		}

		// set texture
		g.graphic = make_texture(GL::Texture2D, GL::RGBA, GL::Red, texture_flags_default);

		// set bounds
		g.bounds = float_rect{
			face->glyph->bitmap_left,
			face->glyph->bitmap_top,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows
		};

		// set advance
		g.advance = (uint32_t)face->glyph->advance.x;
		
		// only load a texture for characters requiring a graphic
		if (!std::isspace(c, m_info.locale) && std::isgraph(c, m_info.locale))
		{
			if (!g.graphic.create(face->glyph->bitmap.buffer, (vec2u)g.size()))
			{
				debug::warning("font failed loading glyph texture: {0}", c);

				g.graphic.destroy();
			}
		}

		return g;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}