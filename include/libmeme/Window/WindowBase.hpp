#ifndef _ML_WINDOW_BASE_HPP_
#define _ML_WINDOW_BASE_HPP_

#include <libmeme/Window/WindowAPI.hpp>

namespace ml
{
	// window implementation base
	struct window_base : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~window_base() = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool open(window_settings const &) = 0;
		
		virtual void close() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void iconify() = 0;

		virtual void maximize() = 0;

		virtual void restore() = 0;

		virtual void swap_buffers() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual int_rect get_bounds() const = 0;

		virtual cstring get_clipboard() const = 0;

		virtual vec2 get_content_scale() const = 0;

		virtual vec2 get_cursor_position() const = 0;

		virtual vec2i get_framebuffer_size() const = 0;

		virtual window_handle get_handle() const = 0;

		virtual int32_t get_input_mode(int32_t) const = 0;

		virtual int32_t get_key(int32_t) const = 0;

		virtual int32_t get_mouse_button(int32_t) const = 0;

		virtual window_handle get_native_handle() const = 0;

		virtual float_t get_opacity() const = 0;

		virtual vec2i get_position() const = 0;

		virtual vec2i get_size() const = 0;

		virtual cstring get_title() const = 0;

		virtual void * get_user_pointer() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool is_auto_iconify() const = 0;

		virtual bool is_decorated() const = 0;

		virtual bool is_center_cursor() const = 0;

		virtual bool is_floating() const = 0;

		virtual bool is_focused() const = 0;

		virtual bool is_focus_on_show() const = 0;

		virtual bool is_hovered() const = 0;

		virtual bool is_iconified() const = 0;

		virtual bool is_maximized() const = 0;

		virtual bool is_open() const = 0;

		virtual bool is_resizable() const = 0;

		virtual bool is_transparent_framebuffer() const = 0;

		virtual bool is_visible() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_auto_iconify(bool) = 0;

		virtual void set_clipboard(cstring) = 0;

		virtual void set_cursor(cursor_handle) = 0;

		virtual void set_cursor_mode(int32_t) = 0;

		virtual void set_cursor_position(vec2d const &) = 0;

		virtual void set_decorated(bool) = 0;

		virtual void set_floating(bool) = 0;

		virtual void set_focus_on_show(bool) = 0;

		virtual void set_icon(size_t, size_t, byte_t const *) = 0;

		virtual void set_input_mode(int32_t, int32_t) = 0;

		virtual void set_opacity(float_t) = 0;

		virtual void set_position(vec2i const &) = 0;

		virtual void set_monitor(monitor_handle, int_rect const &) = 0;

		virtual void set_resizable(bool) = 0;

		virtual void set_size(vec2i const &) = 0;

		virtual void set_title(cstring) = 0;

		virtual void set_user_pointer(void *) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static cursor_handle create_custom_cursor(size_t, size_t, byte_t const *) = delete;

		static cursor_handle create_standard_cursor(int32_t) = delete;

		static int32_t extension_supported(cstring) = delete;

		static window_handle get_current_context() = delete;

		static void * get_proc_address(cstring) = delete;
		
		static pmr::vector<monitor_handle> const & get_monitors() = delete;

		static monitor_handle get_primary_monitor() = delete;

		static duration get_time() = delete;

		static void destroy_cursor(cursor_handle) = delete;

		static void poll_events() = delete;

		static void set_current_context(window_handle) = delete;

		static void set_swap_interval(int32_t) = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual window_char_fn				set_char_callback				(window_char_fn) = 0;
		virtual window_char_mods_fn			set_char_mods_callback			(window_char_mods_fn) = 0;
		virtual window_close_fn				set_close_callback				(window_close_fn) = 0;
		virtual window_content_scale_fn		set_content_scale_callback		(window_content_scale_fn) = 0;
		virtual window_cursor_enter_fn		set_cursor_enter_callback		(window_cursor_enter_fn) = 0;
		virtual window_cursor_position_fn	set_cursor_position_callback	(window_cursor_position_fn) = 0;
		virtual window_drop_fn				set_drop_callback				(window_drop_fn) = 0;
		virtual window_error_fn				set_error_callback				(window_error_fn) = 0;
		virtual window_focus_fn				set_focus_callback				(window_focus_fn) = 0;
		virtual window_framebuffer_size_fn	set_framebuffer_size_callback	(window_framebuffer_size_fn) = 0;
		virtual window_iconify_fn			set_iconify_callback			(window_iconify_fn) = 0;
		virtual window_key_fn				set_key_callback				(window_key_fn) = 0;
		virtual window_maximize_fn			set_maximize_callback			(window_maximize_fn) = 0;
		virtual window_mouse_fn				set_mouse_callback				(window_mouse_fn) = 0;
		virtual window_position_fn			set_position_callback			(window_position_fn) = 0;
		virtual window_refresh_fn			set_refresh_callback			(window_refresh_fn) = 0;
		virtual window_scroll_fn			set_scroll_callback				(window_scroll_fn) = 0;
		virtual window_size_fn				set_size_callback				(window_size_fn) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_WINDOW_BASE_HPP_