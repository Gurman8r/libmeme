#ifndef _ML_FONT_HPP_
#define _ML_FONT_HPP_

#include <libmeme/Renderer/Glyph.hpp>

namespace ml
{
	struct ML_RENDERER_API font final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;
		
		using glyph_page = typename ds::flat_map<uint32_t, glyph>;
		
		using page_table = typename ds::flat_map<uint32_t, glyph_page>;

		struct Info
		{
			pmr::string family;
			std::locale locale;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		font();
		
		explicit font(allocator_type const & alloc);
		
		font(path_t const & path, allocator_type const & alloc = {});
		
		font(font const & other, allocator_type const & alloc = {});
		
		font(font && other, allocator_type const & alloc = {}) noexcept;
		
		~font();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		font & operator=(font const & other);

		font & operator=(font && other) noexcept;

		void swap(font & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD glyph const & get_glyph(uint32_t c, uint32_t char_size) const;

		ML_NODISCARD glyph load_glyph(uint32_t c, uint32_t char_size) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto info() const noexcept -> Info const & { return m_info; }

		ML_NODISCARD inline auto pages() noexcept -> page_table & { return m_pages; }

		ML_NODISCARD inline auto pages() const noexcept -> page_table const & { return m_pages; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void *	m_library;
		void *	m_face;
		Info	m_info;
		mutable page_table m_pages;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static inline auto make_font(Args && ... args)
	{
		return font{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FONT_HPP_