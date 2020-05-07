#ifdef ML_PLATFORM_GLFW
#ifndef _ML_IMPL_WINDOW_GLFW_HPP_
#define _ML_IMPL_WINDOW_GLFW_HPP_

#include <libmeme/Platform/WindowImpl.hpp>

namespace ml
{
	struct impl_window_glfw final : window_impl
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~impl_window_glfw() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual bool open(window_settings const & ws) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		void destroy() override;

		void iconify() override;
		
		void make_context_current() override;
		
		void maximize() override;
		
		void restore() override;
		
		void swap_buffers() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_clipboard(cstring value) override;
		
		void set_cursor(cursor_handle value) override;
		
		void set_cursor_mode(int32_t value) override;
		
		void set_cursor_position(vec2d const & value) override;
		
		void set_fullscreen(bool value) override;
		
		void set_icon(size_t w, size_t h, byte_t const * p) override;

		void set_input_mode(int32_t mode, int32_t value) override;
		
		void set_position(vec2i const & value) override;
		
		void set_monitor(window_handle value) override;

		void set_should_close(bool value) override;
		
		void set_size(vec2i const & value) override;
		
		void set_title(cstring value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool is_focused() const override;
		
		ML_NODISCARD bool is_fullscreen() const override;
		
		ML_NODISCARD bool is_open() const override;
		
		ML_NODISCARD int32_t get_attribute(int32_t value) const override;
		
		ML_NODISCARD cstring get_clipboard() const override;
		
		ML_NODISCARD vec2 get_cursor_pos() const override;
		
		ML_NODISCARD vec2i get_frame_size() const override;

		ML_NODISCARD window_handle get_handle() const override;
		
		ML_NODISCARD int32_t get_input_mode(int32_t mode) const override;

		ML_NODISCARD int32_t get_key(int32_t key) const override;
		
		ML_NODISCARD int32_t get_mouse_button(int32_t button) const override;

		ML_NODISCARD window_handle get_native_handle() const override;
		
		ML_NODISCARD vec2i get_position() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void destroy_cursor(cursor_handle value);

		static void make_context_current(window_handle value);

		static void poll_events();

		static void swap_interval(int32_t value);

		static void terminate();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static cursor_handle create_custom_cursor(size_t w, size_t h, byte_t const * p);
		
		ML_NODISCARD static cursor_handle create_standard_cursor(int32_t value);

		ML_NODISCARD static int32_t extension_supported(cstring value);

		ML_NODISCARD static window_handle get_context_current();

		ML_NODISCARD static video_mode const & get_desktop_mode();
		
		ML_NODISCARD static ds::flat_set<video_mode> const & get_fullscreen_modes();

		ML_NODISCARD static void * get_proc_address(cstring value);
		
		ML_NODISCARD static pmr::vector<window_handle> const & get_monitors();

		ML_NODISCARD static window_handle get_primary_monitor();

		ML_NODISCARD static float64_t get_time();

		ML_NODISCARD static bool initialize();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window_char_fn			set_char_callback			(window_char_fn			fn) override;
		window_cursor_enter_fn	set_cursor_enter_callback	(window_cursor_enter_fn	fn) override;
		window_cursor_pos_fn	set_cursor_pos_callback		(window_cursor_pos_fn	fn) override;
		window_error_fn			set_error_callback			(window_error_fn		fn) override;
		window_frame_size_fn	set_frame_size_callback		(window_frame_size_fn	fn) override;
		window_key_fn			set_key_callback			(window_key_fn			fn) override;
		window_mouse_fn			set_mouse_callback			(window_mouse_fn		fn) override;
		window_scroll_fn		set_scroll_callback			(window_scroll_fn		fn) override;
		window_close_fn			set_window_close_callback	(window_close_fn		fn) override;
		window_focus_fn			set_window_focus_callback	(window_focus_fn		fn) override;
		window_position_fn		set_window_pos_callback		(window_position_fn		fn) override;
		window_size_fn			set_window_size_callback	(window_size_fn			fn) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		window_handle	m_window	{};
		window_handle	m_monitor	{};
		window_handle	m_share		{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_IMPL_WINDOW_GLFW_HPP_
#endif // ML_PLATFORM_GLFW