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
		return &cache.find_or_add(GLFWimage{ (int32_t)w, (int32_t)h, (byte_t *)p });
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	glfw_window::glfw_window() noexcept
		: m_window	{}
		, m_monitor	{}
	{
		static ML_scope{ ML_assert(glfwInit()); };
	}

	glfw_window::glfw_window(window_settings const & ws) noexcept
		: glfw_window{}
	{
		ML_assert(this->open(ws));
	}

	glfw_window::~glfw_window() noexcept
	{
		static ML_defer{ glfwTerminate(); };

		glfwDestroyWindow(m_window);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool glfw_window::open(window_settings const & ws)
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

	void glfw_window::close()
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void glfw_window::iconify()
	{
		glfwIconifyWindow(m_window);
	}
	
	void glfw_window::maximize()
	{
		glfwMaximizeWindow(m_window);
	}

	void glfw_window::restore()
	{
		glfwRestoreWindow(m_window);
	}

	void glfw_window::swap_buffers()
	{
		glfwSwapBuffers(m_window);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool glfw_window::is_fullscreen() const
	{
		return m_monitor && ((monitor_handle)m_monitor == get_primary_monitor());
	}

	bool glfw_window::is_open() const
	{
		return m_window && !glfwWindowShouldClose(m_window);
	}

	int32_t glfw_window::get_attribute(int32_t value) const
	{
		static constexpr int32_t temp[] =
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
		ML_assert(value < ML_arraysize(temp));
		return glfwGetWindowAttrib(m_window, temp[(size_t)value]);
	}

	int_rect glfw_window::get_bounds() const
	{
		int_rect temp{};
		glfwGetWindowFrameSize(m_window, &temp[0], &temp[1], &temp[2], &temp[3]);
		return temp;
	}

	cstring glfw_window::get_clipboard_string() const
	{
		return glfwGetClipboardString(m_window);
	}

	vec2 glfw_window::get_content_scale() const
	{
		vec2 temp{};
		glfwGetWindowContentScale(m_window, &temp[0], &temp[1]);
		return temp;
	}

	vec2 glfw_window::get_cursor_position() const
	{
		vec2d temp{};
		glfwGetCursorPos(m_window, &temp[0], &temp[1]);
		return (vec2)temp;
	}

	vec2i glfw_window::get_framebuffer_size() const
	{
		vec2i temp{};
		glfwGetFramebufferSize(m_window, &temp[0], &temp[1]);
		return temp;
	}

	window_handle glfw_window::get_handle() const
	{
		return (window_handle)m_window;
	}

	int32_t glfw_window::get_input_mode(int32_t mode) const
	{
		return glfwGetInputMode(m_window, mode);
	}

	int32_t	glfw_window::get_key(int32_t key) const
	{
		return glfwGetKey(m_window, key);
	}

	int32_t	glfw_window::get_mouse_button(int32_t button) const
	{
		return glfwGetMouseButton(m_window, button);
	}

	window_handle glfw_window::get_native_handle() const
	{
#ifdef ML_os_windows
		return (window_handle)glfwGetWin32Window(m_window);
#else
		return m_window;
#endif
	}

	float_t glfw_window::get_opacity() const
	{
		return glfwGetWindowOpacity(m_window);
	}

	vec2i glfw_window::get_position() const
	{
		vec2i temp{};
		glfwGetWindowPos(m_window, &temp[0], &temp[1]);
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void glfw_window::set_clipboard_string(cstring value)
	{
		glfwSetClipboardString(m_window, value);
	}

	void glfw_window::set_cursor(cursor_handle value)
	{
		glfwSetCursor(m_window, (GLFWcursor *)value);
	}
	
	void glfw_window::set_cursor_mode(int32_t value)
	{
		static constexpr int32_t temp[] =
		{
			GLFW_CURSOR_NORMAL,
			GLFW_CURSOR_HIDDEN,
			GLFW_CURSOR_DISABLED,
		};
		ML_assert(value < ML_arraysize(temp));
		set_input_mode(GLFW_CURSOR, temp[(size_t)value]);
	}

	void glfw_window::set_cursor_position(vec2d const & value)
	{
		glfwSetCursorPos(m_window, value[0], value[1]);
	}

	void glfw_window::set_fullscreen(bool value)
	{
		set_monitor(value ? get_primary_monitor() : nullptr);
	}

	void glfw_window::set_icon(size_t w, size_t h, byte_t const * p)
	{
		glfwSetWindowIcon(m_window, 1, make_glfw_image(w, h, p));
	}

	void glfw_window::set_input_mode(int32_t mode, int32_t value)
	{
		glfwSetInputMode(m_window, mode, value);
	}

	void glfw_window::set_opacity(float_t value)
	{
		glfwSetWindowOpacity(m_window, value);
	}

	void glfw_window::set_position(vec2i const & value)
	{
		glfwSetWindowPos(m_window, value[0], value[1]);
	}

	void glfw_window::set_monitor(monitor_handle value, int_rect const & bounds)
	{
		m_monitor = (GLFWmonitor *)value;
		if (m_monitor)
		{
			auto const vm{ glfwGetVideoMode(m_monitor) };
			glfwSetWindowMonitor(m_window, m_monitor,
				bounds.left(),
				bounds.top(),
				vm->width,
				vm->height,
				vm->refreshRate);
		}
		else
		{
			glfwSetWindowMonitor(m_window, m_monitor,
				bounds.left(),
				bounds.top(),
				bounds.width(),
				bounds.height(),
				GLFW_DONT_CARE);
		}
	}

	void glfw_window::set_size(vec2i const & value)
	{
		glfwSetWindowSize(m_window, value[0], value[1]);
	}

	void glfw_window::set_title(cstring value)
	{
		glfwSetWindowTitle(m_window, value);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	cursor_handle glfw_window::create_custom_cursor(size_t w, size_t h, byte_t const * p)
	{
		return (cursor_handle)glfwCreateCursor(make_glfw_image(w, h, p), (int32_t)w, (int32_t)h);
	}

	cursor_handle glfw_window::create_standard_cursor(int32_t value)
	{
		static constexpr int32_t temp[] =
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
		ML_assert(value < ML_arraysize(temp));
		return (cursor_handle)glfwCreateStandardCursor(temp[(size_t)value]);
	}

	int32_t glfw_window::extension_supported(cstring value)
	{
		return glfwExtensionSupported(value);
	}

	window_handle glfw_window::get_current_context()
	{
		return (window_handle)glfwGetCurrentContext();
	}

	window_proc glfw_window::get_proc_address(cstring value)
	{
		return (window_proc)glfwGetProcAddress(value);
	}

	pmr::vector<monitor_handle> const & glfw_window::get_monitors()
	{
		static pmr::vector<monitor_handle> temp{};
		static ML_scope // once
		{
			if (int32_t count{}; GLFWmonitor ** monitors{ glfwGetMonitors(&count) })
			{
				temp.reserve((size_t)count);

				for (size_t i = 0, imax = (size_t)count; i < imax; ++i)
				{
					temp.push_back((monitor_handle)monitors[i]);
				}
			}
		};
		return temp;
	}

	monitor_handle glfw_window::get_primary_monitor()
	{
		return (monitor_handle)glfwGetPrimaryMonitor();
	}

	float64_t glfw_window::get_time()
	{
		return glfwGetTime();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void glfw_window::destroy_cursor(cursor_handle value)
	{
		glfwDestroyCursor((GLFWcursor *)value);
	}

	void glfw_window::poll_events()
	{
		glfwPollEvents();
	}

	void glfw_window::set_current_context(window_handle value)
	{
		glfwMakeContextCurrent((GLFWwindow *)value);
	}

	void glfw_window::set_swap_interval(int32_t value)
	{
		glfwSwapInterval(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window_char_fn glfw_window::set_char_callback(window_char_fn fn)
	{
		return reinterpret_cast<window_char_fn>(
			glfwSetCharCallback(m_window,
				reinterpret_cast<GLFWcharfun>(fn)));
	}

	window_char_mods_fn glfw_window::set_char_mods_callback(window_char_mods_fn fn)
	{
		return reinterpret_cast<window_char_mods_fn>(
			glfwSetCharModsCallback(m_window,
				reinterpret_cast<GLFWcharmodsfun>(fn)));
	}

	window_close_fn glfw_window::set_close_callback(window_close_fn fn)
	{
		return reinterpret_cast<window_close_fn>(
			glfwSetWindowCloseCallback(m_window,
				reinterpret_cast<GLFWwindowclosefun>(fn)));
	}

	window_content_scale_fn glfw_window::set_content_scale_callback(window_content_scale_fn fn)
	{
		return reinterpret_cast<window_content_scale_fn>(
			glfwSetWindowContentScaleCallback(m_window,
				reinterpret_cast<GLFWwindowcontentscalefun>(fn)));
	}
	
	window_cursor_enter_fn glfw_window::set_cursor_enter_callback(window_cursor_enter_fn fn)
	{
		return reinterpret_cast<window_cursor_enter_fn>(
			glfwSetCursorEnterCallback(m_window,
				reinterpret_cast<GLFWcursorenterfun>(fn)));
	}

	window_cursor_position_fn glfw_window::set_cursor_position_callback(window_cursor_position_fn fn)
	{
		return reinterpret_cast<window_cursor_position_fn>(
			glfwSetCursorPosCallback(m_window,
				reinterpret_cast<GLFWcursorposfun>(fn)));
	}

	window_drop_fn glfw_window::set_drop_callback(window_drop_fn fn)
	{
		return reinterpret_cast<window_drop_fn>(
			glfwSetDropCallback(m_window,
				reinterpret_cast<GLFWdropfun>(fn)));
	}

	window_error_fn glfw_window::set_error_callback(window_error_fn fn)
	{
		return reinterpret_cast<window_error_fn>(
			glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(fn)));
	}

	window_focus_fn glfw_window::set_focus_callback(window_focus_fn fn)
	{
		return reinterpret_cast<window_focus_fn>(
			glfwSetWindowFocusCallback(m_window,
				reinterpret_cast<GLFWwindowfocusfun>(fn)));
	}

	window_framebuffer_size_fn glfw_window::set_framebuffer_size_callback(window_framebuffer_size_fn fn)
	{
		return reinterpret_cast<window_framebuffer_size_fn>(
			glfwSetFramebufferSizeCallback(m_window,
				reinterpret_cast<GLFWframebuffersizefun>(fn)));
	}

	window_iconify_fn glfw_window::set_iconify_callback(window_iconify_fn fn)
	{
		return reinterpret_cast<window_iconify_fn>(
			glfwSetWindowIconifyCallback(m_window,
				reinterpret_cast<GLFWwindowiconifyfun>(fn)));
	}
	
	window_key_fn glfw_window::set_key_callback(window_key_fn fn)
	{
		return reinterpret_cast<window_key_fn>(
			glfwSetKeyCallback(m_window,
				reinterpret_cast<GLFWkeyfun>(fn)));
	}

	window_maximize_fn glfw_window::set_maximize_callback(window_maximize_fn fn)
	{
		return reinterpret_cast<window_maximize_fn>(
			glfwSetWindowMaximizeCallback(m_window,
				reinterpret_cast<GLFWwindowmaximizefun>(fn)));
	}
	
	window_mouse_fn glfw_window::set_mouse_callback(window_mouse_fn fn)
	{
		return reinterpret_cast<window_mouse_fn>(
			glfwSetMouseButtonCallback(m_window,
				reinterpret_cast<GLFWmousebuttonfun>(fn)));
	}
	
	window_position_fn glfw_window::set_position_callback(window_position_fn fn)
	{
		return reinterpret_cast<window_position_fn>(
			glfwSetWindowPosCallback(m_window,
				reinterpret_cast<GLFWwindowposfun>(fn)));
	}

	window_refresh_fn glfw_window::set_refresh_callback(window_refresh_fn fn)
	{
		return reinterpret_cast<window_refresh_fn>(
			glfwSetWindowRefreshCallback(m_window,
				reinterpret_cast<GLFWwindowrefreshfun>(fn)));
	}

	window_scroll_fn glfw_window::set_scroll_callback(window_scroll_fn fn)
	{
		return reinterpret_cast<window_scroll_fn>(
			glfwSetScrollCallback(m_window,
				reinterpret_cast<GLFWscrollfun>(fn)));
	}
	
	window_size_fn glfw_window::set_size_callback(window_size_fn fn)
	{
		return reinterpret_cast<window_size_fn>(
			glfwSetWindowSizeCallback(m_window,
				reinterpret_cast<GLFWwindowposfun>(fn)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_IMPL_WINDOW_GLFW
