#ifndef _ML_GLYPH_HPP_
#define _ML_GLYPH_HPP_

#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct ML_RENDERER_API Glyph final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Texture, ID_Bounds, ID_Advance };

		using storage_t = typename std::tuple<Texture, FloatRect, uint32_t>;

		static constexpr size_t const count{ 6 };

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

		void swap(Glyph & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline decltype(auto) texture() noexcept		{ return std::get<ID_Texture>(m_storage); }
		inline decltype(auto) texture() const noexcept	{ return std::get<ID_Texture>(m_storage); }

		inline decltype(auto) bounds() noexcept			{ return std::get<ID_Bounds>(m_storage); }
		inline decltype(auto) bounds() const noexcept	{ return std::get<ID_Bounds>(m_storage); }

		inline decltype(auto) advance() noexcept		{ return std::get<ID_Advance>(m_storage); }
		inline decltype(auto) advance() const noexcept	{ return std::get<ID_Advance>(m_storage); }

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

	private:
		storage_t m_storage;

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