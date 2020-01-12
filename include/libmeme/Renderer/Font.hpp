#ifndef _ML_FONT_HPP_
#define _ML_FONT_HPP_

#include <libmeme/Renderer/Glyph.hpp>

namespace ml
{
	struct ML_RENDERER_API Font final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Page = typename ds::pair_map<uint32_t, Glyph>;
		
		using PageTable = typename ds::pair_map<uint32_t, Page>;

		struct Info final
		{
			std::string family;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Font();
		Font(path_t const & path);
		Font(Font const & other);
		Font(Font && other) noexcept;
		~Font();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Font & operator=(Font const & other);

		Font & operator=(Font && other) noexcept;

		void swap(Font & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Glyph const & get_glyph(uint32_t c, uint32_t characterSize) const;

		Page & get_page(uint32_t characterSize) const;

		Glyph load_glyph(uint32_t c, uint32_t characterSize) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto info() const noexcept -> Info const & { return m_info; }

		inline auto pages() noexcept -> PageTable & { return m_pages; }

		inline auto pages() const noexcept -> PageTable const & { return m_pages; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void *	m_library;
		void *	m_face;
		Info	m_info;

		mutable PageTable m_pages;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> static inline auto make_font(Args && ... args)
	{
		return Font{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FONT_HPP_