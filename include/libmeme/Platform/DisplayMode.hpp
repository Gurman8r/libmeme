#ifndef _ML_DISPLAY_MODE_HPP_
#define _ML_DISPLAY_MODE_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	struct DisplayMode final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Size, ID_Depth };

		using storage_type = typename std::tuple<vec2u, uint32_t>;

		std::tuple<vec2u, uint32_t> m_storage;

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

		ML_NODISCARD constexpr uint32_t const & width() const noexcept
		{
			return size()[0];
		}

		ML_NODISCARD constexpr uint32_t const & height() const noexcept
		{
			return size()[1];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr bool operator==(DisplayMode const & other) const
		{
			return !((*this) < other) && !(other < (*this));
		}

		ML_NODISCARD constexpr bool operator!=(DisplayMode const & other) const
		{
			return !((*this) == other);
		}

		ML_NODISCARD constexpr bool operator<(DisplayMode const & other) const
		{
			return (this->size() < other.size()) || (this->depth() < other.depth());
		}

		ML_NODISCARD constexpr bool operator>(DisplayMode const & other) const
		{
			return !((*this) < other);
		}

		ML_NODISCARD constexpr bool operator<=(DisplayMode const & other) const
		{
			return ((*this) == other) || ((*this) < other);
		}

		ML_NODISCARD constexpr bool operator>=(DisplayMode const & other) const
		{
			return ((*this) == other) || ((*this) > other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD constexpr auto make_display_mode(Args && ... args)
	{
		return DisplayMode{ std::make_tuple(std::forward<Args>(args)...) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DISPLAY_MODE_HPP_