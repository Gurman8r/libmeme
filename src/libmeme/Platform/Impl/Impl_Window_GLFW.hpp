#if defined(ML_WINDOW_GLFW)
#ifndef _ML_IMPL_WINDOW_GLFW_HPP_
#define _ML_IMPL_WINDOW_GLFW_HPP_

#include <libmeme/Platform/WindowImpl.hpp>

struct GLFWwindow;
struct GLFWmonitor;

namespace ml
{
	// glfw window implementation
	struct impl_window_glfw final : window_impl
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		impl_window_glfw() noexcept = default;

		~impl_window_glfw() noexcept { this->destroy(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool open(window_settings const & ws) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		void destroy() override;

		void iconify() override;
		
		void maximize() override;
		
		void restore() override;
		
		void swap_buffers() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool is_fullscreen() const override;

		ML_NODISCARD bool is_open() const override;

		ML_NODISCARD int32_t get_attribute(int32_t value) const override;

		ML_NODISCARD int_rect get_bounds() const override;

		ML_NODISCARD cstring get_clipboard_string() const override;

		ML_NODISCARD vec2 get_content_scale() const override;

		ML_NODISCARD vec2 get_cursor_position() const override;

		ML_NODISCARD vec2i get_framebuffer_size() const override;

		ML_NODISCARD window_handle get_handle() const override;

		ML_NODISCARD int32_t get_input_mode(int32_t mode) const override;

		ML_NODISCARD int32_t get_key(int32_t key) const override;

		ML_NODISCARD int32_t get_mouse_button(int32_t button) const override;

		ML_NODISCARD window_handle get_native_handle() const override;

		ML_NODISCARD float_t get_opacity() const override;

		ML_NODISCARD vec2i get_position() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_clipboard_string(cstring value) override;
		
		void set_cursor(cursor_handle value) override;
		
		void set_cursor_mode(int32_t value) override;
		
		void set_cursor_position(vec2d const & value) override;
		
		void set_fullscreen(bool value) override;
		
		void set_icon(size_t w, size_t h, byte_t const * p) override;

		void set_input_mode(int32_t mode, int32_t value) override;

		void set_opacity(float_t value) override;
		
		void set_position(vec2i const & value) override;
		
		void set_monitor(monitor_handle value, int_rect const & bounds = {}) override;

		void set_should_close(bool value) override;
		
		void set_size(vec2i const & value) override;
		
		void set_title(cstring value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static cursor_handle create_custom_cursor(size_t w, size_t h, byte_t const * p);
		
		ML_NODISCARD static cursor_handle create_standard_cursor(int32_t value);

		ML_NODISCARD static int32_t extension_supported(cstring value);

		ML_NODISCARD static window_handle get_current_context();

		ML_NODISCARD static video_mode const & get_desktop_mode();
		
		ML_NODISCARD static pmr::vector<video_mode> const & get_fullscreen_modes();

		ML_NODISCARD static void * get_proc_address(cstring value);
		
		ML_NODISCARD static pmr::vector<monitor_handle> const & get_monitors();

		ML_NODISCARD static monitor_handle get_primary_monitor();

		ML_NODISCARD static float64_t get_time();

		ML_NODISCARD static bool initialize();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void destroy_cursor(cursor_handle value);

		static void finalize();

		static void poll_events();

		static void set_current_context(window_handle value);

		static void set_swap_interval(int32_t value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window_error_fn				set_error_callback				(window_error_fn			fn) override;
		window_char_fn				set_char_callback				(window_char_fn				fn) override;
		window_cursor_enter_fn		set_cursor_enter_callback		(window_cursor_enter_fn		fn) override;
		window_cursor_position_fn	set_cursor_position_callback	(window_cursor_position_fn	fn) override;
		window_framebuffer_size_fn	set_framebuffer_size_callback	(window_framebuffer_size_fn	fn) override;
		window_key_fn				set_key_callback				(window_key_fn				fn) override;
		window_mouse_fn				set_mouse_callback				(window_mouse_fn			fn) override;
		window_scroll_fn			set_scroll_callback				(window_scroll_fn			fn) override;
		window_close_fn				set_window_close_callback		(window_close_fn			fn) override;
		window_focus_fn				set_window_focus_callback		(window_focus_fn			fn) override;
		window_position_fn			set_window_position_callback	(window_position_fn			fn) override;
		window_size_fn				set_window_size_callback		(window_size_fn				fn) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		GLFWwindow	* m_window	{};
		GLFWmonitor	* m_monitor	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_IMPL_WINDOW_GLFW_HPP_
#endif // ML_WINDOW_GLFW