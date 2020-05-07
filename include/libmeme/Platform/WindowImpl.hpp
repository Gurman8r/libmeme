#ifndef _ML_WINDOW_IMPL_HPP_
#define _ML_WINDOW_IMPL_HPP_

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Platform/Input.hpp>
#include <libmeme/Platform/WindowSettings.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct window_impl : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~window_impl() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool open(window_settings const & ws) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		virtual void destroy() = 0;

		virtual void iconify() = 0;

		virtual void make_context_current() = 0;

		virtual void maximize() = 0;

		virtual void restore() = 0;

		virtual void swap_buffers() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_clipboard(cstring) = 0;

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

		virtual void set_title(cstring) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool is_focused() const = 0;

		virtual bool is_fullscreen() const = 0;

		virtual bool is_open() const = 0;

		virtual int32_t get_attribute(int32_t) const = 0;

		virtual cstring get_clipboard() const = 0;

		virtual vec2 get_cursor_pos() const = 0;

		virtual vec2i get_frame_size() const = 0;

		virtual window_handle get_handle() const = 0;

		virtual int32_t get_input_mode(int32_t) const = 0;

		virtual int32_t get_key(int32_t) const = 0;

		virtual int32_t get_mouse_button(int32_t) const = 0;

		virtual window_handle get_native_handle() const = 0;

		virtual vec2i get_position() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual window_char_fn			set_char_callback(window_char_fn) = 0;
		virtual window_cursor_enter_fn	set_cursor_enter_callback(window_cursor_enter_fn) = 0;
		virtual window_cursor_pos_fn	set_cursor_pos_callback(window_cursor_pos_fn) = 0;
		virtual window_error_fn			set_error_callback(window_error_fn) = 0;
		virtual window_frame_size_fn	set_frame_size_callback(window_frame_size_fn) = 0;
		virtual window_key_fn			set_key_callback(window_key_fn) = 0;
		virtual window_mouse_fn			set_mouse_callback(window_mouse_fn) = 0;
		virtual window_scroll_fn		set_scroll_callback(window_scroll_fn) = 0;
		virtual window_close_fn			set_window_close_callback(window_close_fn) = 0;
		virtual window_focus_fn			set_window_focus_callback(window_focus_fn) = 0;
		virtual window_position_fn		set_window_pos_callback(window_position_fn) = 0;
		virtual window_size_fn			set_window_size_callback(window_size_fn) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_WINDOW_IMPL_HPP_