#include <libmeme/Renderer/Font.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Core/Debug.hpp>

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

	font::font()
		: m_pages	{}
		, m_info	{}
		, m_library	{ nullptr }
		, m_face	{ nullptr }
	{
	}

	font::font(allocator_type const & alloc)
		: m_pages	{ alloc }
		, m_info	{ pmr::string{ alloc }, {} }
		, m_library	{ nullptr }
		, m_face	{ nullptr }
	{
	}

	font::font(path_t const & path, allocator_type const & alloc)
		: m_pages	{ alloc }
		, m_info	{ pmr::string{ alloc }, {} }
		, m_library	{ nullptr }
		, m_face	{ nullptr }
	{
		load_from_file(path);
	}

	font::font(font const & other, allocator_type const & alloc)
		: m_pages	{ other.m_pages, alloc }
		, m_info	{ { other.m_info.family, alloc }, other.m_info.locale }
		, m_library	{ other.m_library }
		, m_face	{ other.m_face }
	{
	}

	font::font(font && other, allocator_type const & alloc) noexcept
		: m_pages	{ alloc }
		, m_info	{ pmr::string{ alloc }, {} }
		, m_library	{ nullptr }
		, m_face	{ nullptr }
	{
		swap(std::move(other));
	}

	font::~font()
	{
		if (!m_pages.empty()) { m_pages.clear(); }

		if (m_face) { FT_Done_Face(static_cast<FT_Face>(m_face)); }

		if (m_library) { FT_Done_FreeType(static_cast<FT_Library>(m_library)); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	font & font::operator=(font const & other)
	{
		font temp{ other };
		swap(temp);
		return (*this);
	}

	font & font::operator=(font && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void font::swap(font & other) noexcept
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

	bool font::load_from_file(path_t const & path)
	{
		if (m_library)
			return false;

		// load freetype library instance
		FT_Library library;
		if (FT_Init_FreeType(&library) != 0)
		{
			return debug::log_error(
				"Failed loading font \"{0}\" (failed to open FreeType)",
				path
			);
		}

		// load the new fonts face from the specified file
		FT_Face face;
		if (FT_New_Face(library, path.string().c_str(), 0, &face) != 0)
		{
			return debug::log_error("failed creating font face: {0}", path);
		}

		// load the stroker that will be used to outline the fonts
		FT_Stroker stroker;
		if (FT_Stroker_New(library, &stroker) != 0)
		{
			FT_Done_Face(face);
			return debug::log_error("failed creating font stroker: {0}", path);
		}

		// select the unicode character map
		if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0)
		{
			FT_Stroker_Done(stroker);
			FT_Done_Face(face);
			return debug::log_error("failed selecting font unicode character map: {0}", path);
		}

		// store loaded font library
		m_library = library;

		// store the font faces
		m_face = face;

		// store the font information
		m_info.family = face->family_name;

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	glyph const & font::get_glyph(uint32_t c, uint32_t char_size) const
	{
		glyph_page & page{ m_pages[char_size] };
		
		if (auto it{ page.find(c) })
		{
			return (*it->second);
		}
		else
		{
			return (*page.insert(c, load_glyph(c, char_size)).first.second);
		}
	}

	glyph font::load_glyph(uint32_t c, uint32_t char_size) const
	{
		// face not loaded
		FT_Face face{ static_cast<FT_Face>(m_face) };
		if (!face)
		{
			return make_glyph();
		}

		// set size loading glyphs as
		FT_Set_Pixel_Sizes(face, 0, char_size);

		// disable byte-alignment restriction
		GL::pixelStore(GL::UnpackAlignment, 1);

		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
		{
			debug::log_warning("font failed loading  glyph \'{0}\'", (char)c);
			
			return make_glyph();
		}

		// create the glyph
		glyph g = make_glyph(
			make_texture(
				GL::Texture2D,
				GL::RGBA,
				GL::Red,
				TextureFlags_Default
			),
			float_rect{
				face->glyph->bitmap_left,
				face->glyph->bitmap_top,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows
			},
			(uint32_t)face->glyph->advance.x
		);
		
		// only load a texture for characters requiring a graphic
		if (!std::isspace(c, m_info.locale) && std::isgraph(c, m_info.locale))
		{
			if (!g.graphic.create(face->glyph->bitmap.buffer, (vec2u)g.size()))
			{
				debug::log_warning("font failed loading glyph texture: \'{0}\'", (char)c);
			}
		}

		return g;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}