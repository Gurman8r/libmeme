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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		virtual void destroy() = 0;

		virtual void iconify() = 0;

		virtual void make_context_current() = 0;

		virtual void maximize() = 0;

		virtual void restore() = 0;

		virtual void swap_buffers() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_centered() = 0;

		virtual void set_clipboard(cstring const &) = 0;

		virtual void set_cursor(cursor_handle) = 0;

		virtual void set_cursor_mode(int32_t) = 0;

		virtual void set_cursor_position(vec2d const &) = 0;

		virtual void set_fullscreen(bool) = 0;

		virtual void set_icon(size_t, size_t, byte_t const *) = 0;

		virtual void set_input_mode(int32_t, int32_t) = 0;

		virtual void set_position(vec2i const &) = 0;

		virtual void set_monitor(window_handle) = 0;

		virtual void set_should_close(bool) = 0;

		virtual void set_size(vec2i const &) = 0;

		virtual void set_title(pmr::string const &) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool is_focused() const = 0;

		virtual bool is_fullscreen() const = 0;

		virtual bool is_open() const = 0;

		virtual int32_t get_attribute(int32_t) const = 0;

		virtual cstring get_clipboard() const = 0;

		virtual vec2 get_cursor_pos() const = 0;

		virtual vec2i get_frame_size() const = 0;

		virtual window_handle get_handle() const = 0;

		virtual int32_t get_key(int32_t) const = 0;

		virtual int32_t get_input_mode(int32_t) const = 0;

		virtual int32_t get_mouse_button(int32_t) const = 0;

		virtual window_handle get_native_handle() const = 0;

		virtual vec2i get_position() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void destroy_cursor(cursor_handle) const = 0;

		virtual void make_context_current(window_handle) const = 0;

		virtual void poll_events() const = 0;

		virtual void swap_interval(int32_t) const = 0;

		virtual void terminate() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual cursor_handle create_custom_cursor(size_t, size_t, byte_t const *) const = 0;

		virtual cursor_handle create_standard_cursor(int32_t) const = 0;

		virtual int32_t extension_supported(cstring) const = 0;

		virtual window_handle get_context_current() const = 0;

		virtual video_mode const & get_desktop_mode() const = 0;

		virtual pmr::vector<video_mode> const & get_fullscreen_modes() const = 0;

		virtual proc_fn get_proc_address(cstring) const = 0;

		virtual pmr::vector<window_handle> const & get_monitors() const = 0;

		virtual float64_t get_time() const = 0;

		virtual bool initialize() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual char_fn			set_char_callback			(char_fn		) = 0;
		virtual cursor_enter_fn	set_cursor_enter_callback	(cursor_enter_fn) = 0;
		virtual cursor_pos_fn	set_cursor_pos_callback		(cursor_pos_fn	) = 0;
		virtual error_fn		set_error_callback			(error_fn		) = 0;
		virtual frame_size_fn	set_frame_size_callback		(frame_size_fn	) = 0;
		virtual key_fn			set_key_callback			(key_fn			) = 0;
		virtual mouse_fn		set_mouse_callback			(mouse_fn		) = 0;
		virtual scroll_fn		set_scroll_callback			(scroll_fn		) = 0;
		virtual close_fn		set_window_close_callback	(close_fn		) = 0;
		virtual focus_fn		set_window_focus_callback	(focus_fn		) = 0;
		virtual position_fn		set_window_pos_callback		(position_fn	) = 0;
		virtual size_fn			set_window_size_callback	(size_fn		) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void install_default_callbacks();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_context_setting() const noexcept -> context_settings const & { return m_settings.context; }

		ML_NODISCARD bool get_hint(int32_t const i) const noexcept { return ML_flag_read(m_settings.hints, i); }

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