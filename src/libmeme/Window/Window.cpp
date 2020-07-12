#include <libmeme/Window/Window.hpp>
#include <libmeme/Core/StringUtility.hpp>
#include <libmeme/System/EventBus.hpp>
#include <libmeme/Window/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_WINDOW_GLFW)
#include "Impl/Impl_Window_GLFW.hpp"
using impl_window = _ML glfw_window;

#elif defined(ML_IMPL_WINDOW_WIN32)
#elif defined(ML_IMPL_WINDOW_SDL)
#elif defined(ML_IMPL_WINDOW_SFML)
// etc...

#else
#	error ""
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window::window() noexcept : m_window{ new impl_window }, m_wndcfg{}
	{
		ML_assert("failed creating window implementation" && m_window);
	}

	window::window(window_settings const & ws) noexcept : window{}
	{
		ML_assert(open(ws));
	}

	window::~window() noexcept {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool window::open(window_settings const & ws)
	{
		// check already open
		if (is_open()) { return debug::error("window is already open"); }

		// open backend
		if (!m_window->open(m_wndcfg = ws)) { return debug::error("failed opening window"); }
		
		// make current context
		make_context_current(get_handle());

		// user pointer
		set_user_pointer(this);

		// centered
		set_position((video_mode::get_desktop_mode().resolution - ws.video.resolution) / 2);

		// maximized
		if (ws.hints & window_hints_maximized) { maximize(); }

		// install callbacks
		{
			set_char_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_char_event>(ML_forward(x)...); });

			set_char_mods_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_char_mods_event>(ML_forward(x)...); });

			set_close_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_close_event>(ML_forward(x)...); });

			set_cursor_enter_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_cursor_enter_event>(ML_forward(x)...); });

			set_cursor_position_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_cursor_position_event>(ML_forward(x)...); });

			set_content_scale_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_content_scale_event>(ML_forward(x)...); });

			set_drop_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_drop_event>(ML_forward(x)...); });

			set_error_callback([
			](auto ... x) noexcept { event_bus::fire<window_error_event>(ML_forward(x)...); });

			set_focus_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_focus_event>(ML_forward(x)...); });

			set_framebuffer_size_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_framebuffer_size_event>(ML_forward(x)...); });

			set_iconify_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_iconify_event>(ML_forward(x)...); });

			set_key_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_key_event>(ML_forward(x)...); });

			set_maximize_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_maximize_event>(ML_forward(x)...);  });

			set_mouse_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_mouse_event>(ML_forward(x)...); });

			set_position_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_position_event>(ML_forward(x)...); });

			set_refresh_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_refresh_event>(ML_forward(x)...); });

			set_scroll_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_scroll_event>(ML_forward(x)...); });

			set_size_callback([
			](auto, auto ... x) noexcept { event_bus::fire<window_size_event>(ML_forward(x)...); });
		}

		// success
		return is_open();
	}

	void window::close()
	{
		m_window->close();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::iconify() noexcept
	{
		m_window->iconify();
	}

	void window::maximize() noexcept
	{
		m_window->maximize();
	}

	void window::restore() noexcept
	{
		m_window->restore();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int_rect window::get_bounds() const noexcept
	{
		return m_window->get_bounds();
	}

	cstring window::get_clipboard() const noexcept
	{
		return m_window->get_clipboard();
	}

	vec2 window::get_content_scale() const noexcept
	{
		return m_window->get_content_scale();
	}

	vec2 window::get_cursor_position() const noexcept
	{
		return m_window->get_cursor_position();
	}

	vec2i window::get_framebuffer_size() const noexcept
	{
		return m_window->get_framebuffer_size();
	}

	window_handle window::get_handle() const noexcept
	{
		return m_window->get_handle();
	}

	int32_t window::get_input_mode(int32_t value) const noexcept
	{
		return m_window->get_input_mode(value);
	}

	int32_t	window::get_key(int32_t value) const noexcept
	{
		return m_window->get_key(value);
	}

	int32_t	window::get_mouse_button(int32_t value) const noexcept
	{
		return m_window->get_mouse_button(value);
	}

	window_handle window::get_native_handle() const noexcept
	{
		return m_window->get_native_handle();
	}

	float_t window::get_opacity() const noexcept
	{
		return m_window->get_opacity();
	}

	vec2i window::get_position() const noexcept
	{
		return m_window->get_position();
	}

	vec2i window::get_size() const noexcept
	{
		return m_window->get_size();
	}

	cstring window::get_title() const noexcept
	{
		return m_window->get_title();
	}

	void * window::get_user_pointer() const noexcept
	{
		return m_window->get_user_pointer();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool window::is_auto_iconify() const noexcept
	{
		return m_window->is_auto_iconify();
	}

	bool window::is_decorated() const noexcept
	{
		return m_window->is_decorated();
	}

	bool window::is_floating() const noexcept
	{
		return m_window->is_floating();
	}

	bool window::is_focus_on_show() const noexcept
	{
		return m_window->is_focus_on_show();
	}

	bool window::is_focused() const noexcept
	{
		return m_window->is_focused();
	}

	bool window::is_hovered() const noexcept
	{
		return m_window->is_hovered();
	}

	bool window::is_iconified() const noexcept
	{
		return m_window->is_iconified();
	}

	bool window::is_maximized() const noexcept
	{
		return m_window->is_maximized();
	}

	bool window::is_open() const noexcept
	{
		return m_window->is_open();
	}

	bool window::is_resizable() const noexcept
	{
		return m_window->is_resizable();
	}

	bool window::is_transparent() const noexcept
	{
		return m_window->is_transparent();
	}

	bool window::is_visible() const noexcept
	{
		return m_window->is_visible();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::set_auto_iconify(bool value) noexcept
	{
		return m_window->set_auto_iconify(value);
	}

	void window::set_clipboard(cstring value) noexcept
	{
		m_window->set_clipboard(value);
	}

	void window::set_cursor(cursor_handle value) noexcept
	{
		m_window->set_cursor(value);
	}
	
	void window::set_cursor_mode(int32_t value) noexcept
	{
		m_window->set_cursor_mode(value);
	}

	void window::set_cursor_position(vec2d const & value) noexcept
	{
		m_window->set_cursor_position(value);
	}

	void window::set_decorated(bool value) noexcept
	{
		return m_window->set_decorated(value);
	}

	void window::set_floating(bool value) noexcept
	{
		return m_window->set_floating(value);
	}

	void window::set_focus_on_show(bool value) noexcept
	{
		return m_window->set_focus_on_show(value);
	}

	void window::set_icon(size_t w, size_t h, byte_t const * p) noexcept
	{
		m_window->set_icon(w, h, p);
	}

	void window::set_input_mode(int32_t mode, int32_t value) noexcept
	{
		m_window->set_input_mode(mode, value);
	}

	void window::set_opacity(float_t value) noexcept
	{
		m_window->set_opacity(value);
	}

	void window::set_position(vec2i const & value) noexcept
	{
		m_window->set_position(value);
	}

	void window::set_monitor(monitor_handle value, int_rect const & bounds) noexcept
	{
		m_window->set_monitor(value, bounds);
	}

	void window::set_resizable(bool value) noexcept
	{
		return m_window->set_resizable(value);
	}

	void window::set_size(vec2i const & value) noexcept
	{
		m_window->set_size(value);
	}

	void window::set_title(cstring value) noexcept
	{
		m_window->set_title(value);
	}

	void window::set_user_pointer(void * value) noexcept
	{
		m_window->set_user_pointer(value);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t window::extension_supported(cstring value) noexcept
	{
		return impl_window::extension_supported(value);
	}

	window_handle window::get_context_current() noexcept
	{
		return impl_window::get_context_current();
	}

	void * window::get_proc_address(cstring value) noexcept
	{
		return impl_window::get_proc_address(value);
	}

	pmr::vector<monitor_handle> const & window::get_monitors() noexcept
	{
		return impl_window::get_monitors();
	}

	monitor_handle window::get_primary_monitor() noexcept
	{
		return impl_window::get_primary_monitor();
	}

	duration window::get_time() noexcept
	{
		return impl_window::get_time();
	}

	void window::make_context_current(window_handle value) noexcept
	{
		impl_window::make_context_current(value);
	}

	void window::poll_events() noexcept
	{
		impl_window::poll_events();
	}

	void window::swap_buffers(window_handle value) noexcept
	{
		impl_window::swap_buffers(value);
	}

	void window::swap_interval(int32_t value) noexcept
	{
		impl_window::swap_interval(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	cursor_handle window::create_custom_cursor(size_t w, size_t h, byte_t const * p) noexcept
	{
		return impl_window::create_custom_cursor(w, h, p);
	}

	cursor_handle window::create_standard_cursor(int32_t value) noexcept
	{
		return impl_window::create_standard_cursor(value);
	}

	void window::destroy_cursor(cursor_handle value) noexcept
	{
		impl_window::destroy_cursor(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window_char_callback window::set_char_callback(window_char_callback fn) noexcept
	{
		return m_window->set_char_callback(fn);
	}

	window_char_mods_callback window::set_char_mods_callback(window_char_mods_callback fn) noexcept
	{
		return m_window->set_char_mods_callback(fn);
	}

	window_close_callback window::set_close_callback(window_close_callback fn) noexcept
	{
		return m_window->set_close_callback(fn);
	}

	window_content_scale_callback window::set_content_scale_callback(window_content_scale_callback fn) noexcept
	{
		return m_window->set_content_scale_callback(fn);
	}
	
	window_cursor_enter_callback window::set_cursor_enter_callback(window_cursor_enter_callback fn) noexcept
	{
		return m_window->set_cursor_enter_callback(fn);
	}

	window_cursor_position_callback window::set_cursor_position_callback(window_cursor_position_callback fn) noexcept
	{
		return m_window->set_cursor_position_callback(fn);
	}

	window_drop_callback window::set_drop_callback(window_drop_callback fn) noexcept
	{
		return m_window->set_drop_callback(fn);
	}

	window_error_callback window::set_error_callback(window_error_callback fn) noexcept
	{
		return m_window->set_error_callback(fn);
	}

	window_focus_callback window::set_focus_callback(window_focus_callback fn) noexcept
	{
		return m_window->set_focus_callback(fn);
	}

	window_framebuffer_size_callback window::set_framebuffer_size_callback(window_framebuffer_size_callback fn) noexcept
	{
		return m_window->set_framebuffer_size_callback(fn);
	}

	window_iconify_callback window::set_iconify_callback(window_iconify_callback fn) noexcept
	{
		return m_window->set_iconify_callback(fn);
	}
	
	window_key_callback window::set_key_callback(window_key_callback fn) noexcept
	{
		return m_window->set_key_callback(fn);
	}

	window_maximize_callback window::set_maximize_callback(window_maximize_callback fn) noexcept
	{
		return m_window->set_maximize_callback(fn);
	}
	
	window_mouse_callback window::set_mouse_callback(window_mouse_callback fn) noexcept
	{
		return m_window->set_mouse_callback(fn);
	}
	
	window_position_callback window::set_position_callback(window_position_callback fn) noexcept
	{
		return m_window->set_position_callback(fn);
	}

	window_refresh_callback window::set_refresh_callback(window_refresh_callback fn) noexcept
	{
		return m_window->set_refresh_callback(fn);
	}

	window_scroll_callback window::set_scroll_callback(window_scroll_callback fn) noexcept
	{
		return m_window->set_scroll_callback(fn);
	}
	
	window_size_callback window::set_size_callback(window_size_callback fn) noexcept
	{
		return m_window->set_size_callback(fn);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}