#ifdef ML_PLATFORM_GLFW

#include "Impl_Window_Glfw.hpp"
#include <libmeme/Core/StringUtility.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <glfw/glfw3.h>

#ifdef ML_os_windows
#	undef APIENTRY
#	include <Windows.h>
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <glfw/glfw3native.h>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
	static GLFWimage const * glfw_image(vec2s const & s, byte_t const * p) noexcept
	{
		if (!s[0] || !s[1] || !p) return nullptr;
		static pmr::vector<GLFWimage> cache{};
		return &cache.emplace_back
		(
			GLFWimage{ (int32_t)s[0], (int32_t)s[1], (byte_t *)p }
		);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool impl_window_glfw::open(window_settings const & ws)
	{
		if (m_window) { return false; }

		// window hints
		glfwWindowHint(GLFW_CLIENT_API, ([&]() noexcept
		{
			switch (ws.context.api)
			{
			case window_api::opengl	: return GLFW_OPENGL_API;
			case window_api::vulkan	:
			case window_api::directx:
			default					: return GLFW_NO_API;
			}
		})());
		glfwWindowHint(GLFW_OPENGL_PROFILE, ([&]() noexcept
		{
			switch (ws.context.profile)
			{
			case window_api::core	: return GLFW_OPENGL_CORE_PROFILE;
			case window_api::compat	: return GLFW_OPENGL_COMPAT_PROFILE;
			case window_api::debug	: return GLFW_OPENGL_DEBUG_CONTEXT;
			case window_api::any	:
			default					: return GLFW_OPENGL_ANY_PROFILE;
			}
		})());
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,	ws.context.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,	ws.context.minor);
		glfwWindowHint(GLFW_DEPTH_BITS,				ws.context.depth_bits);
		glfwWindowHint(GLFW_STENCIL_BITS,			ws.context.stencil_bits);
		glfwWindowHint(GLFW_SRGB_CAPABLE,			ws.context.srgb_capable);
		glfwWindowHint(GLFW_RESIZABLE,				ws.hints & window_hints_resizable);
		glfwWindowHint(GLFW_VISIBLE,				ws.hints & window_hints_visible);
		glfwWindowHint(GLFW_DECORATED,				ws.hints & window_hints_decorated);
		glfwWindowHint(GLFW_FOCUSED,				ws.hints & window_hints_focused);
		glfwWindowHint(GLFW_AUTO_ICONIFY,			ws.hints & window_hints_auto_iconify);
		glfwWindowHint(GLFW_FLOATING,				ws.hints & window_hints_floating);
		glfwWindowHint(GLFW_MAXIMIZED,				ws.hints & window_hints_maximized);
		glfwWindowHint(GLFW_DOUBLEBUFFER,			ws.hints & window_hints_double_buffered);
		
		// create window
		return m_window = glfwCreateWindow(
			ws.video.size[0],
			ws.video.size[1],
			ws.title.c_str(),
			static_cast<GLFWmonitor *>(m_monitor),
			static_cast<GLFWwindow *>(m_share)
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void impl_window_glfw::destroy()
	{
		glfwDestroyWindow(static_cast<GLFWwindow *>(m_window));
	}

	void impl_window_glfw::iconify()
	{
		glfwIconifyWindow(static_cast<GLFWwindow *>(m_window));
	}

	void impl_window_glfw::make_context_current()
	{
		make_context_current(m_window);
	}
	
	void impl_window_glfw::maximize()
	{
		glfwMaximizeWindow(static_cast<GLFWwindow *>(m_window));
	}

	void impl_window_glfw::restore()
	{
		glfwRestoreWindow(static_cast<GLFWwindow *>(m_window));
	}

	void impl_window_glfw::swap_buffers()
	{
		glfwSwapBuffers(static_cast<GLFWwindow *>(m_window));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void impl_window_glfw::set_clipboard(cstring value)
	{
		glfwSetClipboardString(static_cast<GLFWwindow *>(m_window), value);
	}

	void impl_window_glfw::set_cursor(cursor_handle value)
	{
		glfwSetCursor(static_cast<GLFWwindow *>(m_window), static_cast<GLFWcursor *>(value));
	}
	
	void impl_window_glfw::set_cursor_mode(int32_t value)
	{
		set_input_mode(GLFW_CURSOR, ([value]() noexcept
		{
			switch (value)
			{
			case cursor_mode_hidden		: return GLFW_CURSOR_HIDDEN;
			case cursor_mode_disabled	: return GLFW_CURSOR_DISABLED;
			case cursor_mode_normal		:
			default						: return GLFW_CURSOR_NORMAL;
			}
		})());
	}

	void impl_window_glfw::set_cursor_position(vec2d const & value)
	{
		glfwSetCursorPos(static_cast<GLFWwindow *>(m_window), value[0], value[1]);
	}

	void impl_window_glfw::set_fullscreen(bool value)
	{
		set_monitor(value ? get_primary_monitor() : nullptr);
	}

	void impl_window_glfw::set_icon(size_t w, size_t h, byte_t const * p)
	{
		if (!w || !h || !p) { return; }

		glfwSetWindowIcon(static_cast<GLFWwindow *>(m_window), 1, impl::glfw_image({ w, h }, p));
	}

	void impl_window_glfw::set_input_mode(int32_t mode, int32_t value)
	{
		glfwSetInputMode(static_cast<GLFWwindow *>(m_window), mode, value);
	}

	void impl_window_glfw::set_position(vec2i const & value)
	{
		glfwSetWindowPos(static_cast<GLFWwindow *>(m_window), value[0], value[1]);
	}

	void impl_window_glfw::set_monitor(window_handle value)
	{
		// FIXME: need default window size when no monitor

		m_monitor = value;
		auto const vm
		{
			m_monitor ? glfwGetVideoMode(static_cast<GLFWmonitor *>(m_monitor)) : nullptr
		};
		glfwSetWindowMonitor(
			static_cast<GLFWwindow *>(m_window),
			static_cast<GLFWmonitor *>(m_monitor),
			0, 0,
			vm ? vm->width : 480,
			vm ? vm->height : 320,
			GLFW_DONT_CARE
		);
	}

	void impl_window_glfw::set_should_close(bool value)
	{
		glfwSetWindowShouldClose(static_cast<GLFWwindow *>(m_window), value);
	}

	void impl_window_glfw::set_size(vec2i const & value)
	{
		glfwSetWindowSize(static_cast<GLFWwindow *>(m_window), value[0], value[1]);
	}

	void impl_window_glfw::set_title(cstring value)
	{
		glfwSetWindowTitle(static_cast<GLFWwindow *>(m_window), value);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool impl_window_glfw::is_focused() const
	{
		return get_attribute(GLFW_FOCUSED);
	}

	bool impl_window_glfw::is_fullscreen() const
	{
		return m_monitor == get_primary_monitor();
	}

	bool impl_window_glfw::is_open() const
	{
		return m_window && !glfwWindowShouldClose(static_cast<GLFWwindow *>(m_window));
	}
	
	int32_t impl_window_glfw::get_attribute(int32_t value) const
	{
		return glfwGetWindowAttrib(static_cast<GLFWwindow *>(m_window), value);
	}

	cstring impl_window_glfw::get_clipboard() const
	{
		return glfwGetClipboardString(static_cast<GLFWwindow *>(m_window));
	}

	vec2 impl_window_glfw::get_cursor_pos() const
	{
		vec2d temp{};
		glfwGetCursorPos(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		return (vec2)temp;
	}

	vec2i impl_window_glfw::get_frame_size() const
	{
		vec2i temp{};
		glfwGetFramebufferSize(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		return temp;
	}

	window_handle impl_window_glfw::get_handle() const
	{
		return m_window;
	}

	int32_t impl_window_glfw::get_input_mode(int32_t mode) const
	{
		return glfwGetInputMode(static_cast<GLFWwindow *>(m_window), mode);
	}

	int32_t	impl_window_glfw::get_key(int32_t key) const
	{
		return glfwGetKey(static_cast<GLFWwindow *>(m_window), key);
	}

	int32_t	impl_window_glfw::get_mouse_button(int32_t button) const
	{
		return glfwGetMouseButton(static_cast<GLFWwindow *>(m_window), button);
	}

	window_handle impl_window_glfw::get_native_handle() const
	{
#ifdef ML_os_windows
		return glfwGetWin32Window(static_cast<GLFWwindow *>(m_window));
#else
		return m_window;
#endif
	}

	vec2i impl_window_glfw::get_position() const
	{
		vec2i temp{};
		if (m_window)
		{
			glfwGetWindowPos(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void impl_window_glfw::destroy_cursor(cursor_handle value)
	{
		if (!value) { return; }

		glfwDestroyCursor(static_cast<GLFWcursor *>(value));
	}

	void impl_window_glfw::make_context_current(window_handle value)
	{
		if (!value) { return; }

		glfwMakeContextCurrent(static_cast<GLFWwindow *>(value));
	}

	void impl_window_glfw::poll_events()
	{
		glfwPollEvents();
	}

	void impl_window_glfw::swap_interval(int32_t value)
	{
		glfwSwapInterval(value);
	}

	void impl_window_glfw::terminate()
	{
		glfwTerminate();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	cursor_handle impl_window_glfw::create_custom_cursor(size_t w, size_t h, byte_t const * p)
	{
		return glfwCreateCursor(impl::glfw_image({ w, h }, p), w, h);
	}

	cursor_handle impl_window_glfw::create_standard_cursor(int32_t value)
	{
		return glfwCreateStandardCursor(([value]() noexcept
		{
			switch (value)
			{
			case cursor_shape_arrow		: return GLFW_ARROW_CURSOR;
			case cursor_shape_ibeam		: return GLFW_IBEAM_CURSOR;
			case cursor_shape_crosshair	: return GLFW_CROSSHAIR_CURSOR;
			case cursor_shape_hand		: return GLFW_POINTING_HAND_CURSOR;
			case cursor_shape_ew		: return GLFW_RESIZE_EW_CURSOR;
			case cursor_shape_ns		: return GLFW_RESIZE_NS_CURSOR;
			case cursor_shape_nesw		: return GLFW_RESIZE_NESW_CURSOR;
			case cursor_shape_nwse		: return GLFW_RESIZE_NWSE_CURSOR;
			default						: return GLFW_ARROW_CURSOR;
			}
		})());
	}

	int32_t impl_window_glfw::extension_supported(cstring value)
	{
		return glfwExtensionSupported(value);
	}

	window_handle impl_window_glfw::get_context_current()
	{
		return glfwGetCurrentContext();
	}

	video_mode const & impl_window_glfw::get_desktop_mode()
	{
		static video_mode temp{};
		static ML_scope
		{
#ifdef ML_os_windows
		DEVMODE dm; dm.dmSize = sizeof(dm);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);
		temp.size = vec2i{ (int32_t)dm.dmPelsWidth, (int32_t)dm.dmPelsHeight };
		temp.depth = dm.dmBitsPerPel;
#else
		// do the thing
#endif
		};
		return temp;
	}

	ds::flat_set<video_mode> const & impl_window_glfw::get_fullscreen_modes()
	{
		static ds::flat_set<video_mode> temp{};
		static ML_scope
		{
#ifdef ML_os_windows
		DEVMODE dm; dm.dmSize = sizeof(dm);
		for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &dm); ++count)
		{
			temp.insert({ { (int32_t)dm.dmPelsWidth, (int32_t)dm.dmPelsHeight }, dm.dmBitsPerPel });
		}
#else
		// do the thing
#endif
		};
		return temp;
	}

	void * impl_window_glfw::get_proc_address(cstring value)
	{
		return glfwGetProcAddress(value);
	}

	pmr::vector<window_handle> const & impl_window_glfw::get_monitors()
	{
		static pmr::vector<window_handle> temp{};
		if (temp.empty())
		{
			int32_t count { 0 };
			GLFWmonitor ** m { glfwGetMonitors(&count) };
			for (size_t i = 0, imax = (size_t)count; i < imax; ++i)
			{
				temp.push_back(m[i]);
			}
		}
		return temp;
	}

	window_handle ml::impl_window_glfw::get_primary_monitor()
	{
		return glfwGetPrimaryMonitor();
	}

	float64_t impl_window_glfw::get_time()
	{
		return glfwGetTime();
	}

	bool impl_window_glfw::initialize()
	{
		return glfwInit();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window_char_fn impl_window_glfw::set_char_callback(window_char_fn fn)
	{
		return reinterpret_cast<window_char_fn>(
			glfwSetCharCallback(static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcharfun>(fn)));
	}
	
	window_cursor_enter_fn impl_window_glfw::set_cursor_enter_callback(window_cursor_enter_fn fn)
	{
		return reinterpret_cast<window_cursor_enter_fn>(
			glfwSetCursorEnterCallback(static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcursorenterfun>(fn)));
	}

	window_cursor_pos_fn impl_window_glfw::set_cursor_pos_callback(window_cursor_pos_fn fn)
	{
		return reinterpret_cast<window_cursor_pos_fn>(
			glfwSetCursorPosCallback(static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcursorposfun>(fn)));
	}

	window_error_fn impl_window_glfw::set_error_callback(window_error_fn fn)
	{
		return reinterpret_cast<window_error_fn>(
			glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(fn)));
	}

	window_frame_size_fn impl_window_glfw::set_frame_size_callback(window_frame_size_fn fn)
	{
		return reinterpret_cast<window_frame_size_fn>(
			glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWframebuffersizefun>(fn)));
	}
	
	window_key_fn impl_window_glfw::set_key_callback(window_key_fn fn)
	{
		return reinterpret_cast<window_key_fn>(
			glfwSetKeyCallback(static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWkeyfun>(fn)));
	}
	
	window_mouse_fn impl_window_glfw::set_mouse_callback(window_mouse_fn fn)
	{
		return reinterpret_cast<window_mouse_fn>(
			glfwSetMouseButtonCallback(static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWmousebuttonfun>(fn)));
	}
	
	window_scroll_fn impl_window_glfw::set_scroll_callback(window_scroll_fn fn)
	{
		return reinterpret_cast<window_scroll_fn>(
			glfwSetScrollCallback(static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWscrollfun>(fn)));
	}
	
	window_close_fn impl_window_glfw::set_window_close_callback(window_close_fn fn)
	{
		return reinterpret_cast<window_close_fn>(
			glfwSetWindowCloseCallback(static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowclosefun>(fn)));
	}
	
	window_focus_fn impl_window_glfw::set_window_focus_callback(window_focus_fn fn)
	{
		return reinterpret_cast<window_focus_fn>(
			glfwSetWindowFocusCallback(static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowfocusfun>(fn)));
	}
	
	window_position_fn impl_window_glfw::set_window_pos_callback(window_position_fn fn)
	{
		return reinterpret_cast<window_position_fn>(
			glfwSetWindowPosCallback(static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowposfun>(fn)));
	}
	
	window_size_fn impl_window_glfw::set_window_size_callback(window_size_fn fn)
	{
		return reinterpret_cast<window_size_fn>(
			glfwSetWindowSizeCallback(static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowposfun>(fn)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_PLATFORM_GLFW