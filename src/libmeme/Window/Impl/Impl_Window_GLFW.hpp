#if defined(ML_IMPL_WINDOW_GLFW)
#ifndef _ML_IMPL_WINDOW_GLFW_HPP_
#define _ML_IMPL_WINDOW_GLFW_HPP_

#include <libmeme/Window/WindowBase.hpp>

struct GLFWwindow;
struct GLFWmonitor;

namespace ml
{
	// glfw window implementation
	struct glfw_window final : window_base
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		glfw_window() noexcept;

		explicit glfw_window(window_settings const & ws) noexcept;

		~glfw_window() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool open(window_settings const & ws) override;
		
		void close() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void iconify() override;
		
		void maximize() override;
		
		void restore() override;
		
		void swap_buffers() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool is_open() const override;

		int32_t get_attribute(int32_t value) const override;

		int_rect get_bounds() const override;

		cstring get_clipboard_string() const override;

		vec2 get_content_scale() const override;

		vec2 get_cursor_position() const override;

		vec2i get_framebuffer_size() const override;

		window_handle get_handle() const override;

		int32_t get_input_mode(int32_t mode) const override;

		int32_t get_key(int32_t key) const override;

		int32_t get_mouse_button(int32_t button) const override;

		window_handle get_native_handle() const override;

		float_t get_opacity() const override;

		vec2i get_position() const override;

		vec2i get_size() const override;

		cstring get_title() const override;

		void * get_user_pointer() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_clipboard_string(cstring value) override;
		
		void set_cursor(cursor_handle value) override;
		
		void set_cursor_mode(int32_t value) override;
		
		void set_cursor_position(vec2d const & value) override;
		
		void set_icon(size_t w, size_t h, byte_t const * p) override;

		void set_input_mode(int32_t mode, int32_t value) override;

		void set_opacity(float_t value) override;
		
		void set_position(vec2i const & value) override;
		
		void set_monitor(monitor_handle value, int_rect const & bounds = {}) override;

		void set_size(vec2i const & value) override;
		
		void set_title(cstring value) override;

		void set_user_pointer(void * value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void poll_events();

		static void set_current_context(window_handle value);

		static void set_swap_interval(int32_t value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t extension_supported(cstring value);

		static window_handle get_current_context();

		static void * get_proc_address(cstring value);
		
		static pmr::vector<monitor_handle> const & get_monitors();

		static monitor_handle get_primary_monitor();

		static duration get_time();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static cursor_handle create_custom_cursor(size_t w, size_t h, byte_t const * p);

		static cursor_handle create_standard_cursor(int32_t value);

		static void destroy_cursor(cursor_handle value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window_char_fn				set_char_callback				(window_char_fn				fn) override;
		window_char_mods_fn			set_char_mods_callback			(window_char_mods_fn		fn) override;
		window_close_fn				set_close_callback				(window_close_fn			fn) override;
		window_content_scale_fn		set_content_scale_callback		(window_content_scale_fn	fn) override;
		window_cursor_enter_fn		set_cursor_enter_callback		(window_cursor_enter_fn		fn) override;
		window_cursor_position_fn	set_cursor_position_callback	(window_cursor_position_fn	fn) override;
		window_drop_fn				set_drop_callback				(window_drop_fn				fn) override;
		window_error_fn				set_error_callback				(window_error_fn			fn) override;
		window_focus_fn				set_focus_callback				(window_focus_fn			fn) override;
		window_framebuffer_size_fn	set_framebuffer_size_callback	(window_framebuffer_size_fn	fn) override;
		window_iconify_fn			set_iconify_callback			(window_iconify_fn			fn) override;
		window_key_fn				set_key_callback				(window_key_fn				fn) override;
		window_maximize_fn			set_maximize_callback			(window_maximize_fn			fn) override;
		window_mouse_fn				set_mouse_callback				(window_mouse_fn			fn) override;
		window_position_fn			set_position_callback			(window_position_fn			fn) override;
		window_refresh_fn			set_refresh_callback			(window_refresh_fn			fn) override;
		window_scroll_fn			set_scroll_callback				(window_scroll_fn			fn) override;
		window_size_fn				set_size_callback				(window_size_fn				fn) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		pmr::string		m_title		;
		GLFWwindow	*	m_window	;
		GLFWmonitor	*	m_monitor	;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_IMPL_WINDOW_GLFW_HPP_
#endif // ML_IMPL_WINDOW_GLFW