#ifndef _ML_GLYPH_HPP_
#define _ML_GLYPH_HPP_

#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct ML_RENDERER_API Glyph final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Texture, ID_Bounds, ID_Advance };

		using storage_type = typename std::tuple<Texture, FloatRect, uint32_t>;

		static constexpr size_t const count{ 6 };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Glyph();

		explicit Glyph(storage_type const & storage)
			: m_storage{ storage }
		{
		}

		explicit Glyph(storage_type && storage) noexcept
			: m_storage{ std::move(storage) }
		{
		}

		Glyph(Glyph const & other)
			: m_storage{ other.m_storage }
		{
		}

		Glyph(Glyph && other) noexcept
			: m_storage{ std::move(other.m_storage) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Glyph & operator=(Glyph const & other)
		{
			Glyph temp{ other };
			swap(temp);
			return (*this);
		}

		inline Glyph & operator=(Glyph && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(Glyph & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline decltype(auto) texture() noexcept
		{
			return std::get<ID_Texture>(m_storage);
		}

		ML_NODISCARD inline decltype(auto) texture() const noexcept
		{
			return std::get<ID_Texture>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline decltype(auto) bounds() noexcept
		{
			return std::get<ID_Bounds>(m_storage);
		}

		ML_NODISCARD inline decltype(auto) bounds() const noexcept
		{
			return std::get<ID_Bounds>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline decltype(auto) advance() noexcept
		{
			return std::get<ID_Advance>(m_storage);
		}

		ML_NODISCARD inline decltype(auto) advance() const noexcept
		{
			return std::get<ID_Advance>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto bearing() const noexcept -> vec2 { return bounds().position(); }
		
		ML_NODISCARD inline auto size() const noexcept -> vec2 { return bounds().size(); }
		
		ML_NODISCARD inline auto x() const noexcept -> float_t { return bearing()[0]; }
		
		ML_NODISCARD inline auto y() const noexcept -> float_t { return bearing()[1]; }
		
		ML_NODISCARD inline auto width() const noexcept -> float_t { return size()[0]; }
		
		ML_NODISCARD inline auto height() const noexcept -> float_t { return size()[1]; }
		
		ML_NODISCARD inline auto offset() const noexcept -> vec2 { return { x(), -y() }; }
		
		ML_NODISCARD inline auto step() const noexcept -> float_t { return (float_t)(advance() >> count); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static inline auto make_glyph(Args && ... args)
	{
		return Glyph{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GLYPH_HPP_