#ifndef _ML_FONT_HPP_
#define _ML_FONT_HPP_

#include <libmeme/Graphics/Glyph.hpp>

namespace ml
{
	struct ML_GRAPHICS_API font final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using page				= typename ds::map<uint32_t, glyph>;
		using page_table		= typename ds::map<uint32_t, page>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		font(allocator_type alloc = {}) noexcept
			: m_pages	{ alloc }
			, m_family	{ alloc }
			, m_library	{}
			, m_face	{}
		{
		}

		font(fs::path const & path, allocator_type alloc = {}) : font{ alloc }
		{
			load_from_file(path);
		}

		font(font const & value, allocator_type alloc = {})
			: m_pages	{ value.m_pages, alloc }
			, m_family	{ value.m_family, alloc }
			, m_library	{ value.m_library }
			, m_face	{ value.m_face }
		{
		}

		font(font && value, allocator_type alloc = {}) noexcept : font{ alloc }
		{
			swap(std::move(value));
		}
		
		~font() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		font & operator=(font const & value)
		{
			font temp{ value };
			this->swap(temp);
			return (*this);
		}

		font & operator=(font && value) noexcept
		{
			this->swap(std::move(value));
			return (*this);
		}

		void swap(font & value) noexcept
		{
			if (this != std::addressof(value))
			{
				std::swap(m_library, value.m_library);
				std::swap(m_face, value.m_face);
				std::swap(m_family, value.m_family);
				m_pages.swap(value.m_pages);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD glyph load_glyph(uint32_t c, uint32_t size);

		ML_NODISCARD glyph & get_glyph(uint32_t c, uint32_t size)
		{
			return m_pages[size].find_or_add_fn(c, &font::load_glyph, this, c, size);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto family() const noexcept -> pmr::string const & { return m_family; }

		ML_NODISCARD auto pages() const noexcept -> page_table const & { return m_pages; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void *		m_library;
		void *		m_face;
		pmr::string	m_family;
		page_table	m_pages;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_FONT_HPP_