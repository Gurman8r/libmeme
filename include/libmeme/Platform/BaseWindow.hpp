#ifndef _ML_BASE_WINDOW_HPP_
#define _ML_BASE_WINDOW_HPP_

// WIP

#include <libmeme/Core/Memory.hpp>
#include <libmeme/Platform/Cursor.hpp>
#include <libmeme/Platform/Keyboard.hpp>
#include <libmeme/Platform/Mouse.hpp>
#include <libmeme/Platform/WindowSettings.hpp>

namespace ml
{
	struct ML_PLATFORM_API base_window : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~base_window() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual bool open(window_settings const &) = 0;

		virtual bool close() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_BASE_WINDOW_HPP_