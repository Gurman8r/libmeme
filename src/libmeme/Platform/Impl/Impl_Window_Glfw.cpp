#ifdef ML_PLATFORM_GLFW

#include "Impl_Window_Glfw.hpp"
#include <glfw/glfw3.h>

#ifdef ML_os_windows
#	undef APIENTRY
#	include <Windows.h>
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <glfw/glfw3native.h>
#else
#endif

bool operator<(GLFWimage const & lhs, GLFWimage const & rhs) noexcept
{
	return (lhs.width < rhs.width) && (lhs.height < rhs.height) && (lhs.pixels < rhs.pixels);
}

bool operator==(GLFWimage const & lhs, GLFWimage const & rhs) noexcept
{
	return !(lhs < rhs) && !(rhs < lhs);
}

namespace ml
{
	static GLFWimage const * make_glfw_image(size_t w, size_t h, byte_t const * p) noexcept
	{
		static ds::set<GLFWimage> cache{};
		GLFWimage const temp{ (int32_t)w, (int32_t)h, (byte_t *)p };
		if (auto const it{ cache.find(temp) }; it != cache.end())
		{
			return &(*it);
		}
		else
		{
			return &(*cache.insert(temp).first);
		}
	}
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool impl_window_glfw::open(window_settings const & ws)
	{
		// already open
		if (m_window) { return false; }

		// window hints
		glfwWindowHint(GLFW_CLIENT_API, ([&]() noexcept
		{
			switch (ws.context.api)
			{
			case window_api_opengl	: return GLFW_OPENGL_API;
			case window_api_vulkan	:
			case window_api_directx:
			case window_api_unknown:
			default						: return GLFW_NO_API;
			}
		})());
		glfwWindowHint(GLFW_OPENGL_PROFILE, ([&]() noexcept
		{
			switch (ws.context.profile)
			{
			case window_profile_core	: return GLFW_OPENGL_CORE_PROFILE;
			case window_profile_compat	: return GLFW_OPENGL_COMPAT_PROFILE;
			case window_profile_debug	: return GLFW_OPENGL_DEBUG_CONTEXT;
			case window_profile_any		:
			default						: return GLFW_OPENGL_ANY_PROFILE;
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
		glfwWindowHint(GLFW_DOUBLEBUFFER,			ws.hints & window_hints_doublebuffer);
		
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
		return (m_monitor == get_primary_monitor());
	}

	bool impl_window_glfw::is_open() const
	{
		return m_window && !glfwWindowShouldClose(m_window);
	}

	int32_t impl_window_glfw::get_attribute(int32_t value) const
	{
		return glfwGetWindowAttrib(m_window, ([&]() noexcept
		{
			switch (value)
			{
			case window_attr_focused					: return GLFW_FOCUSED;
			case window_attr_iconified					: return GLFW_ICONIFIED;
			case window_attr_resizable					: return GLFW_RESIZABLE;
			case window_attr_visible					: return GLFW_VISIBLE;
			case window_attr_decorated					: return GLFW_DECORATED;
			case window_attr_auto_iconify				: return GLFW_AUTO_ICONIFY;
			case window_attr_floating					: return GLFW_FLOATING;
			case window_attr_maximized					: return GLFW_MAXIMIZED;
			case window_attr_center_cursor				: return GLFW_CENTER_CURSOR;
			case window_attr_transparent_framebuffer	: return GLFW_TRANSPARENT_FRAMEBUFFER;
			case window_attr_hovered					: return GLFW_HOVERED;
			case window_attr_focus_on_show				: return GLFW_FOCUS_ON_SHOW;

			case window_attr_red_bits					: return GLFW_RED_BITS;
			case window_attr_green_bits					: return GLFW_GREEN_BITS;
			case window_attr_blue_bits					: return GLFW_BLUE_BITS;
			case window_attr_alpha_bits					: return GLFW_ALPHA_BITS;
			case window_attr_depth_bits					: return GLFW_DEPTH_BITS;
			case window_attr_stencil_bits				: return GLFW_STENCIL_BITS;
			case window_attr_accum_red_bits				: return GLFW_ACCUM_RED_BITS;
			case window_attr_accum_green_bits			: return GLFW_ACCUM_GREEN_BITS;
			case window_attr_accum_blue_bits			: return GLFW_ACCUM_BLUE_BITS;
			case window_attr_accum_alpha_bits			: return GLFW_ACCUM_ALPHA_BITS;
			case window_attr_aux_buffers				: return GLFW_AUX_BUFFERS;
			case window_attr_stereo						: return GLFW_STEREO;
			case window_attr_samples					: return GLFW_SAMPLES;
			case window_attr_srgb_capable				: return GLFW_SRGB_CAPABLE;
			case window_attr_refresh_rate				: return GLFW_REFRESH_RATE;
			case window_attr_doublebuffer				: return GLFW_DOUBLEBUFFER;
			
			case window_attr_client_api					: return GLFW_CLIENT_API;
			case window_attr_context_version_major		: return GLFW_CONTEXT_VERSION_MAJOR;
			case window_attr_context_version_minor		: return GLFW_CONTEXT_VERSION_MINOR;
			case window_attr_context_revision			: return GLFW_CONTEXT_REVISION;
			case window_attr_context_robustness			: return GLFW_CONTEXT_ROBUSTNESS;
			case window_attr_backend_forward_compat		: return GLFW_OPENGL_FORWARD_COMPAT;
			case window_attr_backend_debug_context		: return GLFW_OPENGL_DEBUG_CONTEXT;
			case window_attr_backend_profile			: return GLFW_OPENGL_PROFILE;
			case window_attr_context_release_behavior	: return GLFW_CONTEXT_RELEASE_BEHAVIOR;
			case window_attr_context_no_error			: return GLFW_CONTEXT_NO_ERROR;
			case window_attr_context_creation_api		: return GLFW_CONTEXT_CREATION_API;
			case window_attr_scale_to_monitor			: return GLFW_SCALE_TO_MONITOR;
			}
			return value;
		})());
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

	int_rect impl_window_glfw::get_frame_size() const
	{
		int_rect temp{};
		glfwGetWindowFrameSize(m_window, &temp[0], &temp[1], &temp[2], &temp[3]);
		return temp;
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
		set_input_mode(GLFW_CURSOR, ([&]() noexcept
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
		return glfwCreateCursor(make_glfw_image(w, h, p), w, h);
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

	window_handle impl_window_glfw::get_current_context()
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

	pmr::vector<video_mode> const & impl_window_glfw::get_fullscreen_modes()
	{
		static pmr::vector<video_mode> temp{};
		auto add_vm = [&](video_mode && vm) noexcept
		{
			if (std::find(temp.begin(), temp.end(), ML_forward(vm)) == temp.end())
			{
				temp.emplace_back(ML_forward(vm));
			}
		};
		static ML_scope
		{
#ifdef ML_os_windows
		DEVMODE dm; dm.dmSize = sizeof(dm);
		for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &dm); ++count)
		{
			add_vm({ { (int32_t)dm.dmPelsWidth, (int32_t)dm.dmPelsHeight }, dm.dmBitsPerPel });
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

	pmr::vector<monitor_handle> const & impl_window_glfw::get_monitors()
	{
		static pmr::vector<monitor_handle> temp{};
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

	window_error_fn impl_window_glfw::set_error_callback(window_error_fn fn)
	{
		return reinterpret_cast<window_error_fn>(
			glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(fn)));
	}

	window_framebuffer_size_fn impl_window_glfw::set_framebuffer_size_callback(window_framebuffer_size_fn fn)
	{
		return reinterpret_cast<window_framebuffer_size_fn>(
			glfwSetFramebufferSizeCallback(m_window,
				reinterpret_cast<GLFWframebuffersizefun>(fn)));
	}
	
	window_key_fn impl_window_glfw::set_key_callback(window_key_fn fn)
	{
		return reinterpret_cast<window_key_fn>(
			glfwSetKeyCallback(m_window,
				reinterpret_cast<GLFWkeyfun>(fn)));
	}
	
	window_mouse_fn impl_window_glfw::set_mouse_callback(window_mouse_fn fn)
	{
		return reinterpret_cast<window_mouse_fn>(
			glfwSetMouseButtonCallback(m_window,
				reinterpret_cast<GLFWmousebuttonfun>(fn)));
	}
	
	window_scroll_fn impl_window_glfw::set_scroll_callback(window_scroll_fn fn)
	{
		return reinterpret_cast<window_scroll_fn>(
			glfwSetScrollCallback(m_window,
				reinterpret_cast<GLFWscrollfun>(fn)));
	}
	
	window_close_fn impl_window_glfw::set_window_close_callback(window_close_fn fn)
	{
		return reinterpret_cast<window_close_fn>(
			glfwSetWindowCloseCallback(m_window,
				reinterpret_cast<GLFWwindowclosefun>(fn)));
	}
	
	window_focus_fn impl_window_glfw::set_window_focus_callback(window_focus_fn fn)
	{
		return reinterpret_cast<window_focus_fn>(
			glfwSetWindowFocusCallback(m_window,
				reinterpret_cast<GLFWwindowfocusfun>(fn)));
	}
	
	window_position_fn impl_window_glfw::set_window_position_callback(window_position_fn fn)
	{
		return reinterpret_cast<window_position_fn>(
			glfwSetWindowPosCallback(m_window,
				reinterpret_cast<GLFWwindowposfun>(fn)));
	}
	
	window_size_fn impl_window_glfw::set_window_size_callback(window_size_fn fn)
	{
		return reinterpret_cast<window_size_fn>(
			glfwSetWindowSizeCallback(m_window,
				reinterpret_cast<GLFWwindowposfun>(fn)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_PLATFORM_GLFW