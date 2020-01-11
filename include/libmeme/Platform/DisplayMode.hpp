#ifndef _ML_DISPLAY_MODE_HPP_
#define _ML_DISPLAY_MODE_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	struct DisplayMode final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec2u size{ 0, 0 };
		
		uint32_t depth{ 0 };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto width() const noexcept -> uint32_t const & { return size[0]; }

		constexpr auto height() const noexcept -> uint32_t const & { return size[1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bool operator==(DisplayMode const & other) const { return !((*this) < other) && !(other < (*this)); }

		constexpr bool operator!=(DisplayMode const & other) const { return !((*this) == other); }

		constexpr bool operator<(DisplayMode const & other) const { return (this->size < other.size) && (this->depth < other.depth); }

		constexpr bool operator>(DisplayMode const & other) const { return !((*this) < other); }

		constexpr bool operator<=(DisplayMode const & other) const { return ((*this) == other) || ((*this) < other); }

		constexpr bool operator>=(DisplayMode const & other) const { return ((*this) == other) || ((*this) > other); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_DISPLAY_MODE_HPP_