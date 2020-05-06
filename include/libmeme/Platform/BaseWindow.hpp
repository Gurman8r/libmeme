#ifndef _ML_BASE_WINDOW_HPP_
#define _ML_BASE_WINDOW_HPP_

// WIP

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Platform/Input.hpp>
#include <libmeme/Platform/WindowSettings.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias cursor_handle = typename void *;
	ML_alias window_handle = typename void *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_PLATFORM_API base_window : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using char_fn			= typename void(*)(window_handle, uint32_t);
		using close_fn			= typename void(*)(window_handle);
		using cursor_enter_fn	= typename void(*)(window_handle, int32_t);
		using cursor_pos_fn		= typename void(*)(window_handle, float64_t, float64_t);
		using error_fn			= typename void(*)(int32_t, cstring);
		using focus_fn			= typename void(*)(window_handle, int32_t);
		using frame_size_fn		= typename void(*)(window_handle, int32_t, int32_t);
		using key_fn			= typename void(*)(window_handle, int32_t, int32_t, int32_t, int32_t);
		using mouse_fn			= typename void(*)(window_handle, int32_t, int32_t, int32_t);
		using position_fn		= typename void(*)(window_handle, int32_t, int32_t);
		using proc_fn			= typename void *(*)(void);
		using scroll_fn			= typename void(*)(window_handle, float64_t, float64_t);
		using size_fn			= typename void(*)(window_handle, int32_t, int32_t);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~base_window() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual bool open(window_settings const &) = 0;

		virtual bool close() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_context_setting() const noexcept -> context_settings const & { return m_settings.context; }

		ML_NODISCARD bool get_hint(int32_t const i) const noexcept { return m_settings.hints & i; }

		ML_NODISCARD auto get_hints() const noexcept -> int32_t const & { return m_settings.hints; }

		ML_NODISCARD auto get_monitor() const noexcept -> window_handle const & { return m_monitor; }

		ML_NODISCARD auto get_settings() const noexcept -> window_settings const & { return m_settings; }

		ML_NODISCARD auto get_share() const noexcept -> window_handle const & { return m_share; }

		ML_NODISCARD auto get_title() const noexcept -> pmr::string const & { return m_settings.title; }

		ML_NODISCARD auto get_video_mode() const noexcept -> video_mode const & { return m_settings.video; }

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