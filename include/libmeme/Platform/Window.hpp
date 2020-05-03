#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <libmeme/Platform/BaseWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_PLATFORM_API window : base_window
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using char_fn			= typename void(*)(void *, uint32_t);
		using close_fn			= typename void(*)(void *);
		using cursor_enter_fn	= typename void(*)(void *, int32_t);
		using cursor_pos_fn		= typename void(*)(void *, float64_t, float64_t);
		using error_fn			= typename void(*)(int32_t, cstring);
		using focus_fn			= typename void(*)(void *, int32_t);
		using frame_size_fn		= typename void(*)(void *, int32_t, int32_t);
		using key_fn			= typename void(*)(void *, int32_t, int32_t, int32_t, int32_t);
		using mouse_fn			= typename void(*)(void *, int32_t, int32_t, int32_t);
		using position_fn		= typename void(*)(void *, int32_t, int32_t);
		using proc_fn			= typename void *(*)(void);
		using scroll_fn			= typename void(*)(void *, float64_t, float64_t);
		using size_fn			= typename void(*)(void *, int32_t, int32_t);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~window() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual bool open(window_settings const & ws) override;

		virtual bool close() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		void destroy();

		void iconify();
		
		void make_context_current();
		
		void maximize();
		
		void restore();
		
		void swap_buffers();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_centered();
		
		void set_clipboard(cstring const & value);
		
		void set_cursor(cursor_handle value);
		
		void set_cursor_mode(int32_t value);
		
		void set_cursor_pos(vec2d const & value);
		
		void set_fullscreen(bool value);
		
		void set_icon(size_t w, size_t h, byte_t const * pixels);

		void set_input_mode(int32_t mode, int32_t value);
		
		void set_position(vec2i const & value);
		
		void set_monitor(window_handle value);

		void set_should_close(bool value);
		
		void set_size(vec2i const & value);
		
		void set_title(pmr::string const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool is_focused() const;
		
		ML_NODISCARD bool is_fullscreen() const;
		
		ML_NODISCARD bool is_open() const;
		
		ML_NODISCARD int32_t get_attribute(int32_t value) const;
		
		ML_NODISCARD cstring get_clipboard() const;
		
		ML_NODISCARD vec2 get_cursor_pos() const;
		
		ML_NODISCARD vec2i get_frame_size() const;
		
		ML_NODISCARD window_handle get_handle() const;

		ML_NODISCARD int32_t get_key(int32_t key) const;
		
		ML_NODISCARD int32_t get_input_mode(int32_t mode) const;
		
		ML_NODISCARD int32_t get_mouse_button(int32_t button) const;

		ML_NODISCARD window_handle get_native_handle() const;
		
		ML_NODISCARD vec2i get_position() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void	destroy_cursor(cursor_handle value);

		static void make_context_current(cursor_handle value);

		static void poll_events();

		static void swap_interval(int32_t value);

		static void terminate();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static cursor_handle create_custom_cursor(uint32_t w, uint32_t h, byte_t const * pixels);
		
		ML_NODISCARD static cursor_handle create_standard_cursor(int32_t value);

		ML_NODISCARD static int32_t extension_supported(cstring value);

		ML_NODISCARD static void * get_context_current();

		ML_NODISCARD static video_mode const & get_desktop_mode();
		
		ML_NODISCARD static pmr::vector<video_mode> const & get_fullscreen_modes();

		ML_NODISCARD static proc_fn get_proc_address(cstring value);
		
		ML_NODISCARD static pmr::vector<window_handle> const & get_monitors();

		ML_NODISCARD static float64_t get_time();

		ML_NODISCARD static bool initialize();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static error_fn set_error_callback			(error_fn			fn);
		char_fn			set_char_callback			(char_fn			fn);
		cursor_enter_fn	set_cursor_enter_callback	(cursor_enter_fn	fn);
		cursor_pos_fn	set_cursor_pos_callback		(cursor_pos_fn		fn);
		frame_size_fn	set_frame_size_callback		(frame_size_fn		fn);
		key_fn			set_key_callback			(key_fn				fn);
		mouse_fn		set_mouse_callback			(mouse_fn			fn);
		scroll_fn		set_scroll_callback			(scroll_fn			fn);
		close_fn		set_window_close_callback	(close_fn			fn);
		focus_fn		set_window_focus_callback	(focus_fn			fn);
		position_fn		set_window_pos_callback		(position_fn		fn);
		size_fn			set_window_size_callback	(size_fn			fn);

		void install_default_callbacks();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_aspect() const noexcept -> float_t { return util::aspect(get_size()); };

		ML_NODISCARD auto get_context() const noexcept -> context_settings const & { return m_settings.context; }

		ML_NODISCARD auto get_frame_aspect() const noexcept -> float_t { return util::aspect(get_frame_size()); };

		ML_NODISCARD auto get_frame_height() const noexcept -> int32_t { return get_frame_size()[1]; }

		ML_NODISCARD auto get_frame_width() const noexcept -> int32_t { return get_frame_size()[0]; }

		ML_NODISCARD auto get_height() const noexcept -> int32_t const & { return get_size()[1]; }

		ML_NODISCARD auto get_hints() const -> int32_t const & { return m_settings.hints; }

		ML_NODISCARD auto get_monitor() const noexcept -> window_handle { return m_monitor; }

		ML_NODISCARD auto get_settings() const noexcept -> window_settings const & { return m_settings; }

		ML_NODISCARD auto get_share() const noexcept -> window_handle { return m_share; }

		ML_NODISCARD auto get_size() const noexcept -> vec2i const & { return get_video().size; }

		ML_NODISCARD auto get_title() const noexcept -> pmr::string const & { return m_settings.title; }

		ML_NODISCARD auto get_video() const noexcept -> video_mode const & { return m_settings.video; }

		ML_NODISCARD auto get_width() const noexcept -> int32_t const & { return get_size()[0]; }

		ML_NODISCARD bool has_hint(int32_t const i) const noexcept { return ML_flag_read(get_hints(), i); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_HPP_