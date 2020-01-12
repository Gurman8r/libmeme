#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <libmeme/Core/StringUtility.hpp>
#include <libmeme/Core/EventListener.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Platform/ContextSettings.hpp>
#include <libmeme/Platform/Cursor.hpp>
#include <libmeme/Platform/DisplayMode.hpp>
#include <libmeme/Platform/KeyCode.hpp>
#include <libmeme/Platform/MouseButton.hpp>
#include <libmeme/Platform/WindowSettings.hpp>

#define ML_ASPECT(w, h) ((w != 0 && h != 0) ? ((float_t)w / (float_t)(h)) : 0.0f)
#define ML_ASPECT2(v)	ML_ASPECT(v[0], v[1])

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PLATFORM_API Window : public Trackable, public NonCopyable, public EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using CharFun			= typename void(*)(void *, uint32_t);
		using CursorEnterFun	= typename void(*)(void *, int32_t);
		using CursorPosFun		= typename void(*)(void *, float64_t, float64_t);
		using ErrorFun			= typename void(*)(int32_t, C_String);
		using FrameSizeFun		= typename void(*)(void *, int32_t, int32_t);
		using KeyFun			= typename void(*)(void *, int32_t, int32_t, int32_t, int32_t);
		using MouseFun			= typename void(*)(void *, int32_t, int32_t, int32_t);
		using ScrollFun			= typename void(*)(void *, float64_t, float64_t);
		using CloseFun			= typename void(*)(void *);
		using FocusFun			= typename void(*)(void *, int32_t);
		using PositionFun		= typename void(*)(void *, int32_t, int32_t);
		using SizeFun			= typename void(*)(void *, int32_t, int32_t);
		using ProcFun			= typename void *(*)(void);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window();

		virtual ~Window();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool create(
			std::string const & title, 
			DisplayMode const & display,
			WindowSettings const & settings,
			ContextSettings const & context
		);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void onEvent(Event const & value) override;

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

		bool is_focused() const;
		
		bool is_fullscreen() const;
		
		bool is_open() const;
		
		int32_t	get_attribute(int32_t value) const;
		
		C_String get_clipboard() const;
		
		vec2 get_cursor_pos() const;
		
		vec2i get_frame_size() const;
		
		void * get_handle() const;

		int32_t get_key(int32_t value) const;
		
		int32_t	get_input_mode(int32_t value) const;
		
		int32_t	get_mouse_button(int32_t value) const;
		
		vec2i get_position() const;
		
		void * get_raw_handle() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto get_aspect() const -> float_t { return ML_ASPECT2(get_size()); };
		
		inline auto get_context() const -> ContextSettings const & { return m_context; }
		
		inline auto get_frame_aspect() const -> float_t { return ML_ASPECT2(get_frame_size()); };
		
		inline auto get_frame_height() const -> int32_t { return get_frame_size()[1]; }
		
		inline auto get_frame_width() const -> int32_t { return get_frame_size()[0]; }
		
		inline auto get_height() const -> uint32_t { return get_size()[1]; }
		
		inline auto get_monitor() const -> void * { return m_monitor; }

		inline auto get_share() const -> void * { return m_share; }
		
		inline auto get_size() const -> vec2u const & { return get_display_mode().size; }

		inline auto get_settings() const -> WindowSettings const & { return m_settings; }
		
		inline auto get_title() const -> std::string const & { return m_title; }
		
		inline auto get_display_mode() const -> DisplayMode const & { return m_video; }
		
		inline auto get_width() const -> uint32_t { return get_size()[0]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void * create_custom_cursor(uint32_t w, uint32_t h, byte_t const * pixels);
		
		static void * create_standard_cursor(Cursor::Shape value);
		
		static bool	destroy_cursor(void * value);

		static int32_t extension_supported(C_String value);

		static void * get_context_current();

		static DisplayMode const & get_desktop_mode();
		
		static std::vector<DisplayMode> const & get_fullscreen_modes();

		static ProcFun get_proc_address(C_String value);
		
		static std::vector<void *> const & get_monitors();

		static float64_t get_time();

		static void make_context_current(void * value);

		static void poll_events();
		
		static void swap_interval(int32_t value);

		static void terminate();
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static ErrorFun set_error_callback			(ErrorFun		value);
		CharFun			set_char_callback			(CharFun		value);
		CursorEnterFun	set_cursor_enter_callback	(CursorEnterFun value);
		CursorPosFun	set_cursor_pos_callback		(CursorPosFun	value);
		FrameSizeFun	set_frame_size_callback		(FrameSizeFun	value);
		KeyFun			set_key_callback			(KeyFun			value);
		MouseFun		set_mouse_callback			(MouseFun		value);
		ScrollFun		set_scroll_callback			(ScrollFun		value);
		CloseFun		set_window_close_callback	(CloseFun		value);
		FocusFun		set_window_focus_callback	(FocusFun		value);
		PositionFun		set_window_pos_callback		(PositionFun	value);
		SizeFun			set_window_size_callback	(SizeFun		value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		void * 			m_window;
		void * 			m_monitor;
		void * 			m_share;
		ContextSettings	m_context;
		WindowSettings	m_settings;
		DisplayMode		m_video;
		std::string		m_title;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_HPP_