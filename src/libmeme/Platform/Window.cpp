#include <libmeme/Platform/Window.hpp>
#include <libmeme/Core/StringUtility.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_PLATFORM_GLFW)
#include "Impl/Impl_Window_Glfw.hpp"
using ml_window_impl = _ML impl_window_glfw;
#else
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window::window() noexcept
	{
		ML_assert(m_impl = new ml_window_impl{});
	}

	window::~window() noexcept
	{
		delete m_impl;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool window::open(window_settings const & ws)
	{
		if (is_open())			{ return debug::error(""); }
		if (ws.title.empty())	{ return debug::error(""); }
		if (!ws.video)			{ return debug::error(""); }
		if (!ws.context)		{ return debug::error(""); }
		if (!ws.hints)			{ return debug::error(""); }
		if (!m_impl->open(ws))	{ return debug::error(""); }

		m_settings = ws;

		make_context_current();

		set_cursor_mode(cursor_mode_normal);

		if (ws.hints & window_hints_install_callbacks)
		{
			install_default_callbacks();
		}

		if (ws.hints & window_hints_fullscreen)
		{
			// fullscreen
			set_fullscreen(true);
		}
		else if (ws.hints & window_hints_maximized)
		{
			// maximized
			maximize();
		}
		else
		{
			// centered
			set_position((get_desktop_mode().size - get_frame_size()) / 2);
		}

		return is_open();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::destroy()
	{
		m_impl->destroy();
	}

	void window::iconify()
	{
		m_impl->iconify();
	}

	void window::make_context_current()
	{
		m_impl->make_context_current();
	}
	
	void window::maximize()
	{
		m_impl->maximize();
	}

	void window::restore()
	{
		m_impl->restore();
	}

	void window::swap_buffers()
	{
		if (m_settings.hints & window_hints_double_buffered)
		{
			m_impl->swap_buffers();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::set_clipboard(cstring value)
	{
		m_impl->set_clipboard(value);
	}

	void window::set_cursor(cursor_handle value)
	{
		m_impl->set_cursor(value);
	}
	
	void window::set_cursor_mode(int32_t value)
	{
		m_impl->set_cursor_mode(value);
	}

	void window::set_cursor_position(vec2d const & value)
	{
		m_impl->set_cursor_position(value);
	}

	void window::set_fullscreen(bool value)
	{
		m_impl->set_fullscreen(value);

		if (!value)
		{
			set_size(m_settings.video.size);
			set_position((get_desktop_mode().size - get_frame_size()) / 2);
		}
	}

	void window::set_icon(size_t w, size_t h, byte_t const * p)
	{
		m_impl->set_icon(w, h, p);
	}

	void window::set_input_mode(int32_t mode, int32_t value)
	{
		m_impl->set_input_mode(mode, value);
	}

	void window::set_position(vec2i const & value)
	{
		m_impl->set_position(value);
	}

	void window::set_monitor(window_handle value)
	{
		m_impl->set_monitor(value);
	}

	void window::set_should_close(bool value)
	{
		m_impl->set_should_close(value);
	}

	void window::set_size(vec2i const & value)
	{
		m_impl->set_size(value);
	}

	void window::set_title(cstring value)
	{
		m_settings.title = value;
		m_impl->set_title(value);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool window::is_focused() const
	{
		return m_impl->is_focused();
	}

	bool window::is_fullscreen() const
	{
		return m_impl->is_fullscreen();
	}

	bool window::is_open() const
	{
		return m_impl->is_open();
	}
	
	int32_t window::get_attribute(int32_t value) const
	{
		return m_impl->get_attribute(value);
	}

	cstring window::get_clipboard() const
	{
		return m_impl->get_clipboard();
	}

	vec2 window::get_cursor_pos() const
	{
		return m_impl->get_cursor_pos();
	}

	vec2i window::get_frame_size() const
	{
		return m_impl->get_frame_size();
	}

	window_handle window::get_handle() const
	{
		return m_impl->get_handle();
	}

	int32_t window::get_input_mode(int32_t mode) const
	{
		return m_impl->get_input_mode(mode);
	}

	int32_t	window::get_key(int32_t key) const
	{
		return m_impl->get_key(key);
	}

	int32_t	window::get_mouse_button(int32_t button) const
	{
		return m_impl->get_mouse_button(button);
	}

	window_handle window::get_native_handle() const
	{
		return m_impl->get_native_handle();
	}

	vec2i window::get_position() const
	{
		return m_impl->get_position();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::destroy_cursor(cursor_handle value)
	{
		ml_window_impl::destroy_cursor(value);
	}

	void window::make_context_current(window_handle value)
	{
		ml_window_impl::make_context_current(value);
	}

	void window::poll_events()
	{
		ml_window_impl::poll_events();
	}

	void window::swap_interval(int32_t value)
	{
		ml_window_impl::swap_interval(value);
	}

	void window::terminate()
	{
		ml_window_impl::terminate();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	cursor_handle window::create_custom_cursor(size_t w, size_t h, byte_t const * p)
	{
		return ml_window_impl::create_custom_cursor(w, h, p);
	}

	cursor_handle window::create_standard_cursor(int32_t value)
	{
		return ml_window_impl::create_standard_cursor(value);
	}

	int32_t window::extension_supported(cstring value)
	{
		return ml_window_impl::extension_supported(value);
	}

	window_handle window::get_context_current()
	{
		return ml_window_impl::get_context_current();
	}

	video_mode const & window::get_desktop_mode()
	{
		return ml_window_impl::get_desktop_mode();
	}

	ds::flat_set<video_mode> const & window::get_fullscreen_modes()
	{
		return ml_window_impl::get_fullscreen_modes();
	}

	void * window::get_proc_address(cstring value)
	{
		return ml_window_impl::get_proc_address(value);
	}

	pmr::vector<window_handle> const & window::get_monitors()
	{
		return ml_window_impl::get_monitors();
	}



	float64_t window::get_time()
	{
		return ml_window_impl::get_time();
	}

	bool window::initialize()
	{
		return ml_window_impl::initialize();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window_char_fn window::set_char_callback(window_char_fn fn)
	{
		return m_impl->set_char_callback(fn);
	}
	
	window_cursor_enter_fn window::set_cursor_enter_callback(window_cursor_enter_fn fn)
	{
		return m_impl->set_cursor_enter_callback(fn);
	}

	window_cursor_pos_fn window::set_cursor_pos_callback(window_cursor_pos_fn fn)
	{
		return m_impl->set_cursor_pos_callback(fn);
	}

	window_error_fn window::set_error_callback(window_error_fn fn)
	{
		return m_impl->set_error_callback(fn);
	}

	window_frame_size_fn window::set_frame_size_callback(window_frame_size_fn fn)
	{
		return m_impl->set_frame_size_callback(fn);
	}
	
	window_key_fn window::set_key_callback(window_key_fn fn)
	{
		return m_impl->set_key_callback(fn);
	}
	
	window_mouse_fn window::set_mouse_callback(window_mouse_fn fn)
	{
		return m_impl->set_mouse_callback(fn);
	}
	
	window_scroll_fn window::set_scroll_callback(window_scroll_fn fn)
	{
		return m_impl->set_scroll_callback(fn);
	}
	
	window_close_fn window::set_window_close_callback(window_close_fn fn)
	{
		return m_impl->set_window_close_callback(fn);
	}
	
	window_focus_fn window::set_window_focus_callback(window_focus_fn fn)
	{
		return m_impl->set_window_focus_callback(fn);
	}
	
	window_position_fn window::set_window_pos_callback(window_position_fn fn)
	{
		return m_impl->set_window_pos_callback(fn);
	}
	
	window_size_fn window::set_window_size_callback(window_size_fn fn)
	{
		return m_impl->set_window_size_callback(fn);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::install_default_callbacks()
	{
		if (!is_open()) { return; }
	
		set_char_callback([](auto, auto ... args)
		{
			event_system::fire_event<char_event>(ML_forward(args)...);
		});
	
		set_cursor_enter_callback([](auto, auto ... args)
		{
			event_system::fire_event<cursor_enter_event>(ML_forward(args)...);
		});
	
		set_cursor_pos_callback([](auto, auto ... args)
		{
			event_system::fire_event<cursor_position_event>(ML_forward(args)...);
		});
	
		set_error_callback([](auto ... args)
		{
			event_system::fire_event<window_error_event>(ML_forward(args)...);
		});
	
		set_frame_size_callback([](auto, auto ... args)
		{
			event_system::fire_event<frame_size_event>(ML_forward(args)...);
		});
	
		set_key_callback([](auto, auto ... args)
		{
			event_system::fire_event<key_event>(ML_forward(args)...);
		});
	
		set_mouse_callback([](auto, auto ... args)
		{
			event_system::fire_event<mouse_event>(ML_forward(args)...);
		});
	
		set_scroll_callback([](auto, auto ... args)
		{
			event_system::fire_event<scroll_event>(ML_forward(args)...);
		});
	
		set_window_close_callback([](auto)
		{
			event_system::fire_event<window_close_event>();
		});
	
		set_window_focus_callback([](auto, auto ... args)
		{
			event_system::fire_event<window_focus_event>(ML_forward(args)...);
		});
	
		set_window_pos_callback([](auto, auto ... args)
		{
			event_system::fire_event<window_position_event>(ML_forward(args)...);
		});
	
		set_window_size_callback([](auto, auto ... args)
		{
			event_system::fire_event<window_size_event>(ML_forward(args)...);
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}