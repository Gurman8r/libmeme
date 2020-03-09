#ifndef _ML_DISPLAY_SETTINGS_HPP_
#define _ML_DISPLAY_SETTINGS_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct display_settings final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec2i		resolution	{};
		uint32_t	color_depth	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator bool() const noexcept
		{
			return resolution[0] && resolution[1] && color_depth;
		}

		ML_NODISCARD constexpr bool operator==(display_settings const & other) const
		{
			return (resolution == other.resolution)
				&& (color_depth == other.color_depth);
		}

		ML_NODISCARD constexpr bool operator!=(display_settings const & other) const
		{
			return !(*this == other);
		}

		ML_NODISCARD constexpr bool operator<(display_settings const & other) const
		{
			return (resolution < other.resolution)
				|| (color_depth < other.color_depth);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static void from_json(json const & j, display_settings & value)
	{
		j.at("resolution").get_to(value.resolution);
		j.at("color_depth").get_to(value.color_depth);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DISPLAY_SETTINGS_HPP_