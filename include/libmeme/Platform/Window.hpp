#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <libmeme/Platform/WindowAPI.hpp>

namespace ml
{
	struct window_base;

	struct ML_PLATFORM_API window : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window() noexcept;

		explicit window(window_settings const & ws) noexcept;

		virtual ~window() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool open(window_settings const & ws);
		
		virtual void close();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void iconify() noexcept;

		void maximize() noexcept;
		
		void restore() noexcept;
		
		void swap_buffers() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool is_open() const noexcept;

		ML_NODISCARD int32_t get_attribute(int32_t value) const noexcept;

		ML_NODISCARD int_rect get_bounds() const noexcept;

		ML_NODISCARD cstring get_clipboard_string() const noexcept;

		ML_NODISCARD vec2 get_content_scale() const noexcept;

		ML_NODISCARD vec2 get_cursor_position() const noexcept;

		ML_NODISCARD vec2i get_framebuffer_size() const noexcept;

		ML_NODISCARD window_handle get_handle() const noexcept;

		ML_NODISCARD int32_t get_input_mode(int32_t mode) const noexcept;

		ML_NODISCARD int32_t get_key(int32_t key) const noexcept;

		ML_NODISCARD int32_t get_mouse_button(int32_t button) const noexcept;

		ML_NODISCARD window_handle get_native_handle() const noexcept;

		ML_NODISCARD float_t get_opacity() const noexcept;

		ML_NODISCARD vec2i get_position() const noexcept;

		ML_NODISCARD vec2i get_size() const noexcept;

		ML_NODISCARD cstring get_title() const noexcept;

		ML_NODISCARD void * get_user_pointer() const noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_clipboard_string(cstring value) noexcept;
		
		void set_cursor(cursor_handle value) noexcept;
		
		void set_cursor_mode(int32_t value) noexcept;
		
		void set_cursor_position(vec2d const & value) noexcept;
		
		void set_icon(size_t w, size_t h, byte_t const * p) noexcept;

		void set_input_mode(int32_t mode, int32_t value) noexcept;
		
		void set_opacity(float_t value) noexcept;

		void set_position(vec2i const & value) noexcept;
		
		void set_monitor(monitor_handle value, int_rect const & bounds = {}) noexcept;

		void set_size(vec2i const & value) noexcept;
		
		void set_title(cstring value) noexcept;

		void set_user_pointer(void * value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static cursor_handle create_custom_cursor(size_t w, size_t h, byte_t const * p) noexcept;
		
		ML_NODISCARD static cursor_handle create_standard_cursor(int32_t value) noexcept;

		ML_NODISCARD static int32_t extension_supported(cstring value) noexcept;

		ML_NODISCARD static window_handle get_current_context() noexcept;

		ML_NODISCARD static void * get_proc_address(cstring value) noexcept;
		
		ML_NODISCARD static pmr::vector<monitor_handle> const & get_monitors() noexcept;

		ML_NODISCARD static monitor_handle get_primary_monitor() noexcept;

		ML_NODISCARD static duration get_time() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void destroy_cursor(cursor_handle value) noexcept;

		static void poll_events() noexcept;

		static void set_current_context(window_handle value) noexcept;

		static void set_swap_interval(int32_t value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window_char_fn				set_char_callback				(window_char_fn				fn) noexcept;
		window_char_mods_fn			set_char_mods_callback			(window_char_mods_fn		fn) noexcept;
		window_close_fn				set_close_callback				(window_close_fn			fn) noexcept;
		window_content_scale_fn		set_content_scale_callback		(window_content_scale_fn	fn) noexcept;
		window_cursor_enter_fn		set_cursor_enter_callback		(window_cursor_enter_fn		fn) noexcept;
		window_cursor_position_fn	set_cursor_position_callback	(window_cursor_position_fn	fn) noexcept;
		window_drop_fn				set_drop_callback				(window_drop_fn				fn) noexcept;
		window_error_fn				set_error_callback				(window_error_fn			fn) noexcept;
		window_focus_fn				set_focus_callback				(window_focus_fn			fn) noexcept;
		window_framebuffer_size_fn	set_framebuffer_size_callback	(window_framebuffer_size_fn	fn) noexcept;
		window_iconify_fn			set_iconify_callback			(window_iconify_fn			fn) noexcept;
		window_key_fn				set_key_callback				(window_key_fn				fn) noexcept;
		window_maximize_fn			set_maximize_callback			(window_maximize_fn			fn) noexcept;
		window_mouse_fn				set_mouse_callback				(window_mouse_fn			fn) noexcept;
		window_position_fn			set_position_callback			(window_position_fn			fn) noexcept;
		window_refresh_fn			set_refresh_callback			(window_refresh_fn			fn) noexcept;
		window_scroll_fn			set_scroll_callback				(window_scroll_fn			fn) noexcept;
		window_size_fn				set_size_callback				(window_size_fn				fn) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool has_hint(int32_t value) const noexcept { return m_ws.hints & value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		window_settings m_ws;

	private:
		unique<window_base> m_window;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_WINDOW_HPP_