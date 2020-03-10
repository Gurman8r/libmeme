#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <libmeme/Core/Memory.hpp>
#include <libmeme/Platform/Cursor.hpp>
#include <libmeme/Platform/KeyCode.hpp>
#include <libmeme/Platform/MouseButton.hpp>
#include <libmeme/Platform/WindowSettings.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_PLATFORM_API window : trackable, non_copyable
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

		window() noexcept;

		virtual ~window() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool create(
			pmr::string const & title,
			display_settings const & display,
			context_settings const & context,
			int32_t hints
		) noexcept;

		ML_NODISCARD bool create(window_settings const & value) noexcept;

		ML_NODISCARD virtual bool open();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window & close();
		
		window & destroy();

		window & iconify();
		
		window & make_context_current();
		
		window & maximize();
		
		window & restore();
		
		window & swap_buffers();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window & set_centered();
		
		window & set_clipboard(cstring const & value);
		
		window & set_cursor(void * value);
		
		window & set_cursor_mode(cursor::mode value);
		
		window & set_cursor_pos(vec2d const & value);
		
		window & set_fullscreen(bool value);
		
		window & set_icon(size_t w, size_t h, byte_t const * pixels);
		
		window & set_position(vec2i const & value);
		
		window & set_monitor(void * value);
		
		window & set_size(vec2i const & value);
		
		window & set_title(pmr::string const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool is_focused() const;
		
		ML_NODISCARD bool is_fullscreen() const;
		
		ML_NODISCARD bool is_open() const;
		
		ML_NODISCARD int32_t get_attribute(int32_t value) const;
		
		ML_NODISCARD cstring get_clipboard() const;
		
		ML_NODISCARD vec2 get_cursor_pos() const;
		
		ML_NODISCARD vec2i get_frame_size() const;
		
		ML_NODISCARD void * get_handle() const;

		ML_NODISCARD int32_t get_key(int32_t value) const;
		
		ML_NODISCARD int32_t get_input_mode(int32_t value) const;
		
		ML_NODISCARD int32_t get_mouse_button(int32_t value) const;
		
		ML_NODISCARD vec2i get_position() const;
		
		ML_NODISCARD void * get_raw_handle() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void	destroy_cursor(void * value);

		static void make_context_current(void * value);

		static void poll_events();

		static void swap_interval(int32_t value);

		static void terminate();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static void * create_custom_cursor(uint32_t w, uint32_t h, byte_t const * pixels);
		
		ML_NODISCARD static void * create_standard_cursor(cursor::shape value);

		ML_NODISCARD static int32_t extension_supported(cstring value);

		ML_NODISCARD static void * get_context_current();

		ML_NODISCARD static display_settings const & get_desktop_mode();
		
		ML_NODISCARD static pmr::vector<display_settings> const & get_fullscreen_modes();

		ML_NODISCARD static proc_fn get_proc_address(cstring value);
		
		ML_NODISCARD static pmr::vector<void *> const & get_monitors();

		ML_NODISCARD static float64_t get_time();

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

		static void install_default_callbacks(window * win);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto get_aspect() const noexcept -> float_t { return util::aspect(get_size()); };

		ML_NODISCARD inline auto get_context_settings() const noexcept -> context_settings const & { return m_settings.context; }

		ML_NODISCARD inline auto get_frame_aspect() const noexcept -> float_t { return util::aspect(get_frame_size()); };

		ML_NODISCARD inline auto get_frame_height() const noexcept -> int32_t { return get_frame_size()[1]; }

		ML_NODISCARD inline auto get_frame_width() const noexcept -> int32_t { return get_frame_size()[0]; }

		ML_NODISCARD inline auto get_height() const noexcept -> int32_t const & { return get_size()[1]; }
		
		ML_NODISCARD inline auto get_hints() const -> int32_t const & { return m_settings.hints; }

		ML_NODISCARD inline auto get_monitor() const noexcept -> void * { return m_monitor; }

		ML_NODISCARD inline auto get_settings() const noexcept -> window_settings const & { return m_settings; }

		ML_NODISCARD inline auto get_share() const noexcept -> void * { return m_share; }

		ML_NODISCARD inline auto get_size() const noexcept -> vec2i const & { return get_display_settings().resolution; }

		ML_NODISCARD inline auto get_title() const noexcept -> pmr::string const & { return m_settings.title; }

		ML_NODISCARD inline auto get_display_settings() const noexcept -> display_settings const & { return m_settings.display; }

		ML_NODISCARD inline auto get_width() const noexcept -> int32_t const & { return get_size()[0]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool has_hint(int32_t const i) const noexcept { return get_hints() & i; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		void *			m_window	; // 
		void * 			m_monitor	; // 
		void * 			m_share		; // 
		window_settings	m_settings	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_HPP_