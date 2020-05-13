#ifndef _ML_VIDEO_MODE_HPP_
#define _ML_VIDEO_MODE_HPP_

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Core/Rect.hpp>
#include <libmeme/Core/FlatSet.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_PLATFORM_API ML_NODISCARD video_mode final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec2i		size	{ 640, 480 };
		uint32_t	depth	{ 32 };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator bool() const noexcept
		{
			return size.nonzero() && depth;
		}

		constexpr bool operator==(video_mode const & other) const noexcept
		{
			return (this == std::addressof(other))
				|| (size == other.size && depth == other.depth);
		}

		constexpr bool operator!=(video_mode const & other) const noexcept
		{
			return !(*this == other);
		}

		constexpr bool operator<(video_mode const & other) const noexcept
		{
			return (this != std::addressof(other))
				&& (size < other.size || depth < other.depth);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static video_mode const & get_desktop_mode();

		static ds::set<video_mode> const & get_fullscreen_modes();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline void from_json(json const & j, video_mode & value)
	{
		j.at("size").get_to(value.size);
		j.at("depth").get_to(value.depth);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VIDEO_MODE_HPP_