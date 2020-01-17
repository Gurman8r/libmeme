#ifndef _ML_DISPLAY_SETTINGS_HPP_
#define _ML_DISPLAY_SETTINGS_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct VideoMode final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec2u		resolution	{ 0, 0 };
		uint32_t	color_depth	{ 0 };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr VideoMode() noexcept = default;
		constexpr VideoMode(VideoMode const &) = default;
		constexpr VideoMode(VideoMode &&) noexcept = default;
		constexpr VideoMode & operator=(VideoMode const &) = default;
		constexpr VideoMode & operator=(VideoMode &&) noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr bool operator==(VideoMode const & other) const
		{
			return (resolution == other.resolution)
				&& (color_depth == other.color_depth);
		}

		ML_NODISCARD constexpr bool operator!=(VideoMode const & other) const
		{
			return !(*this == other);
		}

		ML_NODISCARD constexpr bool operator<(VideoMode const & other) const
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
		return VideoMode{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DISPLAY_SETTINGS_HPP_