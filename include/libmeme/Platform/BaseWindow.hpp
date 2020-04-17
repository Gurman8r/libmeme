#ifndef _ML_BASE_WINDOW_HPP_
#define _ML_BASE_WINDOW_HPP_

// WIP

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	struct ML_PLATFORM_API base_window : non_copyable, trackable
	{
		virtual ~base_window() noexcept = default;
	};
}

#endif // !_ML_BASE_WINDOW_HPP_