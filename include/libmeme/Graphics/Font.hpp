#ifndef _ML_FONT_HPP_
#define _ML_FONT_HPP_

#include <libmeme/Graphics/Glyph.hpp>

namespace ml
{
	struct ML_GRAPHICS_API font final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;
		
		using page = typename ds::map<uint32_t, glyph>;
		
		using page_table = typename ds::map<uint32_t, page>;

		struct ML_NODISCARD info final
		{
			pmr::string family;
			std::locale locale;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		font(allocator_type const & alloc = {}) noexcept;
		
		font(fs::path const & path, allocator_type const & alloc = {});
		
		font(font const & value, allocator_type const & alloc = {});
		
		font(font && value, allocator_type const & alloc = {}) noexcept;
		
		~font() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		font & operator=(font const & value)
		{
			font temp{ value };
			swap(temp);
			return (*this);
		}

		font & operator=(font && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		void swap(font & value) noexcept
		{
			if (this != std::addressof(value))
			{
				std::swap(m_pages, value.m_pages);
				std::swap(m_library, value.m_library);
				std::swap(m_face, value.m_face);
				std::swap(m_info, value.m_info);
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

		ML_NODISCARD auto get_info() const noexcept -> info const & { return m_info; }

		ML_NODISCARD auto get_pages() const noexcept -> page_table const & { return m_pages; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void *		m_library;
		void *		m_face;
		info		m_info;
		page_table	m_pages;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD inline auto make_font(Args && ... args)
	{
		return font{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FONT_HPP_