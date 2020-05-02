#ifndef _ML_VIDEO_MODE_HPP_
#define _ML_VIDEO_MODE_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD video_mode final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec2i		size {};
		uint32_t	depth{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr bool nonzero() const noexcept
		{
			return size[0] && size[1] && depth;
		}

		ML_NODISCARD constexpr operator bool() const noexcept
		{
			return this->nonzero();
		}

		ML_NODISCARD constexpr bool operator==(video_mode const & other) const
		{
			return (size == other.size) && (depth == other.depth);
		}

		ML_NODISCARD constexpr bool operator!=(video_mode const & other) const
		{
			return !(*this == other);
		}

		ML_NODISCARD constexpr bool operator<(video_mode const & other) const
		{
			return (size < other.size) || (depth < other.depth);
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