#ifndef _ML_BASE_WINDOW_HPP_
#define _ML_BASE_WINDOW_HPP_

#include <libmeme/Core/Memory.hpp>
#include <libmeme/Platform/Cursor.hpp>
#include <libmeme/Platform/Keyboard.hpp>
#include <libmeme/Platform/Mouse.hpp>
#include <libmeme/Platform/WindowSettings.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias window_handle = typename void *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_PLATFORM_API base_window : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~base_window() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual bool open(window_settings const &) = 0;

		virtual bool close() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		window_handle	m_window	{}; // 
		window_handle	m_monitor	{}; // 
		window_handle	m_share		{}; // 
		window_settings	m_settings	{}; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_BASE_WINDOW_HPP_