#ifndef _ML_DISPLAY_MODE_HPP_
#define _ML_DISPLAY_MODE_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct DisplayMode final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Size, ID_BitsPerPixel };

		using storage_type = typename std::tuple<
			vec2u,		// size
			uint32_t	// bits-per-pixel
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr DisplayMode() noexcept : m_storage{} {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr DisplayMode(storage_type const & value)
			: m_storage{ value }
		{
		}

		constexpr DisplayMode(storage_type && value) noexcept
			: m_storage{ std::move(value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr DisplayMode(vec2 const & size, uint32_t depth)
			: m_storage{ std::make_tuple(size, depth) }
		{
		}

		constexpr DisplayMode(vec2 && size, uint32_t depth) noexcept
			: m_storage{ std::make_tuple(std::move(size), depth) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr DisplayMode(DisplayMode const & other)
			: m_storage{ other.m_storage }
		{
		}

		constexpr DisplayMode(DisplayMode && other) noexcept
			: m_storage{ std::move(other.m_storage) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr DisplayMode & operator=(DisplayMode const & other)
		{
			DisplayMode temp{ other };
			this->swap(temp);
			return (*this);
		}

		constexpr DisplayMode & operator=(DisplayMode && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		constexpr void swap(DisplayMode & other) noexcept
		{
			if (this != std::addressof(other))
			{
				alg::swap(m_storage, other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr vec2u & resolution() noexcept
		{
			return std::get<ID_Size>(m_storage);
		}

		ML_NODISCARD constexpr vec2u const & resolution() const noexcept
		{
			return std::get<ID_Size>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr uint32_t & bits_per_pixel() noexcept
		{
			return std::get<ID_BitsPerPixel>(m_storage);
		}

		ML_NODISCARD constexpr uint32_t const & bits_per_pixel() const noexcept
		{
			return std::get<ID_BitsPerPixel>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr uint32_t & width() noexcept
		{
			return resolution()[0];
		}

		ML_NODISCARD constexpr uint32_t const & width() const noexcept
		{
			return resolution()[0];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr uint32_t & height() noexcept
		{
			return resolution()[1];
		}

		ML_NODISCARD constexpr uint32_t const & height() const noexcept
		{
			return resolution()[1];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr bool operator==(DisplayMode const & other) const noexcept
		{
			return !((*this) < other) && !(other < (*this));
		}

		ML_NODISCARD constexpr bool operator!=(DisplayMode const & other) const noexcept
		{
			return !((*this) == other);
		}

		ML_NODISCARD constexpr bool operator<(DisplayMode const & other) const noexcept
		{
			return (this->resolution() < other.resolution())
				|| (this->bits_per_pixel() < other.bits_per_pixel());
		}

		ML_NODISCARD constexpr bool operator>(DisplayMode const & other) const noexcept
		{
			return !((*this) < other);
		}

		ML_NODISCARD constexpr bool operator<=(DisplayMode const & other) const noexcept
		{
			return ((*this) == other) || ((*this) < other);
		}

		ML_NODISCARD constexpr bool operator>=(DisplayMode const & other) const noexcept
		{
			return ((*this) == other) || ((*this) > other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static constexpr auto make_display_mode(vec2u const & size, uint32_t depth)
	{
		return DisplayMode{ std::make_tuple(size, depth) };
	}

	ML_NODISCARD static constexpr auto make_display_mode(vec2u && size, uint32_t depth) noexcept
	{
		return DisplayMode{ std::make_tuple(std::move(size), depth) };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_display_mode(Args && ... args) noexcept
	{
		return DisplayMode{ std::make_tuple(std::forward<Args>(args)...) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DISPLAY_MODE_HPP_