#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <libmeme/Platform/WindowAPI.hpp>

namespace ml
{
	struct window_base;

	struct ML_PLATFORM_API window : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window() noexcept;

		explicit window(window_settings const & ws, bool ic = true) noexcept;

		virtual ~window() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual bool open(window_settings const & ws, bool ic = true);
		
		virtual void close();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void iconify();

		void maximize();
		
		void restore();
		
		void swap_buffers();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool is_open() const;

		ML_NODISCARD int32_t get_attribute(int32_t value) const;

		ML_NODISCARD int_rect get_bounds() const;

		ML_NODISCARD cstring get_clipboard_string() const;

		ML_NODISCARD vec2 get_content_scale() const;

		ML_NODISCARD vec2 get_cursor_position() const;

		ML_NODISCARD vec2i get_framebuffer_size() const;

		ML_NODISCARD window_handle get_handle() const;

		ML_NODISCARD int32_t get_input_mode(int32_t mode) const;

		ML_NODISCARD int32_t get_key(int32_t key) const;

		ML_NODISCARD int32_t get_mouse_button(int32_t button) const;

		ML_NODISCARD window_handle get_native_handle() const;

		ML_NODISCARD float_t get_opacity() const;

		ML_NODISCARD vec2i get_position() const;

		ML_NODISCARD void * get_user_pointer() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_clipboard_string(cstring value);
		
		void set_cursor(cursor_handle value);
		
		void set_cursor_mode(int32_t value);
		
		void set_cursor_position(vec2d const & value);
		
		void set_icon(size_t w, size_t h, byte_t const * p);

		void set_input_mode(int32_t mode, int32_t value);
		
		void set_opacity(float_t value);

		void set_position(vec2i const & value);
		
		void set_monitor(monitor_handle value, int_rect const & bounds = {});

		void set_size(vec2i const & value);
		
		void set_title(cstring value);

		void set_user_pointer(void * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static cursor_handle create_custom_cursor(size_t w, size_t h, byte_t const * p);
		
		ML_NODISCARD static cursor_handle create_standard_cursor(int32_t value);

		ML_NODISCARD static int32_t extension_supported(cstring value);

		ML_NODISCARD static window_handle get_current_context();

		ML_NODISCARD static window_proc get_proc_address(cstring value);
		
		ML_NODISCARD static pmr::vector<monitor_handle> const & get_monitors();

		ML_NODISCARD static monitor_handle get_primary_monitor();

		ML_NODISCARD static duration get_time();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void destroy_cursor(cursor_handle value);

		static void poll_events();

		static void set_current_context(window_handle value);

		static void set_swap_interval(int32_t value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window_char_fn				set_char_callback				(window_char_fn				fn);
		window_char_mods_fn			set_char_mods_callback			(window_char_mods_fn		fn);
		window_close_fn				set_close_callback				(window_close_fn			fn);
		window_content_scale_fn		set_content_scale_callback		(window_content_scale_fn	fn);
		window_cursor_enter_fn		set_cursor_enter_callback		(window_cursor_enter_fn		fn);
		window_cursor_position_fn	set_cursor_position_callback	(window_cursor_position_fn	fn);
		window_drop_fn				set_drop_callback				(window_drop_fn				fn);
		window_error_fn				set_error_callback				(window_error_fn			fn);
		window_focus_fn				set_focus_callback				(window_focus_fn			fn);
		window_framebuffer_size_fn	set_framebuffer_size_callback	(window_framebuffer_size_fn	fn);
		window_iconify_fn			set_iconify_callback			(window_iconify_fn			fn);
		window_key_fn				set_key_callback				(window_key_fn				fn);
		window_maximize_fn			set_maximize_callback			(window_maximize_fn			fn);
		window_mouse_fn				set_mouse_callback				(window_mouse_fn			fn);
		window_position_fn			set_position_callback			(window_position_fn			fn);
		window_refresh_fn			set_refresh_callback			(window_refresh_fn			fn);
		window_scroll_fn			set_scroll_callback				(window_scroll_fn			fn);
		window_size_fn				set_size_callback				(window_size_fn				fn);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_context_settings() const & noexcept -> context_settings const & { return m_settings.context; }

		ML_NODISCARD bool get_hint(int32_t const i) const noexcept { return ML_flag_read(m_settings.hints, i); }

		ML_NODISCARD auto get_hints() const & noexcept -> int32_t { return m_settings.hints; }

		ML_NODISCARD auto get_title() const & noexcept -> pmr::string const & { return m_settings.title; }

		ML_NODISCARD auto get_video_mode() const & noexcept -> video_mode const & { return m_settings.video; }

		ML_NODISCARD auto get_window_settings() const & noexcept -> window_settings const & { return m_settings; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		window_settings m_settings;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		unique<window_base> m_wnd;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_WINDOW_HPP_