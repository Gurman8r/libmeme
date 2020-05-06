#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

// WIP

#include <libmeme/Platform/BaseWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_PLATFORM_API window : base_window
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~window() noexcept = default;

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

		void set_centered() override;
		
		void set_clipboard(cstring const & value) override;
		
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
		
		void set_title(pmr::string const & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool is_focused() const override;
		
		ML_NODISCARD bool is_fullscreen() const override;
		
		ML_NODISCARD bool is_open() const override;
		
		ML_NODISCARD int32_t get_attribute(int32_t value) const override;
		
		ML_NODISCARD cstring get_clipboard() const override;
		
		ML_NODISCARD vec2 get_cursor_pos() const override;
		
		ML_NODISCARD vec2i get_frame_size() const override;
		
		ML_NODISCARD window_handle get_handle() const override;

		ML_NODISCARD int32_t get_key(int32_t key) const override;
		
		ML_NODISCARD int32_t get_input_mode(int32_t mode) const override;
		
		ML_NODISCARD int32_t get_mouse_button(int32_t button) const override;

		ML_NODISCARD window_handle get_native_handle() const override;
		
		ML_NODISCARD vec2i get_position() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void destroy_cursor(cursor_handle value) const override;

		void make_context_current(window_handle value) const override;

		void poll_events() const override;

		void swap_interval(int32_t value) const override;

		void terminate() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD cursor_handle create_custom_cursor(size_t w, size_t h, byte_t const * p) const override;
		
		ML_NODISCARD cursor_handle create_standard_cursor(int32_t value) const override;

		ML_NODISCARD int32_t extension_supported(cstring value) const override;

		ML_NODISCARD window_handle get_context_current() const override;

		ML_NODISCARD video_mode const & get_desktop_mode() const override;
		
		ML_NODISCARD ds::flat_set<video_mode> const & get_fullscreen_modes() const override;

		ML_NODISCARD void * get_proc_address(cstring value) const override;
		
		ML_NODISCARD pmr::vector<window_handle> const & get_monitors() const override;

		ML_NODISCARD float64_t get_time() const override;

		ML_NODISCARD bool initialize() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		char_fn			set_char_callback			(char_fn			fn) override;
		cursor_enter_fn	set_cursor_enter_callback	(cursor_enter_fn	fn) override;
		cursor_pos_fn	set_cursor_pos_callback		(cursor_pos_fn		fn) override;
		error_fn		set_error_callback			(error_fn			fn) override;
		frame_size_fn	set_frame_size_callback		(frame_size_fn		fn) override;
		key_fn			set_key_callback			(key_fn				fn) override;
		mouse_fn		set_mouse_callback			(mouse_fn			fn) override;
		scroll_fn		set_scroll_callback			(scroll_fn			fn) override;
		close_fn		set_window_close_callback	(close_fn			fn) override;
		focus_fn		set_window_focus_callback	(focus_fn			fn) override;
		position_fn		set_window_pos_callback		(position_fn		fn) override;
		size_fn			set_window_size_callback	(size_fn			fn) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		base_window * m_impl{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_HPP_