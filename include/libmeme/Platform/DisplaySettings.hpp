#ifndef _ML_DISPLAY_SETTINGS_HPP_
#define _ML_DISPLAY_SETTINGS_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct display_settings final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec2i		size {};
		uint32_t	depth{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator bool() const noexcept
		{
			return size[0] && size[1] && depth;
		}

		ML_NODISCARD constexpr bool operator==(display_settings const & value) const
		{
			return (size == value.size)
				&& (depth == value.depth);
		}

		ML_NODISCARD constexpr bool operator!=(display_settings const & value) const
		{
			return !(*this == value);
		}

		ML_NODISCARD constexpr bool operator<(display_settings const & value) const
		{
			return (size < value.size)
				|| (depth < value.depth);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static void from_json(json const & j, display_settings & value)
	{
		j.at("size").get_to(value.size);
		j.at("depth").get_to(value.depth);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DISPLAY_SETTINGS_HPP_