#include <libmeme/Platform/Window.hpp>
#include <libmeme/Core/StringUtility.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Platform/WindowEvents.hpp>
#include <libmeme/Platform/Native.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// implementation
#if defined(ML_IMPL_WINDOW_GLFW)
#include "Impl/Impl_Window_GLFW.hpp"
using impl_window = _ML glfw_window;

#elif defined(ML_IMPL_WINDOW_WIN32)
#elif defined(ML_IMPL_WINDOW_SDL)
#elif defined(ML_IMPL_WINDOW_SFML)

#else
#	error "window is unavailable"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window::window() noexcept : m_window{ new impl_window }, m_settings{}
	{
	}

	window::window(window_settings const & ws, bool ic) noexcept : window{}
	{
		ML_assert(this->open(ws, ic));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool window::open(window_settings const & ws, bool ic)
	{
		// open backend
		if (!m_window->open(m_settings = ws))
		{
			return debug::error("failed opening window implementation");
		}
		
		// make context current
		set_current_context(get_handle());

		// centered
		set_position((video_mode::get_desktop_mode().resolution - ws.video.resolution) / 2);

		// maximized
		if (get_hint(window_hints_maximized)) { maximize(); }

		// install callbacks
		if (ic)
		{
			set_char_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_char_event>(ML_forward(x)...); });

			set_char_mods_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_char_mods_event>(ML_forward(x)...); });

			set_close_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_close_event>(ML_forward(x)...); });

			set_cursor_enter_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_cursor_enter_event>(ML_forward(x)...); });

			set_cursor_position_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_cursor_position_event>(ML_forward(x)...); });

			set_content_scale_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_content_scale_event>(ML_forward(x)...); });

			set_drop_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_drop_event>(ML_forward(x)...); });

			set_error_callback([
			](auto ... x) noexcept { event_system::fire_event<window_error_event>(ML_forward(x)...); });

			set_focus_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_focus_event>(ML_forward(x)...); });

			set_framebuffer_size_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_framebuffer_size_event>(ML_forward(x)...); });

			set_iconify_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_iconify_event>(ML_forward(x)...); });

			set_key_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_key_event>(ML_forward(x)...); });

			set_maximize_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_maximize_event>(ML_forward(x)...);  });

			set_mouse_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_mouse_event>(ML_forward(x)...); });

			set_position_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_position_event>(ML_forward(x)...); });

			set_refresh_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_refresh_event>(ML_forward(x)...); });

			set_scroll_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_scroll_event>(ML_forward(x)...); });

			set_size_callback([
			](auto, auto ... x) noexcept { event_system::fire_event<window_size_event>(ML_forward(x)...); });
		}

		// success
		return is_open();
	}

	void window::close()
	{
		m_window->close();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::iconify()
	{
		m_window->iconify();
	}

	void window::maximize()
	{
		m_window->maximize();
	}

	void window::restore()
	{
		m_window->restore();
	}

	void window::swap_buffers()
	{
		m_window->swap_buffers();
		
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool window::is_fullscreen() const
	{
		return m_window->is_fullscreen();
	}

	bool window::is_open() const
	{
		return m_window->is_open();
	}
	
	int32_t window::get_attribute(int32_t value) const
	{
		return m_window->get_attribute(value);
	}

	int_rect window::get_bounds() const
	{
		return m_window->get_bounds();
	}

	cstring window::get_clipboard_string() const
	{
		return m_window->get_clipboard_string();
	}

	vec2 window::get_content_scale() const
	{
		return m_window->get_content_scale();
	}

	vec2 window::get_cursor_position() const
	{
		return m_window->get_cursor_position();
	}

	vec2i window::get_framebuffer_size() const
	{
		return m_window->get_framebuffer_size();
	}

	window_handle window::get_handle() const
	{
		return m_window->get_handle();
	}

	int32_t window::get_input_mode(int32_t mode) const
	{
		return m_window->get_input_mode(mode);
	}

	int32_t	window::get_key(int32_t key) const
	{
		return m_window->get_key(key);
	}

	int32_t	window::get_mouse_button(int32_t button) const
	{
		return m_window->get_mouse_button(button);
	}

	window_handle window::get_native_handle() const
	{
		return m_window->get_native_handle();
	}

	float_t window::get_opacity() const
	{
		return m_window->get_opacity();
	}

	vec2i window::get_position() const
	{
		return m_window->get_position();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::set_clipboard_string(cstring value)
	{
		m_window->set_clipboard_string(value);
	}

	void window::set_cursor(cursor_handle value)
	{
		m_window->set_cursor(value);
	}
	
	void window::set_cursor_mode(int32_t value)
	{
		m_window->set_cursor_mode(value);
	}

	void window::set_cursor_position(vec2d const & value)
	{
		m_window->set_cursor_position(value);
	}

	void window::set_fullscreen(bool value)
	{
		m_window->set_fullscreen(value);

		if (!value)
		{
			set_size(m_settings.video.resolution);
			
			set_position((video_mode::get_desktop_mode().resolution - get_framebuffer_size()) / 2);
		}
	}

	void window::set_icon(size_t w, size_t h, byte_t const * p)
	{
		m_window->set_icon(w, h, p);
	}

	void window::set_input_mode(int32_t mode, int32_t value)
	{
		m_window->set_input_mode(mode, value);
	}

	void window::set_opacity(float_t value)
	{
		m_window->set_opacity(value);
	}

	void window::set_position(vec2i const & value)
	{
		m_window->set_position(value);
	}

	void window::set_monitor(monitor_handle value, int_rect const & bounds)
	{
		m_window->set_monitor(value, bounds);
	}

	void window::set_size(vec2i const & value)
	{
		m_window->set_size(value);
	}

	void window::set_title(cstring value)
	{
		m_settings.title = value;
		m_window->set_title(value);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	cursor_handle window::create_custom_cursor(size_t w, size_t h, byte_t const * p)
	{
		return impl_window::create_custom_cursor(w, h, p);
	}

	cursor_handle window::create_standard_cursor(int32_t value)
	{
		return impl_window::create_standard_cursor(value);
	}

	int32_t window::extension_supported(cstring value)
	{
		return impl_window::extension_supported(value);
	}

	window_handle window::get_current_context()
	{
		return impl_window::get_current_context();
	}

	window_proc window::get_proc_address(cstring value)
	{
		return impl_window::get_proc_address(value);
	}

	pmr::vector<monitor_handle> const & window::get_monitors()
	{
		return impl_window::get_monitors();
	}

	monitor_handle window::get_primary_monitor()
	{
		return impl_window::get_primary_monitor();
	}

	duration window::get_time()
	{
		return impl_window::get_time();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::destroy_cursor(cursor_handle value)
	{
		impl_window::destroy_cursor(value);
	}

	void window::poll_events()
	{
		impl_window::poll_events();
	}

	void window::set_current_context(window_handle value)
	{
		impl_window::set_current_context(value);
	}

	void window::set_swap_interval(int32_t value)
	{
		impl_window::set_swap_interval(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window_char_fn window::set_char_callback(window_char_fn fn)
	{
		return m_window->set_char_callback(fn);
	}

	window_char_mods_fn window::set_char_mods_callback(window_char_mods_fn fn)
	{
		return m_window->set_char_mods_callback(fn);
	}

	window_close_fn window::set_close_callback(window_close_fn fn)
	{
		return m_window->set_close_callback(fn);
	}

	window_content_scale_fn window::set_content_scale_callback(window_content_scale_fn fn)
	{
		return m_window->set_content_scale_callback(fn);
	}
	
	window_cursor_enter_fn window::set_cursor_enter_callback(window_cursor_enter_fn fn)
	{
		return m_window->set_cursor_enter_callback(fn);
	}

	window_cursor_position_fn window::set_cursor_position_callback(window_cursor_position_fn fn)
	{
		return m_window->set_cursor_position_callback(fn);
	}

	window_drop_fn window::set_drop_callback(window_drop_fn fn)
	{
		return m_window->set_drop_callback(fn);
	}

	window_error_fn window::set_error_callback(window_error_fn fn)
	{
		return m_window->set_error_callback(fn);
	}

	window_focus_fn window::set_focus_callback(window_focus_fn fn)
	{
		return m_window->set_focus_callback(fn);
	}

	window_framebuffer_size_fn window::set_framebuffer_size_callback(window_framebuffer_size_fn fn)
	{
		return m_window->set_framebuffer_size_callback(fn);
	}

	window_iconify_fn window::set_iconify_callback(window_iconify_fn fn)
	{
		return m_window->set_iconify_callback(fn);
	}
	
	window_key_fn window::set_key_callback(window_key_fn fn)
	{
		return m_window->set_key_callback(fn);
	}

	window_maximize_fn window::set_maximize_callback(window_maximize_fn fn)
	{
		return m_window->set_maximize_callback(fn);
	}
	
	window_mouse_fn window::set_mouse_callback(window_mouse_fn fn)
	{
		return m_window->set_mouse_callback(fn);
	}
	
	window_position_fn window::set_position_callback(window_position_fn fn)
	{
		return m_window->set_position_callback(fn);
	}

	window_refresh_fn window::set_refresh_callback(window_refresh_fn fn)
	{
		return m_window->set_refresh_callback(fn);
	}

	window_scroll_fn window::set_scroll_callback(window_scroll_fn fn)
	{
		return m_window->set_scroll_callback(fn);
	}
	
	window_size_fn window::set_size_callback(window_size_fn fn)
	{
		return m_window->set_size_callback(fn);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}