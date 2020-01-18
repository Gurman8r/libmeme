#ifndef _ML_FONT_HPP_
#define _ML_FONT_HPP_

#include <libmeme/Renderer/Glyph.hpp>

namespace ml
{
	struct ML_RENDERER_API Font final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using glyph_page		= typename ds::flat_map<uint32_t, Glyph>;
		using page_table		= typename ds::flat_map<uint32_t, glyph_page>;

		struct Info
		{
			std::string family;
			std::locale locale;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Font();
		explicit Font(allocator_type const & alloc);
		Font(path_t const & path, allocator_type const & alloc = {});
		Font(Font const & other, allocator_type const & alloc = {});
		Font(Font && other, allocator_type const & alloc = {}) noexcept;
		~Font();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Font & operator=(Font const & other);

		Font & operator=(Font && other) noexcept;

		void swap(Font & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD Glyph const & get_glyph(uint32_t c, uint32_t characterSize) const;

		ML_NODISCARD Glyph load_glyph(uint32_t c, uint32_t characterSize) const;

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
		return Font{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FONT_HPP_