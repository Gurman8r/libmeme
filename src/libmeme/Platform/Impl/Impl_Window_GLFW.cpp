#if defined(ML_IMPL_WINDOW_GLFW)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Impl_Window_Glfw.hpp"
#include <glfw/glfw3.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// platform specific
#if defined(ML_os_windows)
#	undef APIENTRY
#	include <Windows.h>
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <glfw/glfw3native.h>
#elif defined(ML_os_apple)
#elif defined(ML_os_unix)
#else
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool operator<(GLFWimage const & lhs, GLFWimage const & rhs)
{
	return !(std::addressof(lhs) == std::addressof(rhs))
		&& (lhs.width < rhs.width && lhs.height < rhs.height && lhs.pixels < rhs.pixels);
}

bool operator==(GLFWimage const & lhs, GLFWimage const & rhs)
{
	return !(lhs < rhs) && !(rhs < lhs);
}

namespace ml
{
	static GLFWimage const * make_glfw_image(size_t w, size_t h, byte_t const * p) noexcept
	{
		static ds::set<GLFWimage> cache{};
		return &cache.find_or_add({ (int32_t)w, (int32_t)h, (byte_t *)p });
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool impl_window_glfw::open(window_settings const & ws)
	{
		// already open
		if (m_window) { return false; }

		// window hints
		glfwWindowHint(GLFW_CLIENT_API, std::invoke([&]() noexcept
		{
			switch (ws.context.api)
			{
			case window_client_opengl	: return GLFW_OPENGL_API;
			case window_client_vulkan	:
			case window_client_directx	:
			case window_client_unknown	:
			default						: return GLFW_NO_API;
			}
		}));
		glfwWindowHint(GLFW_OPENGL_PROFILE, std::invoke([&]() noexcept
		{
			switch (ws.context.profile)
			{
			case window_profile_core	: return GLFW_OPENGL_CORE_PROFILE;
			case window_profile_compat	: return GLFW_OPENGL_COMPAT_PROFILE;
			case window_profile_debug	: return GLFW_OPENGL_DEBUG_CONTEXT;
			case window_profile_any		:
			default						: return GLFW_OPENGL_ANY_PROFILE;
			}
		}));
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
		glfwWindowHint(GLFW_DOUBLEBUFFER,			ws.hints & window_hints_doublebuffer);
		//glfwWindowHint(GLFW_RED_BITS,				8);
		//glfwWindowHint(GLFW_GREEN_BITS,				8);
		//glfwWindowHint(GLFW_BLUE_BITS,				8);
		//glfwWindowHint(GLFW_ALPHA_BITS,				8);
		//glfwWindowHint(GLFW_REFRESH_RATE,			GLFW_DONT_CARE);
		
		// create window
		return m_window = glfwCreateWindow(
			ws.video.size[0],
			ws.video.size[1],
			ws.title.c_str(),
			nullptr,
			nullptr
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void impl_window_glfw::destroy()
	{
		glfwDestroyWindow(m_window);
		m_window = nullptr;
		m_monitor = nullptr;
	}

	void impl_window_glfw::iconify()
	{
		glfwIconifyWindow(m_window);
	}
	
	void impl_window_glfw::maximize()
	{
		glfwMaximizeWindow(m_window);
	}

	void impl_window_glfw::restore()
	{
		glfwRestoreWindow(m_window);
	}

	void impl_window_glfw::swap_buffers()
	{
		glfwSwapBuffers(m_window);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool impl_window_glfw::is_fullscreen() const
	{
		return m_monitor && (m_monitor == get_primary_monitor());
	}

	bool impl_window_glfw::is_open() const
	{
		return m_window && !glfwWindowShouldClose(m_window);
	}

	int32_t impl_window_glfw::get_attribute(int32_t value) const
	{
		static constexpr int32_t attribs[] =
		{
			GLFW_FOCUSED,
			GLFW_ICONIFIED,
			GLFW_RESIZABLE,
			GLFW_VISIBLE,
			GLFW_DECORATED,
			GLFW_AUTO_ICONIFY,
			GLFW_FLOATING,
			GLFW_MAXIMIZED,
			GLFW_CENTER_CURSOR,
			GLFW_TRANSPARENT_FRAMEBUFFER,
			GLFW_HOVERED,
			GLFW_FOCUS_ON_SHOW,

			GLFW_RED_BITS,
			GLFW_GREEN_BITS,
			GLFW_BLUE_BITS,
			GLFW_ALPHA_BITS,
			GLFW_DEPTH_BITS,
			GLFW_STENCIL_BITS,
			GLFW_ACCUM_RED_BITS,
			GLFW_ACCUM_GREEN_BITS,
			GLFW_ACCUM_BLUE_BITS,
			GLFW_ACCUM_ALPHA_BITS,
			GLFW_AUX_BUFFERS,
			GLFW_STEREO,
			GLFW_SAMPLES,
			GLFW_SRGB_CAPABLE,
			GLFW_REFRESH_RATE,
			GLFW_DOUBLEBUFFER,

			GLFW_CLIENT_API,
			GLFW_CONTEXT_VERSION_MAJOR,
			GLFW_CONTEXT_VERSION_MINOR,
			GLFW_CONTEXT_REVISION,
			GLFW_CONTEXT_ROBUSTNESS,
			GLFW_OPENGL_FORWARD_COMPAT,
			GLFW_OPENGL_DEBUG_CONTEXT,
			GLFW_OPENGL_PROFILE,
			GLFW_CONTEXT_RELEASE_BEHAVIOR,
			GLFW_CONTEXT_NO_ERROR,
			GLFW_CONTEXT_CREATION_API,
			GLFW_SCALE_TO_MONITOR,
		};
		return glfwGetWindowAttrib(m_window, attribs[static_cast<size_t>(value)]);
	}

	int_rect impl_window_glfw::get_bounds() const
	{
		int_rect temp{};
		glfwGetWindowFrameSize(m_window, &temp[0], &temp[1], &temp[2], &temp[3]);
		return temp;
	}

	cstring impl_window_glfw::get_clipboard_string() const
	{
		return glfwGetClipboardString(m_window);
	}

	vec2 impl_window_glfw::get_content_scale() const
	{
		vec2 temp{};
		glfwGetWindowContentScale(m_window, &temp[0], &temp[1]);
		return temp;
	}

	vec2 impl_window_glfw::get_cursor_position() const
	{
		vec2d temp{};
		glfwGetCursorPos(m_window, &temp[0], &temp[1]);
		return (vec2)temp;
	}

	vec2i impl_window_glfw::get_framebuffer_size() const
	{
		vec2i temp{};
		glfwGetFramebufferSize(m_window, &temp[0], &temp[1]);
		return temp;
	}

	window_handle impl_window_glfw::get_handle() const
	{
		return m_window;
	}

	int32_t impl_window_glfw::get_input_mode(int32_t mode) const
	{
		return glfwGetInputMode(m_window, mode);
	}

	int32_t	impl_window_glfw::get_key(int32_t key) const
	{
		return glfwGetKey(m_window, key);
	}

	int32_t	impl_window_glfw::get_mouse_button(int32_t button) const
	{
		return glfwGetMouseButton(m_window, button);
	}

	window_handle impl_window_glfw::get_native_handle() const
	{
#ifdef ML_os_windows
		return glfwGetWin32Window(m_window);
#else
		return m_window;
#endif
	}

	float_t impl_window_glfw::get_opacity() const
	{
		return glfwGetWindowOpacity(m_window);
	}

	vec2i impl_window_glfw::get_position() const
	{
		vec2i temp{};
		glfwGetWindowPos(m_window, &temp[0], &temp[1]);
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void impl_window_glfw::set_clipboard_string(cstring value)
	{
		glfwSetClipboardString(m_window, value);
	}

	void impl_window_glfw::set_cursor(cursor_handle value)
	{
		glfwSetCursor(m_window, static_cast<GLFWcursor *>(value));
	}
	
	void impl_window_glfw::set_cursor_mode(int32_t value)
	{
		set_input_mode(GLFW_CURSOR, std::invoke([&]() noexcept
		{
			switch (value)
			{
			case cursor_mode_hidden		: return GLFW_CURSOR_HIDDEN;
			case cursor_mode_disabled	: return GLFW_CURSOR_DISABLED;
			case cursor_mode_normal		:
			default						: return GLFW_CURSOR_NORMAL;
			}
		}));
	}

	void impl_window_glfw::set_cursor_position(vec2d const & value)
	{
		glfwSetCursorPos(m_window, value[0], value[1]);
	}

	void impl_window_glfw::set_fullscreen(bool value)
	{
		set_monitor(value ? get_primary_monitor() : nullptr);
	}

	void impl_window_glfw::set_icon(size_t w, size_t h, byte_t const * p)
	{
		glfwSetWindowIcon(m_window, 1, make_glfw_image(w, h, p));
	}

	void impl_window_glfw::set_input_mode(int32_t mode, int32_t value)
	{
		glfwSetInputMode(m_window, mode, value);
	}

	void impl_window_glfw::set_opacity(float_t value)
	{
		glfwSetWindowOpacity(m_window, value);
	}

	void impl_window_glfw::set_position(vec2i const & value)
	{
		glfwSetWindowPos(m_window, value[0], value[1]);
	}

	void impl_window_glfw::set_monitor(monitor_handle value, int_rect const & bounds)
	{
		if (m_monitor = (GLFWmonitor *)value)
		{
			if (auto const vm{ glfwGetVideoMode(m_monitor) })
			{
				glfwSetWindowMonitor(m_window, m_monitor,
					bounds.left(),
					bounds.top(),
					vm->width,
					vm->height,
					vm->refreshRate);
			}
		}
		else
		{
			glfwSetWindowMonitor(m_window, nullptr,
				bounds.left(),
				bounds.top(),
				bounds.width(),
				bounds.height(),
				GLFW_DONT_CARE);
		}
	}

	void impl_window_glfw::set_should_close(bool value)
	{
		glfwSetWindowShouldClose(m_window, value);
	}

	void impl_window_glfw::set_size(vec2i const & value)
	{
		glfwSetWindowSize(m_window, value[0], value[1]);
	}

	void impl_window_glfw::set_title(cstring value)
	{
		glfwSetWindowTitle(m_window, value);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	cursor_handle impl_window_glfw::create_custom_cursor(size_t w, size_t h, byte_t const * p)
	{
		return glfwCreateCursor(make_glfw_image(w, h, p), (int32_t)w, (int32_t)h);
	}

	cursor_handle impl_window_glfw::create_standard_cursor(int32_t value)
	{
		static constexpr int32_t shapes[] =
		{
			GLFW_ARROW_CURSOR,
			GLFW_IBEAM_CURSOR,
			GLFW_CROSSHAIR_CURSOR,
			GLFW_POINTING_HAND_CURSOR,
			GLFW_RESIZE_EW_CURSOR,
			GLFW_RESIZE_NS_CURSOR,
			GLFW_RESIZE_NESW_CURSOR,
			GLFW_RESIZE_NWSE_CURSOR,
			GLFW_ARROW_CURSOR,
		};
		return glfwCreateStandardCursor(shapes[static_cast<size_t>(value)]);
	}

	int32_t impl_window_glfw::extension_supported(cstring value)
	{
		return glfwExtensionSupported(value);
	}

	window_handle impl_window_glfw::get_current_context()
	{
		return glfwGetCurrentContext();
	}

	void * impl_window_glfw::get_proc_address(cstring value)
	{
		return glfwGetProcAddress(value);
	}

	pmr::vector<monitor_handle> const & impl_window_glfw::get_monitors()
	{
		static pmr::vector<monitor_handle> temp{};
		static ML_scope // once
		{
			int32_t count{};
			GLFWmonitor ** monitors{ glfwGetMonitors(&count) };
			temp.reserve((size_t)count);
			for (size_t i = 0, imax = (size_t)count; i < imax; ++i)
			{
				temp.push_back(monitors[i]);
			}
		};
		return temp;
	}

	monitor_handle impl_window_glfw::get_primary_monitor()
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

	void impl_window_glfw::destroy_cursor(cursor_handle value)
	{
		glfwDestroyCursor(static_cast<GLFWcursor *>(value));
	}

	void impl_window_glfw::finalize()
	{
		glfwTerminate();
	}

	void impl_window_glfw::poll_events()
	{
		glfwPollEvents();
	}

	void impl_window_glfw::set_current_context(window_handle value)
	{
		glfwMakeContextCurrent(static_cast<GLFWwindow *>(value));
	}

	void impl_window_glfw::set_swap_interval(int32_t value)
	{
		glfwSwapInterval(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window_char_fn impl_window_glfw::set_char_callback(window_char_fn fn)
	{
		return reinterpret_cast<window_char_fn>(
			glfwSetCharCallback(m_window,
				reinterpret_cast<GLFWcharfun>(fn)));
	}

	window_char_mods_fn impl_window_glfw::set_char_mods_callback(window_char_mods_fn fn)
	{
		return reinterpret_cast<window_char_mods_fn>(
			glfwSetCharModsCallback(m_window,
				reinterpret_cast<GLFWcharmodsfun>(fn)));
	}

	window_close_fn impl_window_glfw::set_close_callback(window_close_fn fn)
	{
		return reinterpret_cast<window_close_fn>(
			glfwSetWindowCloseCallback(m_window,
				reinterpret_cast<GLFWwindowclosefun>(fn)));
	}

	window_content_scale_fn impl_window_glfw::set_content_scale_callback(window_content_scale_fn fn)
	{
		return reinterpret_cast<window_content_scale_fn>(
			glfwSetWindowContentScaleCallback(m_window,
				reinterpret_cast<GLFWwindowcontentscalefun>(fn)));
	}
	
	window_cursor_enter_fn impl_window_glfw::set_cursor_enter_callback(window_cursor_enter_fn fn)
	{
		return reinterpret_cast<window_cursor_enter_fn>(
			glfwSetCursorEnterCallback(m_window,
				reinterpret_cast<GLFWcursorenterfun>(fn)));
	}

	window_cursor_position_fn impl_window_glfw::set_cursor_position_callback(window_cursor_position_fn fn)
	{
		return reinterpret_cast<window_cursor_position_fn>(
			glfwSetCursorPosCallback(m_window,
				reinterpret_cast<GLFWcursorposfun>(fn)));
	}

	window_drop_fn impl_window_glfw::set_drop_callback(window_drop_fn fn)
	{
		return reinterpret_cast<window_drop_fn>(
			glfwSetDropCallback(m_window,
				reinterpret_cast<GLFWdropfun>(fn)));
	}

	window_error_fn impl_window_glfw::set_error_callback(window_error_fn fn)
	{
		return reinterpret_cast<window_error_fn>(
			glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(fn)));
	}

	window_focus_fn impl_window_glfw::set_focus_callback(window_focus_fn fn)
	{
		return reinterpret_cast<window_focus_fn>(
			glfwSetWindowFocusCallback(m_window,
				reinterpret_cast<GLFWwindowfocusfun>(fn)));
	}

	window_framebuffer_size_fn impl_window_glfw::set_framebuffer_size_callback(window_framebuffer_size_fn fn)
	{
		return reinterpret_cast<window_framebuffer_size_fn>(
			glfwSetFramebufferSizeCallback(m_window,
				reinterpret_cast<GLFWframebuffersizefun>(fn)));
	}

	window_iconify_fn impl_window_glfw::set_iconify_callback(window_iconify_fn fn)
	{
		return reinterpret_cast<window_iconify_fn>(
			glfwSetWindowIconifyCallback(m_window,
				reinterpret_cast<GLFWwindowiconifyfun>(fn)));
	}
	
	window_key_fn impl_window_glfw::set_key_callback(window_key_fn fn)
	{
		return reinterpret_cast<window_key_fn>(
			glfwSetKeyCallback(m_window,
				reinterpret_cast<GLFWkeyfun>(fn)));
	}

	window_maximize_fn impl_window_glfw::set_maximize_callback(window_maximize_fn fn)
	{
		return reinterpret_cast<window_maximize_fn>(
			glfwSetWindowMaximizeCallback(m_window,
				reinterpret_cast<GLFWwindowmaximizefun>(fn)));
	}
	
	window_mouse_fn impl_window_glfw::set_mouse_callback(window_mouse_fn fn)
	{
		return reinterpret_cast<window_mouse_fn>(
			glfwSetMouseButtonCallback(m_window,
				reinterpret_cast<GLFWmousebuttonfun>(fn)));
	}
	
	window_position_fn impl_window_glfw::set_position_callback(window_position_fn fn)
	{
		return reinterpret_cast<window_position_fn>(
			glfwSetWindowPosCallback(m_window,
				reinterpret_cast<GLFWwindowposfun>(fn)));
	}

	window_refresh_fn impl_window_glfw::set_refresh_callback(window_refresh_fn fn)
	{
		return reinterpret_cast<window_refresh_fn>(
			glfwSetWindowRefreshCallback(m_window,
				reinterpret_cast<GLFWwindowrefreshfun>(fn)));
	}

	window_scroll_fn impl_window_glfw::set_scroll_callback(window_scroll_fn fn)
	{
		return reinterpret_cast<window_scroll_fn>(
			glfwSetScrollCallback(m_window,
				reinterpret_cast<GLFWscrollfun>(fn)));
	}
	
	window_size_fn impl_window_glfw::set_size_callback(window_size_fn fn)
	{
		return reinterpret_cast<window_size_fn>(
			glfwSetWindowSizeCallback(m_window,
				reinterpret_cast<GLFWwindowposfun>(fn)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_IMPL_WINDOW_GLFW
