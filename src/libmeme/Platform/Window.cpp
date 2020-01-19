#include <libmeme/Platform/Window.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/StringUtility.hpp>
#include <libmeme/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#include <glfw/glfw3.h>

#ifdef ML_OS_WINDOWS
#	undef APIENTRY
#	include <Windows.h>
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <glfw/glfw3native.h>
#else
#endif

/* * * * * * * * * * * * * * * * * * * * */

#define GLFW_HAS_WINDOW_TOPMOST		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ GLFW_FLOATING
#define GLFW_HAS_WINDOW_HOVERED		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ GLFW_HOVERED
#define GLFW_HAS_WINDOW_ALPHA		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwSetWindowOpacity
#define GLFW_HAS_PER_MONITOR_DPI	(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwGetMonitorContentScale
#define GLFW_HAS_VULKAN				(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ glfwCreateWindowSurface
#define GLFW_HAS_FOCUS_WINDOW		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ glfwFocusWindow
#define GLFW_HAS_FOCUS_ON_SHOW		(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ GLFW_FOCUS_ON_SHOW
#define GLFW_HAS_MONITOR_WORK_AREA	(GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwGetMonitorWorkarea

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static GLFWimage const * make_glfw_image(size_t w, size_t h, byte_t const * pixels)
	{
		static ds::flat_map<void const *, GLFWimage> cache{};
		if (w && h && pixels)
		{
			if (auto const it{ cache.find(pixels) })
			{
				return &(*it->second);
			}
			return &(*cache.insert(
				pixels, GLFWimage{ (int32_t)w, (int32_t)h, (uint8_t *)pixels }
			).first.second);
		}
		return nullptr;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::Window() noexcept
		: m_window	{ nullptr }
		, m_monitor	{ nullptr }
		, m_share	{ nullptr }
		, m_title	{}
		, m_video	{}
		, m_context	{}
		, m_flags	{}
	{
#if defined(ML_OS_WINDOWS)
		if (HWND window{ ::GetConsoleWindow() })
		{
			if (HMENU menu{ ::GetSystemMenu(window, false) })
			{
				::EnableMenuItem(menu, SC_CLOSE, MF_GRAYED);
			}
		}
#endif
	}
	
	Window::~Window() noexcept
	{
#if defined(ML_OS_WINDOWS)
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

	bool Window::create(pmr::string const & title, VideoMode const & video, ContextSettings const & context, int32_t flags)
	{
#ifdef ML_IMPL_RENDERER_OPENGL
		ML_ASSERT(context.api == Client_API::OpenGL);
#endif
		if (m_window)
		{
			return debug::log_error("Window already initialized");
		}

		if (!glfwInit())
		{
			return debug::log_error("Failed initializing GLFW");
		}

		m_title		= title;
		m_video		= video;
		m_flags		= flags;
		m_context	= context;
		
		// API
		glfwWindowHint(GLFW_CLIENT_API, ([api = m_context.api]() {
			switch (api)
			{
			case Client_API::OpenGL	: return GLFW_OPENGL_API;
			case Client_API::Vulkan	:
			case Client_API::DirectX:
			default					: return GLFW_NO_API;
			}
		})());

		// Profile
		glfwWindowHint(GLFW_OPENGL_PROFILE, ([profile = m_context.profile]() {
			switch (profile)
			{
			case Client_API::Core	: return GLFW_OPENGL_CORE_PROFILE;
			case Client_API::Compat	: return GLFW_OPENGL_COMPAT_PROFILE;
			case Client_API::Debug	: return GLFW_OPENGL_DEBUG_CONTEXT;
			case Client_API::Any	:
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
		if (m_window = static_cast<GLFWwindow *>(glfwCreateWindow(
			this->get_width(),
			this->get_height(),
			this->get_title().c_str(),
			static_cast<GLFWmonitor *>(m_monitor = nullptr),
			static_cast<GLFWwindow *>(m_share = nullptr)
		)))
		{
			this->make_context_current();

			this->set_cursor_mode(Cursor::Mode::Normal);

			if (m_flags & WindowFlags_Fullscreen)
			{
				this->set_fullscreen(true); // Fullscreen
			}
			else if (m_flags & WindowFlags_Maximized)
			{
				this->maximize(); // Maximized
			}
			else
			{
				this->set_centered(); // Centered
			}

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window & Window::close()
	{
		if (m_window)
		{
			glfwSetWindowShouldClose(static_cast<GLFWwindow *>(m_window), true);
		}
		return (*this);
	}

	Window & Window::destroy()
	{
		if (m_window)
		{
			glfwDestroyWindow(static_cast<GLFWwindow *>(m_window));
		}
		return (*this);
	}

	Window & Window::iconify()
	{
		if (m_window) 
		{
			glfwIconifyWindow(static_cast<GLFWwindow *>(m_window));
		}
		return (*this);
	}

	Window & Window::make_context_current()
	{
		if (m_window)
		{
			make_context_current(m_window);
		}
		return (*this);
	}
	
	Window & Window::maximize()
	{
		if (m_window)
		{
			glfwMaximizeWindow(static_cast<GLFWwindow *>(m_window));

			m_flags |= WindowFlags_Maximized;
		}
		return (*this);
	}

	Window & Window::restore()
	{
		if (m_window)
		{
			glfwRestoreWindow(static_cast<GLFWwindow *>(m_window));

			m_flags &= ~WindowFlags_Maximized;
		}
		return (*this);
	}

	Window & Window::swap_buffers()
	{
		if (m_window)
		{
			glfwSwapBuffers(static_cast<GLFWwindow *>(m_window));
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window & Window::set_centered()
	{
		return set_position((vec2i)(get_desktop_mode().resolution - this->get_size()) / 2);
	}

	Window & Window::set_clipboard(pmr::string const & value)
	{
		if (m_window)
		{
			glfwSetClipboardString(static_cast<GLFWwindow *>(m_window), value.c_str());
		}
		return (*this);
	}

	Window & Window::set_cursor(void * value)
	{
		if (m_window && value)
		{
			glfwSetCursor(static_cast<GLFWwindow *>(m_window), static_cast<GLFWcursor *>(value));
		}
		return (*this);
	}
	
	Window & Window::set_cursor_mode(Cursor::Mode value)
	{
		if (m_window)
		{
			glfwSetInputMode(static_cast<GLFWwindow *>(m_window), GLFW_CURSOR, ([value]()
			{
				switch (value)
				{
				case Cursor::Mode::Normal: return GLFW_CURSOR_NORMAL;
				case Cursor::Mode::Hidden: return GLFW_CURSOR_HIDDEN;
				case Cursor::Mode::Disabled: return GLFW_CURSOR_DISABLED;
				default: return GLFW_CURSOR_NORMAL;
				}
			})());
		}
		return (*this);
	}

	Window & Window::set_cursor_pos(vec2i const & value)
	{
		if (m_window)
		{
			glfwSetCursorPos(static_cast<GLFWwindow *>(m_window), value[0], value[1]);
		}
		return (*this);
	}

	Window & Window::set_fullscreen(bool value)
	{
		return set_monitor(value ? glfwGetPrimaryMonitor() : nullptr);
	}

	Window & Window::set_icon(size_t w, size_t h, byte_t const * pixels)
	{
		if (m_window)
		{
			glfwSetWindowIcon(static_cast<GLFWwindow *>(m_window),
				1, make_glfw_image(w, h, pixels)
			);
		}
		return (*this);
	}

	Window & Window::set_position(vec2i const & value)
	{
		if (m_window)
		{
			glfwSetWindowPos(static_cast<GLFWwindow *>(m_window), value[0], value[1]);
		}
		return (*this);
	}

	Window & Window::set_monitor(void * value)
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

	Window & Window::set_size(vec2u const & value)
	{
		m_video.resolution = value;
		if (m_window)
		{
			glfwSetWindowSize(static_cast<GLFWwindow *>(m_window), get_width(), get_height());
		}
		return (*this);
	}

	Window & Window::set_title(pmr::string const & value)
	{
		m_title = value;
		if (m_window)
		{
			glfwSetWindowTitle(static_cast<GLFWwindow *>(m_window), m_title.c_str());
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Window::is_focused() const
	{
		return get_attribute(GLFW_FOCUSED);
	}

	bool Window::is_fullscreen() const
	{
		return m_window && m_monitor && (m_monitor == glfwGetPrimaryMonitor());
	}

	bool Window::is_open() const
	{
		return m_window && !glfwWindowShouldClose(static_cast<GLFWwindow *>(m_window));
	}
	
	int32_t Window::get_attribute(int32_t value) const
	{
		return (m_window ? glfwGetWindowAttrib(static_cast<GLFWwindow *>(m_window), value) : 0);
	}

	C_string Window::get_clipboard() const
	{
		return (m_window 
			? glfwGetClipboardString(static_cast<GLFWwindow *>(m_window)) 
			: nullptr
		);
	}

	vec2 Window::get_cursor_pos() const
	{
		vec2d temp { 0 };
		if (m_window)
		{
			glfwGetCursorPos(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		}
		return (vec2)temp;
	}

	vec2i Window::get_frame_size() const
	{
		vec2i temp { 0 };
		if (m_window)
		{
			glfwGetFramebufferSize(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		}
		return temp;
	}

	void * Window::get_handle() const
	{
		return m_window;
	}

	int32_t	Window::get_key(int32_t value) const
	{
		return (m_window ? glfwGetKey(static_cast<GLFWwindow *>(m_window), value) : 0);
	}

	int32_t Window::get_input_mode(int32_t value) const
	{
		return (m_window ? glfwGetInputMode(static_cast<GLFWwindow *>(m_window), value) : 0);
	}

	int32_t	Window::get_mouse_button(int32_t value) const
	{
		return (m_window ? glfwGetMouseButton(static_cast<GLFWwindow *>(m_window), value) : 0);
	}

	vec2i Window::get_position() const
	{
		vec2i temp { 0 };
		if (m_window)
		{
			glfwGetWindowPos(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		}
		return temp;
	}

	void * Window::get_raw_handle() const
	{
#ifdef ML_OS_WINDOWS
		return glfwGetWin32Window(static_cast<GLFWwindow *>(m_window));
#else
		return m_window;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Window::destroy_cursor(void * value)
	{
		return glfwDestroyCursor(static_cast<GLFWcursor *>(value));
	}

	void Window::make_context_current(void * value)
	{
		return glfwMakeContextCurrent(static_cast<GLFWwindow *>(value));
	}

	void Window::poll_events()
	{
		return glfwPollEvents();
	}

	void Window::swap_interval(int32_t value)
	{
		return glfwSwapInterval(value);
	}

	void Window::terminate()
	{
		return glfwTerminate();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * Window::create_custom_cursor(uint32_t w, uint32_t h, byte_t const * pixels)
	{
		return glfwCreateCursor(make_glfw_image(w, h, pixels), w, h);
	}

	void * Window::create_standard_cursor(Cursor::Shape value)
	{
		return glfwCreateStandardCursor(([value]()
		{
			switch (value)
			{
			case Cursor::Shape::Arrow: return GLFW_ARROW_CURSOR;
			case Cursor::Shape::TextInput: return GLFW_IBEAM_CURSOR;
			case Cursor::Shape::Crosshair: return GLFW_CROSSHAIR_CURSOR;
			case Cursor::Shape::Hand: return GLFW_POINTING_HAND_CURSOR;
			case Cursor::Shape::ResizeEW: return GLFW_RESIZE_EW_CURSOR;
			case Cursor::Shape::ResizeNS: return GLFW_RESIZE_NS_CURSOR;
			case Cursor::Shape::ResizeNESW: return GLFW_RESIZE_NESW_CURSOR;
			case Cursor::Shape::ResizeNWSE: return GLFW_RESIZE_NWSE_CURSOR;
			default: return GLFW_ARROW_CURSOR;
			}
		})());
	}

	int32_t Window::extension_supported(C_string value)
	{
		return glfwExtensionSupported(value);
	}

	void * Window::get_context_current()
	{
		return glfwGetCurrentContext();
	}

	VideoMode const & Window::get_desktop_mode()
	{
		static VideoMode temp {};
		static bool once { true };
		if (once && !(once = false))
		{
#ifdef ML_OS_WINDOWS
			DEVMODE dm;
			dm.dmSize = sizeof(dm);
			EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);
			temp = VideoMode{ vec2u{
				dm.dmPelsWidth,
				dm.dmPelsHeight },
				dm.dmBitsPerPel
			};
#else
			// do the thing
#endif
		}
		return temp;
	}

	pmr::vector<VideoMode> const & Window::get_fullscreen_modes()
	{
		static pmr::vector<VideoMode> temp {};
		static bool once { true };
		if (once && !(once = false))
		{
#ifdef ML_OS_WINDOWS
			DEVMODE dm;
			dm.dmSize = sizeof(dm);
			for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &dm); ++count)
			{
				auto vm{ VideoMode{ vec2u{
					dm.dmPelsWidth,
					dm.dmPelsHeight },
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

	Window::proc_fn Window::get_proc_address(C_string value)
	{
		return reinterpret_cast<Window::proc_fn>(glfwGetProcAddress(value));
	}

	pmr::vector<void *> const & Window::get_monitors()
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

	float64_t Window::get_time()
	{
		return glfwGetTime();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::error_fn Window::set_error_callback(error_fn value)
	{
		return glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(value))
			? value
			: nullptr;
	}

	Window::char_fn Window::set_char_callback(char_fn value)
	{
		return m_window
			? glfwSetCharCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcharfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	Window::cursor_enter_fn Window::set_cursor_enter_callback(cursor_enter_fn value)
	{
		return m_window
			? glfwSetCursorEnterCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcursorenterfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	Window::cursor_pos_fn Window::set_cursor_pos_callback(cursor_pos_fn value)
	{
		return m_window
			? glfwSetCursorPosCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWcursorposfun>(value)) ? value : nullptr
			: nullptr;
	}

	Window::frame_size_fn Window::set_frame_size_callback(frame_size_fn value)
	{
		return m_window
			? glfwSetFramebufferSizeCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWframebuffersizefun>(value)) ? value : nullptr
			: nullptr;
	}
	
	Window::key_fn Window::set_key_callback(key_fn value)
	{
		return m_window
			? glfwSetKeyCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWkeyfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	Window::mouse_fn Window::set_mouse_callback(mouse_fn value)
	{
		return m_window
			? glfwSetMouseButtonCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWmousebuttonfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	Window::scroll_fn Window::set_scroll_callback(scroll_fn value)
	{
		return m_window
			? glfwSetScrollCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWscrollfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	Window::close_fn Window::set_window_close_callback(close_fn value)
	{
		return m_window
			? glfwSetWindowCloseCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowclosefun>(value)) ? value : nullptr
			: nullptr;
	}
	
	Window::focus_fn Window::set_window_focus_callback(focus_fn value)
	{
		return m_window
			? glfwSetWindowFocusCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowfocusfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	Window::position_fn Window::set_window_pos_callback(position_fn value)
	{
		return m_window
			? glfwSetWindowPosCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowposfun>(value)) ? value : nullptr
			: nullptr;
	}
	
	Window::size_fn Window::set_window_size_callback(size_fn value)
	{
		return m_window
			? glfwSetWindowSizeCallback(
				static_cast<GLFWwindow *>(m_window),
				reinterpret_cast<GLFWwindowposfun>(value)) ? value : nullptr
			: nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}