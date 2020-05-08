#ifndef _ML_VIDEO_MODE_HPP_
#define _ML_VIDEO_MODE_HPP_

#include <libmeme/Core/Rect.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD video_mode final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec2i		size	{};
		uint32_t	depth	{};

		constexpr operator bool() const noexcept
		{
			return size[0] && size[1] && depth;
		}

		constexpr bool operator==(video_mode const & other) const noexcept
		{
			return (this == std::addressof(other))
				|| ((size == other.size) && (depth == other.depth));
		}

		constexpr bool operator!=(video_mode const & other) const noexcept
		{
			return !(*this == other);
		}

		constexpr bool operator<(video_mode const & other) const noexcept
		{
			return (this != std::addressof(other))
				&& (size < other.size) || (depth < other.depth);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static void from_json(json const & j, video_mode & value)
	{
		j.at("size").get_to(value.size);
		j.at("depth").get_to(value.depth);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VIDEO_MODE_HPP_