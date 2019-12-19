#ifndef _ML_DISPLAY_MODE_HPP_
#define _ML_DISPLAY_MODE_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	struct DisplayMode final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec2u	 size;
		uint32_t depth;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit DisplayMode(vec2u const & size, uint32_t depth)
			: size{ size }, depth{ depth }
		{
		}

		constexpr DisplayMode(uint32_t width, uint32_t height, uint32_t depth)
			: DisplayMode{ { width, height }, depth }
		{
		}

		constexpr DisplayMode(DisplayMode const & copy)
			: DisplayMode{ copy.size, copy.depth }
		{
		}

		constexpr DisplayMode()
			: DisplayMode{ { 0, 0 }, 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto width()  const -> uint32_t const & { return size[0]; }

		constexpr auto height() const -> uint32_t const & { return size[1]; }

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