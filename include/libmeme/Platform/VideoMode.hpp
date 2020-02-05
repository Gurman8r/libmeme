#ifndef _ML_DISPLAY_SETTINGS_HPP_
#define _ML_DISPLAY_SETTINGS_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct video_mode final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec2u		resolution	{ 0, 0 };
		uint32_t	color_depth	{ 0 };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr video_mode() noexcept = default;
		constexpr video_mode(video_mode const &) = default;
		constexpr video_mode(video_mode &&) noexcept = default;
		constexpr video_mode & operator=(video_mode const &) = default;
		constexpr video_mode & operator=(video_mode &&) noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr bool operator==(video_mode const & other) const
		{
			return (resolution == other.resolution)
				&& (color_depth == other.color_depth);
		}

		ML_NODISCARD constexpr bool operator!=(video_mode const & other) const
		{
			return !(*this == other);
		}

		ML_NODISCARD constexpr bool operator<(video_mode const & other) const
		{
			return (resolution < other.resolution)
				|| (color_depth < other.color_depth);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_video_mode(Args && ... args) noexcept
	{
		return video_mode{ ML_FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DISPLAY_SETTINGS_HPP_