#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Platform/ContextSettings.hpp>
#include <libmeme/Platform/Cursor.hpp>
#include <libmeme/Platform/VideoMode.hpp>
#include <libmeme/Platform/KeyCode.hpp>
#include <libmeme/Platform/MouseButton.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum WindowFlags_ : int32_t
	{
		WindowFlags_None,
		WindowFlags_Resizable		= (1 << 0),
		WindowFlags_Visible			= (1 << 1),
		WindowFlags_Decorated		= (1 << 2),
		WindowFlags_Focused			= (1 << 3),
		WindowFlags_AutoIconify		= (1 << 4),
		WindowFlags_Floating		= (1 << 5),
		WindowFlags_Maximized		= (1 << 6),
		WindowFlags_Fullscreen		= (1 << 7),
		WindowFlags_DoubleBuffered	= (1 << 8),

		// Resizable / Visible / Decorated / Focused / Auto Iconify
		WindowFlags_Default = 0
			| WindowFlags_Resizable
			| WindowFlags_Visible
			| WindowFlags_Decorated
			| WindowFlags_Focused
			| WindowFlags_AutoIconify,

		// Resizable / Decorated / Focused / Auto Iconify / Maximized
		WindowFlags_DefaultMaximized = 0
			| WindowFlags_Resizable
			| WindowFlags_Decorated
			| WindowFlags_Focused
			| WindowFlags_AutoIconify
			| WindowFlags_Maximized,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_PLATFORM_API Window : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using char_fn			= typename void(*)(void *, uint32_t);
		using close_fn			= typename void(*)(void *);
		using cursor_enter_fn	= typename void(*)(void *, int32_t);
		using cursor_pos_fn		= typename void(*)(void *, float64_t, float64_t);
		using error_fn			= typename void(*)(int32_t, C_string);
		using focus_fn			= typename void(*)(void *, int32_t);
		using frame_size_fn		= typename void(*)(void *, int32_t, int32_t);
		using key_fn			= typename void(*)(void *, int32_t, int32_t, int32_t, int32_t);
		using mouse_fn			= typename void(*)(void *, int32_t, int32_t, int32_t);
		using position_fn		= typename void(*)(void *, int32_t, int32_t);
		using proc_fn			= typename void *(*)(void);
		using scroll_fn			= typename void(*)(void *, float64_t, float64_t);
		using size_fn			= typename void(*)(void *, int32_t, int32_t);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window();

		virtual ~Window();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool create(
			std::string const & title, 
			VideoMode const & display,
			ContextSettings const & context,
			int32_t flags
		);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window & close();
		
		Window & destroy();

		Window & iconify();
		
		Window & make_context_current();
		
		Window & maximize();
		
		Window & restore();
		
		Window & swap_buffers();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window & set_centered();
		
		Window & set_clipboard(std::string const & value);
		
		Window & set_cursor(void * value);
		
		Window & set_cursor_mode(Cursor::Mode value);
		
		Window & set_cursor_pos(vec2i const & value);
		
		Window & set_fullscreen(bool value);
		
		Window & set_icon(size_t w, size_t h, byte_t const * pixels);
		
		Window & set_position(vec2i const & value);
		
		Window & set_monitor(void * value);
		
		Window & set_size(vec2u const & value);
		
		Window & set_title(std::string const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool is_focused() const;
		
		ML_NODISCARD bool is_fullscreen() const;
		
		ML_NODISCARD bool is_open() const;
		
		ML_NODISCARD int32_t get_attribute(int32_t value) const;
		
		ML_NODISCARD C_string get_clipboard() const;
		
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
		
		ML_NODISCARD static void * create_standard_cursor(Cursor::Shape value);

		ML_NODISCARD static int32_t extension_supported(C_string value);

		ML_NODISCARD static void * get_context_current();

		ML_NODISCARD static VideoMode const & get_desktop_mode();
		
		ML_NODISCARD static std::vector<VideoMode> const & get_fullscreen_modes();

		ML_NODISCARD static proc_fn get_proc_address(C_string value);
		
		ML_NODISCARD static std::vector<void *> const & get_monitors();

		ML_NODISCARD static float64_t get_time();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static error_fn set_error_callback			(error_fn value);
		char_fn			set_char_callback			(char_fn value);
		cursor_enter_fn	set_cursor_enter_callback	(cursor_enter_fn value);
		cursor_pos_fn	set_cursor_pos_callback		(cursor_pos_fn value);
		frame_size_fn	set_frame_size_callback		(frame_size_fn value);
		key_fn			set_key_callback			(key_fn value);
		mouse_fn		set_mouse_callback			(mouse_fn value);
		scroll_fn		set_scroll_callback			(scroll_fn value);
		close_fn		set_window_close_callback	(close_fn value);
		focus_fn		set_window_focus_callback	(focus_fn value);
		position_fn		set_window_pos_callback		(position_fn value);
		size_fn			set_window_size_callback	(size_fn value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto get_aspect() const -> float_t { return ML_ASPECT2(get_size()); };

		ML_NODISCARD inline auto get_context() const -> ContextSettings const & { return m_context; }

		ML_NODISCARD inline auto get_frame_aspect() const -> float_t { return ML_ASPECT2(get_frame_size()); };

		ML_NODISCARD inline auto get_frame_height() const -> int32_t { return get_frame_size()[1]; }

		ML_NODISCARD inline auto get_frame_width() const -> int32_t { return get_frame_size()[0]; }

		ML_NODISCARD inline auto get_height() const -> uint32_t { return get_size()[1]; }

		ML_NODISCARD inline auto get_monitor() const -> void * { return m_monitor; }

		ML_NODISCARD inline auto get_share() const -> void * { return m_share; }

		ML_NODISCARD inline auto get_size() const -> vec2u const & { return get_video_mode().resolution; }

		ML_NODISCARD inline auto get_flags() const -> int32_t const & { return m_flags; }

		ML_NODISCARD inline auto get_title() const -> std::string const & { return m_title; }

		ML_NODISCARD inline auto get_video_mode() const -> VideoMode const & { return m_video; }

		ML_NODISCARD inline auto get_width() const -> uint32_t { return get_size()[0]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		void * 			m_window;
		void * 			m_monitor;
		void * 			m_share;
		std::string		m_title;
		VideoMode		m_video;
		ContextSettings	m_context;
		int32_t			m_flags;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_HPP_