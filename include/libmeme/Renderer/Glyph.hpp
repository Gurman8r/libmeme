#ifndef _ML_GLYPH_HPP_
#define _ML_GLYPH_HPP_

#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct ML_RENDERER_API Glyph final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_t = typename std::tuple<Texture, FloatRect, uint32_t>;

		static constexpr const size_t count{ 6 };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Glyph();
		Glyph(storage_t const & storage);
		Glyph(storage_t && storage) noexcept;
		Glyph(Glyph const & other);
		Glyph(Glyph && other) noexcept;
		~Glyph();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Glyph operator=(Glyph const & other);

		Glyph operator=(Glyph && other) noexcept;

		void swap(Glyph & other);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto texture() const noexcept -> Texture const & { return std::get<0>(m_storage); }

		inline auto bounds() const noexcept -> FloatRect const & { return std::get<1>(m_storage); }

		inline auto advance() const noexcept -> uint32_t const & { return std::get<2>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto bearing() const noexcept -> vec2 { return bounds().position(); }
		
		inline auto size() const noexcept -> vec2 { return bounds().size(); }
		
		inline auto x() const noexcept -> float_t { return bearing()[0]; }
		
		inline auto y() const noexcept -> float_t { return bearing()[1]; }
		
		inline auto width() const noexcept -> float_t { return size()[0]; }
		
		inline auto height() const noexcept -> float_t { return size()[1]; }
		
		inline auto offset() const noexcept -> vec2 { return { x(), -y() }; }
		
		inline auto step() const noexcept -> float_t { return (float_t)(advance() >> count); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: union { storage_t m_storage; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> static inline auto make_glyph(Args && ... args)
	{
		return Glyph{ Glyph::storage_t{ std::forward<Args>(args)... } };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GLYPH_HPP_