#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <libmeme/Window/WindowBase.hpp>

namespace ml
{
	struct ML_WINDOW_API window : window_base
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window() noexcept;

		explicit window(window_settings const & ws) noexcept;

		virtual ~window() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool open(window_settings const & ws) override;
		
		virtual void close() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void iconify() noexcept final;

		void maximize() noexcept final;

		void restore() noexcept final;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD int_rect get_bounds() const noexcept final;

		ML_NODISCARD cstring get_clipboard() const noexcept final;

		ML_NODISCARD vec2 get_content_scale() const noexcept final;

		ML_NODISCARD vec2 get_cursor_position() const noexcept final;

		ML_NODISCARD int32_t get_cursor_mode() const noexcept final;

		ML_NODISCARD vec2i get_framebuffer_size() const noexcept final;

		ML_NODISCARD window_handle get_handle() const noexcept final;

		ML_NODISCARD int32_t get_hints() const noexcept final;

		ML_NODISCARD int32_t get_input_mode(int32_t value) const noexcept final;

		ML_NODISCARD int32_t get_key(int32_t value) const noexcept final;

		ML_NODISCARD int32_t get_mouse_button(int32_t value) const noexcept final;

		ML_NODISCARD window_handle get_native_handle() const noexcept final;

		ML_NODISCARD float_t get_opacity() const noexcept final;

		ML_NODISCARD vec2i get_position() const noexcept final;

		ML_NODISCARD vec2i get_size() const noexcept final;

		ML_NODISCARD void * get_user_pointer() const noexcept final;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool is_auto_iconify() const noexcept final;

		ML_NODISCARD bool is_decorated() const noexcept final;

		ML_NODISCARD bool is_floating() const noexcept final;

		ML_NODISCARD bool is_focus_on_show() const noexcept final;

		ML_NODISCARD bool is_focused() const noexcept final;

		ML_NODISCARD bool is_hovered() const noexcept final;

		ML_NODISCARD bool is_iconified() const noexcept final;

		ML_NODISCARD bool is_maximized() const noexcept final;

		ML_NODISCARD bool is_open() const noexcept final;

		ML_NODISCARD bool is_resizable() const noexcept final;

		ML_NODISCARD bool is_transparent() const noexcept final;

		ML_NODISCARD bool is_visible() const noexcept final;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_auto_iconify(bool value) noexcept final;

		void set_clipboard(cstring value) noexcept final;
		
		void set_cursor(cursor_handle value) noexcept final;
		
		void set_cursor_mode(int32_t value) noexcept final;
		
		void set_cursor_position(vec2d const & value) noexcept final;

		void set_decorated(bool value) noexcept final;

		void set_floating(bool value) noexcept final;

		void set_focus_on_show(bool value) noexcept final;
		
		void set_icon(size_t w, size_t h, byte_t const * p) noexcept final;

		void set_input_mode(int32_t mode, int32_t value) noexcept final;
		
		void set_opacity(float_t value) noexcept final;

		void set_position(vec2i const & value) noexcept final;
		
		void set_monitor(monitor_handle value, int_rect const & bounds = {}) noexcept final;

		void set_resizable(bool value) noexcept final;

		void set_size(vec2i const & value) noexcept final;
		
		void set_title(cstring value) noexcept final;

		void set_user_pointer(void * value) noexcept final;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static int32_t extension_supported(cstring value) noexcept;

		ML_NODISCARD static window_handle get_context_current() noexcept;

		ML_NODISCARD static void * get_proc_address(cstring value) noexcept;
		
		ML_NODISCARD static pmr::vector<monitor_handle> const & get_monitors() noexcept;

		ML_NODISCARD static monitor_handle get_primary_monitor() noexcept;

		ML_NODISCARD static duration get_time() noexcept;

		static void make_context_current(window_handle value) noexcept;

		static void poll_events() noexcept;

		static void swap_buffers(window_handle value) noexcept;

		static void swap_buffers(window const & value) noexcept
		{
			if (value.get_hints() & window_hints_doublebuffer)
			{
				swap_buffers(value.get_handle());
			}
		}

		static void swap_interval(int32_t value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static cursor_handle create_custom_cursor(size_t w, size_t h, byte_t const * p) noexcept;

		ML_NODISCARD static cursor_handle create_standard_cursor(int32_t value) noexcept;

		static void destroy_cursor(cursor_handle value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window_char_callback				set_char_callback				(window_char_callback				fn) noexcept;
		window_char_mods_callback			set_char_mods_callback			(window_char_mods_callback			fn) noexcept;
		window_close_callback				set_close_callback				(window_close_callback				fn) noexcept;
		window_content_scale_callback		set_content_scale_callback		(window_content_scale_callback		fn) noexcept;
		window_cursor_enter_callback		set_cursor_enter_callback		(window_cursor_enter_callback		fn) noexcept;
		window_cursor_pos_callback			set_cursor_position_callback	(window_cursor_pos_callback			fn) noexcept;
		window_drop_callback				set_drop_callback				(window_drop_callback				fn) noexcept;
		window_error_callback				set_error_callback				(window_error_callback				fn) noexcept;
		window_focus_callback				set_focus_callback				(window_focus_callback				fn) noexcept;
		window_framebuffer_size_callback	set_framebuffer_size_callback	(window_framebuffer_size_callback	fn) noexcept;
		window_iconify_callback				set_iconify_callback			(window_iconify_callback			fn) noexcept;
		window_key_callback					set_key_callback				(window_key_callback				fn) noexcept;
		window_maximize_callback			set_maximize_callback			(window_maximize_callback			fn) noexcept;
		window_mouse_callback				set_mouse_callback				(window_mouse_callback				fn) noexcept;
		window_pos_callback					set_position_callback			(window_pos_callback				fn) noexcept;
		window_refresh_callback				set_refresh_callback			(window_refresh_callback			fn) noexcept;
		window_scroll_callback				set_scroll_callback				(window_scroll_callback				fn) noexcept;
		window_size_callback				set_size_callback				(window_size_callback				fn) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		scoped<	window_base	> m_impl;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_WINDOW_HPP_