#include <libmeme/Platform/Window.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/StringUtility.hpp>
#include <libmeme/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <glfw/glfw3.h>

#ifdef ML_OS_WINDOWS
#	undef APIENTRY
#	include <Windows.h>
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <glfw/glfw3native.h>
#else
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define GLFW_HAS_WINDOW_TOPMOST		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ GLFW_FLOATING
#define GLFW_HAS_WINDOW_HOVERED		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ GLFW_HOVERED
#define GLFW_HAS_WINDOW_ALPHA		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwSetWindowOpacity
#define GLFW_HAS_PER_MONITOR_DPI	(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwGetMonitorContentScale
#define GLFW_HAS_VULKAN				(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ glfwCreateWindowSurface
#define GLFW_HAS_FOCUS_WINDOW		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ glfwFocusWindow
#define GLFW_HAS_FOCUS_ON_SHOW		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ GLFW_FOCUS_ON_SHOW
#define GLFW_HAS_MONITOR_WORK_AREA	(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwGetMonitorWorkarea

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	static GLFWimage const * make_glfw_image(vec2s const & size, byte_t const * pixels)
	{
		auto const & w{ size[0] }, & h{ size[1] };

		if (!w || !h || !pixels) return nullptr;

		static ds::flat_map<size_t, GLFWimage> img{};

		if (size_t const code{ util::hash(pixels, w * h) }; auto const it{ img.find(code) })
		{
			return &(*it->second);
		}
		else
		{
			return &(*img.insert(code,
				GLFWimage{ (int32_t)w, (int32_t)h, (uint8_t *)pixels }
			).second);
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window::window() noexcept
		: m_window	{ nullptr }
		, m_monitor	{ nullptr }
		, m_share	{ nullptr }
		, m_title	{}
		, m_video	{}
		, m_context	{}
		, m_flags	{}
	{
#ifdef ML_OS_WINDOWS
		if (HWND window{ ::GetConsoleWindow() })
		{
			if (HMENU menu{ ::GetSystemMenu(window, false) })
			{
				::EnableMenuItem(menu, SC_CLOSE, MF_GRAYED);
			}
		}
#endif
	}
	
	window::~window() noexcept
	{
#ifdef ML_OS_WINDOWS
		if (HWND window{ ::GetConsoleWindow() })
		{
			if (HMENU menu{ ::GetSystemMenu(window, false) })
			{
				::EnableMenuItem(menu, SC_CLOSE, MF_ENABLED);
			}
		}
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool window::create(pmr::string const & title, video_mode const & video, context_settings const & context, int32_t flags)
	{
		if (m_window)
		{
			return debug::log_error("window is already open");
		}

		if (!glfwInit())
		{
			return debug::log_error("failed initializing glfw");
		}

		m_title		= title;
		m_video		= video;
		m_flags		= flags;
		m_context	= context;
		
		// Client API
		glfwWindowHint(GLFW_CLIENT_API, ([api = m_context.api]() {
			switch (api)
			{
			case client_api::opengl	: return GLFW_OPENGL_API;
			case client_api::vulkan	:
			case client_api::directx:
			default					: return GLFW_NO_API;
			}
		})());

		// API Profile
		glfwWindowHint(GLFW_OPENGL_PROFILE, ([profile = m_context.profile]() {
			switch (profile)
			{
			case client_api::core	: return GLFW_OPENGL_CORE_PROFILE;
			case client_api::compat	: return GLFW_OPENGL_COMPAT_PROFILE;
			case client_api::debug	: return GLFW_OPENGL_DEBUG_CONTEXT;
			case client_api::any	:
			default					: return GLFW_OPENGL_ANY_PROFILE;
			}
		})());

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,	m_context.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,	m_context.minor);
		glfwWindowHint(GLFW_DEPTH_BITS,				m_context.depth_bits);
		glfwWindowHint(GLFW_STENCIL_BITS,			m_context.stencil_bits);
		glfwWindowHint(GLFW_SRGB_CAPABLE,			m_context.sRGB_capable);
		glfwWindowHint(GLFW_RESIZABLE,				m_flags & WindowFlags_Resizable);
		glfwWindowHint(GLFW_VISIBLE,				m_flags & WindowFlags_Visible);
		glfwWindowHint(GLFW_DECORATED,				m_flags & WindowFlags_Decorated);
		glfwWindowHint(GLFW_FOCUSED,				m_flags & WindowFlags_Focused);
		glfwWindowHint(GLFW_AUTO_ICONIFY,			m_flags & WindowFlags_AutoIconify);
		glfwWindowHint(GLFW_FLOATING,				m_flags & WindowFlags_Floating);
		glfwWindowHint(GLFW_MAXIMIZED,				m_flags & WindowFlags_Maximized);
		glfwWindowHint(GLFW_DOUBLEBUFFER,			m_flags & WindowFlags_DoubleBuffered);
		
		// Create Window
		if (!(m_window = static_cast<GLFWwindow *>(glfwCreateWindow(
			m_video.resolution[0],
			m_video.resolution[1],
			m_title.c_str(),
			static_cast<GLFWmonitor *>(m_monitor),
			static_cast<GLFWwindow *>(m_share)
		))))
		{
			return debug::log_error("failed creating glfw window");
		}

		make_context_current();

		set_cursor_mode(cursor::mode::normal);

		if (has_flags(WindowFlags_Fullscreen))
		{
			set_fullscreen(true); // Fullscreen
		}
		else if (has_flags(WindowFlags_Maximized))
		{
			maximize(); // Maximized
		}
		else
		{
			set_centered(); // Centered
		}

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::on_event(event const & value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window & window::close()
	{
		if (m_window)
		{
			glfwSetWindowShouldClose(static_cast<GLFWwindow *>(m_window), true);
		}
		return (*this);
	}

	window & window::destroy()
	{
		if (m_window)
		{
			glfwDestroyWindow(static_cast<GLFWwindow *>(m_window));
		}
		return (*this);
	}

	window & window::iconify()
	{
		if (m_window) 
		{
			glfwIconifyWindow(static_cast<GLFWwindow *>(m_window));
		}
		return (*this);
	}

	window & window::make_context_current()
	{
		if (m_window)
		{
			make_context_current(m_window);
		}
		return (*this);
	}
	
	window & window::maximize()
	{
		if (m_window)
		{
			glfwMaximizeWindow(static_cast<GLFWwindow *>(m_window));

			m_flags |= WindowFlags_Maximized;
		}
		return (*this);
	}

	window & window::restore()
	{
		if (m_window)
		{
			glfwRestoreWindow(static_cast<GLFWwindow *>(m_window));

			m_flags &= ~WindowFlags_Maximized;
		}
		return (*this);
	}

	window & window::swap_buffers()
	{
		if (m_window)
		{
			glfwSwapBuffers(static_cast<GLFWwindow *>(m_window));
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window & window::set_centered()
	{
		return set_position((vec2i)(get_desktop_mode().resolution - get_size()) / 2);
	}

	window & window::set_clipboard(pmr::string const & value)
	{
		if (m_window)
		{
			glfwSetClipboardString(static_cast<GLFWwindow *>(m_window), value.c_str());
		}
		return (*this);
	}

	window & window::set_cursor(void * value)
	{
		if (m_window && value)
		{
			glfwSetCursor(static_cast<GLFWwindow *>(m_window), static_cast<GLFWcursor *>(value));
		}
		return (*this);
	}
	
	window & window::set_cursor_mode(cursor::mode value)
	{
		if (m_window)
		{
			glfwSetInputMode(static_cast<GLFWwindow *>(m_window), GLFW_CURSOR, ([value]()
			{
				switch (value)
				{
				case cursor::mode::hidden	: return GLFW_CURSOR_HIDDEN;
				case cursor::mode::disabled	: return GLFW_CURSOR_DISABLED;
				case cursor::mode::normal	:
				default						: return GLFW_CURSOR_NORMAL;
				}
			})());
		}
		return (*this);
	}

	window & window::set_cursor_pos(vec2d const & value)
	{
		if (m_window)
		{
			glfwSetCursorPos(static_cast<GLFWwindow *>(m_window), value[0], value[1]);
		}
		return (*this);
	}

	window & window::set_fullscreen(bool value)
	{
		return set_monitor(value ? glfwGetPrimaryMonitor() : nullptr);
	}

	window & window::set_icon(size_t w, size_t h, byte_t const * pixels)
	{
		if (m_window)
		{
			glfwSetWindowIcon(static_cast<GLFWwindow *>(m_window),
				1, make_glfw_image({ w, h }, pixels)
			);
		}
		return (*this);
	}

	window & window::set_position(vec2i const & value)
	{
		if (m_window)
		{
			glfwSetWindowPos(static_cast<GLFWwindow *>(m_window), value[0], value[1]);
		}
		return (*this);
	}

	window & window::set_monitor(void * value)
	{
		if (m_window && (m_monitor != value))
		{
			if (m_monitor = value)
			{
				if (auto vm{ glfwGetVideoMode(static_cast<GLFWmonitor *>(m_monitor)) })
				{
					glfwSetWindowMonitor(
						static_cast<GLFWwindow *>(m_window),
						static_cast<GLFWmonitor *>(m_monitor),
						0, 0, vm->width, vm->height,
						GLFW_DONT_CARE
					);
				}
			}
			else
			{
				glfwSetWindowMonitor(
					static_cast<GLFWwindow *>(m_window),
					static_cast<GLFWmonitor *>(m_monitor),
					0, 0, get_width(), get_height(),
					GLFW_DONT_CARE
				);

				if (!(m_flags & WindowFlags_Maximized))
				{
					set_centered();
				}
			}
		}
		return (*this);
	}

	window & window::set_size(vec2i const & value)
	{
		m_video.resolution = value;
		if (m_window)
		{
			glfwSetWindowSize(static_cast<GLFWwindow *>(m_window), get_width(), get_height());
		}
		return (*this);
	}

	window & window::set_title(pmr::string const & value)
	{
		if (m_window && !value.empty())
		{
			glfwSetWindowTitle(static_cast<GLFWwindow *>(m_window), (m_title = value).c_str());
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool window::is_focused() const
	{
		return get_attribute(GLFW_FOCUSED);
	}

	bool window::is_fullscreen() const
	{
		return m_window && m_monitor && (m_monitor == glfwGetPrimaryMonitor());
	}

	bool window::is_open() const
	{
		return m_window && !glfwWindowShouldClose(static_cast<GLFWwindow *>(m_window));
	}
	
	int32_t window::get_attribute(int32_t value) const
	{
		return (m_window ? glfwGetWindowAttrib(static_cast<GLFWwindow *>(m_window), value) : 0);
	}

	cstring window::get_clipboard() const
	{
		return (m_window 
			? glfwGetClipboardString(static_cast<GLFWwindow *>(m_window)) 
			: nullptr
		);
	}

	vec2 window::get_cursor_pos() const
	{
		vec2d temp { 0 };
		if (m_window)
		{
			glfwGetCursorPos(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		}
		return (vec2)temp;
	}

	vec2i window::get_frame_size() const
	{
		vec2i temp { 0 };
		if (m_window)
		{
			glfwGetFramebufferSize(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		}
		return temp;
	}

	void * window::get_handle() const
	{
		return m_window;
	}

	int32_t	window::get_key(int32_t value) const
	{
		return (m_window ? glfwGetKey(static_cast<GLFWwindow *>(m_window), value) : 0);
	}

	int32_t window::get_input_mode(int32_t value) const
	{
		return (m_window ? glfwGetInputMode(static_cast<GLFWwindow *>(m_window), value) : 0);
	}

	int32_t	window::get_mouse_button(int32_t value) const
	{
		return (m_window ? glfwGetMouseButton(static_cast<GLFWwindow *>(m_window), value) : 0);
	}

	vec2i window::get_position() const
	{
		vec2i temp { 0 };
		if (m_window)
		{
			glfwGetWindowPos(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		}
		return temp;
	}

	void * window::get_raw_handle() const
	{
#ifdef ML_OS_WINDOWS
		return glfwGetWin32Window(static_cast<GLFWwindow *>(m_window));
#else
		return m_window;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::destroy_cursor(void * value)
	{
		return glfwDestroyCursor(static_cast<GLFWcursor *>(value));
	}

	void window::make_context_current(void * value)
	{
		return glfwMakeContextCurrent(static_cast<GLFWwindow *>(value));
	}

	void window::poll_events()
	{
		return glfwPollEvents();
	}

	void window::swap_interval(int32_t value)
	{
		return glfwSwapInterval(value);
	}

	void window::terminate()
	{
		return glfwTerminate();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * window::create_custom_cursor(uint32_t w, uint32_t h, byte_t const * pixels)
	{
		return glfwCreateCursor(make_glfw_image({ w, h }, pixels), w, h);
	}

	void * window::create_standard_cursor(cursor::shape value)
	{
		return glfwCreateStandardCursor(([value]()
		{
			switch (value)
			{
			case cursor::shape::arrow		: return GLFW_ARROW_CURSOR;
			case cursor::shape::text_input	: return GLFW_IBEAM_CURSOR;
			case cursor::shape::crosshair	: return GLFW_CROSSHAIR_CURSOR;
			case cursor::shape::hand		: return GLFW_POINTING_HAND_CURSOR;
			case cursor::shape::resize_ew	: return GLFW_RESIZE_EW_CURSOR;
			case cursor::shape::resize_ns	: return GLFW_RESIZE_NS_CURSOR;
			case cursor::shape::resize_nesw	: return GLFW_RESIZE_NESW_CURSOR;
			case cursor::shape::resize_nwse	: return GLFW_RESIZE_NWSE_CURSOR;
			default: return GLFW_ARROW_CURSOR;
			}
		})());
	}

	int32_t window::extension_supported(cstring value)
	{
		return glfwExtensionSupported(value);
	}

	void * window::get_context_current()
	{
		return glfwGetCurrentContext();
	}

	video_mode const & window::get_desktop_mode()
	{
		static video_mode temp {};
		static bool once { true };
		if (once && !(once = false))
		{
#ifdef ML_OS_WINDOWS
			DEVMODE dm; dm.dmSize = sizeof(dm);
			EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);
			temp = video_mode{ vec2i{
				(int32_t)dm.dmPelsWidth,
				(int32_t)dm.dmPelsHeight },
				dm.dmBitsPerPel
			};
#else
			// do the thing
#endif
		}
		return temp;
	}

	pmr::vector<video_mode> const & window::get_fullscreen_modes()
	{
		static pmr::vector<video_mode> temp {};
		static bool once { true };
		if (once && !(once = false))
		{
#ifdef ML_OS_WINDOWS
			DEVMODE dm; dm.dmSize = sizeof(dm);
			for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &dm); ++count)
			{
				auto const vm{ video_mode{ vec2i{
					(int32_t)dm.dmPelsWidth,
					(int32_t)dm.dmPelsHeight },
					dm.dmBitsPerPel
				} };

				if (std::find(temp.begin(), temp.end(), vm) == temp.end())
				{
					temp.push_back(vm);
				}
			}
#else
			// do the thing
#endif
		}
		return temp;
	}

	window::proc_fn window::get_proc_address(cstring value)
	{
		return reinterpret_cast<window::proc_fn>(glfwGetProcAddress(value));
	}

	pmr::vector<void *> const & window::get_monitors()
	{
		static pmr::vector<void *> temp {};
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

	float64_t window::get_time()
	{
		return glfwGetTime();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window::error_fn window::set_error_callback(error_fn value)
	{
		return glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(value))
			? value
			: nullptr;
	}

	window::char_fn window::set_char_callback(char_fn value)
	{
		return m_window
			? glfwSetCharCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcharfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	window::cursor_enter_fn window::set_cursor_enter_callback(cursor_enter_fn value)
	{
		return m_window
			? glfwSetCursorEnterCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcursorenterfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	window::cursor_pos_fn window::set_cursor_pos_callback(cursor_pos_fn value)
	{
		return m_window
			? glfwSetCursorPosCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcursorposfun>(value)) ? value : nullptr
			: nullptr;
	}

	window::frame_size_fn window::set_frame_size_callback(frame_size_fn value)
	{
		return m_window
			? glfwSetFramebufferSizeCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWframebuffersizefun>(value)) ? value : nullptr
			: nullptr;
	}
	
	window::key_fn window::set_key_callback(key_fn value)
	{
		return m_window
			? glfwSetKeyCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWkeyfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	window::mouse_fn window::set_mouse_callback(mouse_fn value)
	{
		return m_window
			? glfwSetMouseButtonCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWmousebuttonfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	window::scroll_fn window::set_scroll_callback(scroll_fn value)
	{
		return m_window
			? glfwSetScrollCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWscrollfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	window::close_fn window::set_window_close_callback(close_fn value)
	{
		return m_window
			? glfwSetWindowCloseCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowclosefun>(value)) ? value : nullptr
			: nullptr;
	}
	
	window::focus_fn window::set_window_focus_callback(focus_fn value)
	{
		return m_window
			? glfwSetWindowFocusCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowfocusfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	window::position_fn window::set_window_pos_callback(position_fn value)
	{
		return m_window
			? glfwSetWindowPosCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowposfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	window::size_fn window::set_window_size_callback(size_fn value)
	{
		return m_window
			? glfwSetWindowSizeCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowposfun>(value)) ? value : nullptr
			: nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}