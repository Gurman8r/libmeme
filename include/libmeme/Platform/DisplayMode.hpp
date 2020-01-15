#ifndef _ML_DISPLAY_MODE_HPP_
#define _ML_DISPLAY_MODE_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	struct DisplayMode final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Size, ID_Depth };

		using storage_type = typename std::tuple<
			vec2u,		// size
			uint32_t	// depth
		>;

		storage_type m_storage;

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

		ML_NODISCARD constexpr vec2u & size() noexcept
		{
			return std::get<ID_Size>(m_storage);
		}

		ML_NODISCARD constexpr vec2u const & size() const noexcept
		{
			return std::get<ID_Size>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr uint32_t & depth() noexcept
		{
			return std::get<ID_Depth>(m_storage);
		}

		ML_NODISCARD constexpr uint32_t const & depth() const noexcept
		{
			return std::get<ID_Depth>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr uint32_t & width() noexcept
		{
			return size()[0];
		}

		ML_NODISCARD constexpr uint32_t const & width() const noexcept
		{
			return size()[0];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr uint32_t & height() noexcept
		{
			return size()[1];
		}

		ML_NODISCARD constexpr uint32_t const & height() const noexcept
		{
			return size()[1];
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
			return (this->size() < other.size()) || (this->depth() < other.depth());
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
	};
}

#endif // !_ML_DISPLAY_MODE_HPP_