#ifndef _ML_FONT_HPP_
#define _ML_FONT_HPP_

#include <libmeme/Renderer/Glyph.hpp>

namespace ml
{
	struct ML_RENDERER_API font final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;
		
		using page = typename ds::flat_map<uint32_t, glyph>;
		
		using page_table = typename ds::flat_map<uint32_t, page>;

		struct ML_NODISCARD info final
		{
			pmr::string family;
			std::locale locale;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		font(allocator_type const & alloc = {});
		
		font(fs::path const & path, allocator_type const & alloc = {});
		
		font(font const & other, allocator_type const & alloc = {});
		
		font(font && other, allocator_type const & alloc = {}) noexcept;
		
		~font();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		font & operator=(font const & other)
		{
			font temp{ other };
			swap(temp);
			return (*this);
		}

		font & operator=(font && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		void swap(font & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_pages, other.m_pages);
				std::swap(m_lib, other.m_lib);
				std::swap(m_face, other.m_face);
				std::swap(m_info, other.m_info);
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
		void *		m_lib;
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