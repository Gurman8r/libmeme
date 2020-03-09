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
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	static GLFWimage const * make_glfw_image(vec2s const & size, byte_t const * pixels) noexcept
	{
		if (!size[0] || !size[1] || !pixels) return nullptr;
		static pmr::vector<GLFWimage> cache{};
		return &cache.emplace_back(
			GLFWimage{ (int32_t)size[0], (int32_t)size[1], (uint8_t *)pixels }
		);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window::window() noexcept
		: m_window	{}
		, m_monitor	{}
		, m_share	{}
		, m_settings{}
	{
#ifdef ML_OS_WINDOWS
		if (HWND window{ GetConsoleWindow() })
		{
			if (HMENU menu{ GetSystemMenu(window, false) })
			{
				EnableMenuItem(menu, SC_CLOSE, MF_GRAYED);
			}
		}
#endif
	}
	
	window::~window() noexcept
	{
#ifdef ML_OS_WINDOWS
		if (HWND window{ GetConsoleWindow() })
		{
			if (HMENU menu{ GetSystemMenu(window, false) })
			{
				EnableMenuItem(menu, SC_CLOSE, MF_ENABLED);
			}
		}
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool window::create(pmr::string const & title, display_settings const & video, context_settings const & context, int32_t hints) noexcept
	{
		return create(window_settings{ title, video, context, hints });
	}

	bool window::create(window_settings const & value) noexcept
	{
		if (is_open()) return debug::log::error("window is already open");
		m_settings = value;
		return open();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool window::open()
	{
		// validate
		{
			if (is_open()) return debug::log::error("window is already open");

			if (!glfwInit()) return debug::log::error("failed initializing glfw");

			if (m_settings.title.empty()) return debug::log::error("invalid window title");

			if (!m_settings.display) return debug::log::error("invalid window video mode");

			if (!m_settings.context) return debug::log::error("invalid window context settings");
		}
		
		// context api
		glfwWindowHint(GLFW_CLIENT_API, ([&]()
		{
			switch (m_settings.context.api)
			{
			case client_api::opengl	: return GLFW_OPENGL_API;
			case client_api::vulkan	:
			case client_api::directx:
			default					: return GLFW_NO_API;
			}
		})());
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,	m_settings.context.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,	m_settings.context.minor);

		// context profile
		glfwWindowHint(GLFW_OPENGL_PROFILE, ([&]() {
			switch (m_settings.context.profile)
			{
			case client_api::core	: return GLFW_OPENGL_CORE_PROFILE;
			case client_api::compat	: return GLFW_OPENGL_COMPAT_PROFILE;
			case client_api::debug	: return GLFW_OPENGL_DEBUG_CONTEXT;
			case client_api::any	:
			default					: return GLFW_OPENGL_ANY_PROFILE;
			}
		})());
		glfwWindowHint(GLFW_DEPTH_BITS,				m_settings.context.depth_bits);
		glfwWindowHint(GLFW_STENCIL_BITS,			m_settings.context.stencil_bits);
		glfwWindowHint(GLFW_SRGB_CAPABLE,			m_settings.context.sRGB_capable);

		// style hints
		glfwWindowHint(GLFW_RESIZABLE,				has_hint(window_hints_resizable));
		glfwWindowHint(GLFW_VISIBLE,				has_hint(window_hints_visible));
		glfwWindowHint(GLFW_DECORATED,				has_hint(window_hints_decorated));
		glfwWindowHint(GLFW_FOCUSED,				has_hint(window_hints_focused));
		glfwWindowHint(GLFW_AUTO_ICONIFY,			has_hint(window_hints_auto_iconify));
		glfwWindowHint(GLFW_FLOATING,				has_hint(window_hints_floating));
		glfwWindowHint(GLFW_MAXIMIZED,				has_hint(window_hints_maximized));
		glfwWindowHint(GLFW_DOUBLEBUFFER,			has_hint(window_hints_double_buffered));
		
		// create window
		if (!(m_window = static_cast<GLFWwindow *>(glfwCreateWindow(
			m_settings.display.resolution[0],
			m_settings.display.resolution[1],
			m_settings.title.c_str(),
			static_cast<GLFWmonitor *>(m_monitor),
			static_cast<GLFWwindow *>(m_share)
		))))
		{
			return debug::log::error("failed creating glfw window");
		}

		make_context_current();

		set_cursor_mode(cursor::mode::normal);

		if (has_hint(window_hints_fullscreen))
		{
			set_fullscreen(true); // fullscreen
		}
		else if (has_hint(window_hints_maximized))
		{
			maximize(); // maximized
		}
		else
		{
			set_centered(); // centered
		}

		return true;
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

			m_settings.hints |= window_hints_maximized;
		}
		return (*this);
	}

	window & window::restore()
	{
		if (m_window)
		{
			glfwRestoreWindow(static_cast<GLFWwindow *>(m_window));

			m_settings.hints &= ~window_hints_maximized;
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

	window & window::set_clipboard(cstring const & value)
	{
		if (m_window)
		{
			glfwSetClipboardString(static_cast<GLFWwindow *>(m_window), value);
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

				set_centered();
			}
		}
		return (*this);
	}

	window & window::set_size(vec2i const & value)
	{
		m_settings.display.resolution = value;
		if (m_window)
		{
			glfwSetWindowSize(static_cast<GLFWwindow *>(m_window), get_width(), get_height());
		}
		return (*this);
	}

	window & window::set_title(pmr::string const & value)
	{
		m_settings.title = value;
		if (m_window && !m_settings.title.empty())
		{
			glfwSetWindowTitle(static_cast<GLFWwindow *>(m_window), m_settings.title.c_str());
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
		if (!m_window) return false;
		return !glfwWindowShouldClose(static_cast<GLFWwindow *>(m_window));
	}
	
	int32_t window::get_attribute(int32_t value) const
	{
		if (!m_window) return 0;
		return glfwGetWindowAttrib(static_cast<GLFWwindow *>(m_window), value);
	}

	cstring window::get_clipboard() const
	{
		if (!m_window) return nullptr;
		return glfwGetClipboardString(static_cast<GLFWwindow *>(m_window));
	}

	vec2 window::get_cursor_pos() const
	{
		vec2d temp{};
		if (m_window)
		{
			glfwGetCursorPos(static_cast<GLFWwindow *>(m_window), &temp[0], &temp[1]);
		}
		return (vec2)temp;
	}

	vec2i window::get_frame_size() const
	{
		vec2i temp{};
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
		if (!m_window) return 0;
		return glfwGetKey(static_cast<GLFWwindow *>(m_window), value);
	}

	int32_t window::get_input_mode(int32_t value) const
	{
		if (!m_window) return 0;
		return glfwGetInputMode(static_cast<GLFWwindow *>(m_window), value);
	}

	int32_t	window::get_mouse_button(int32_t value) const
	{
		if (!m_window) return 0;
		return glfwGetMouseButton(static_cast<GLFWwindow *>(m_window), value);
	}

	vec2i window::get_position() const
	{
		vec2i temp{};
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

	display_settings const & window::get_desktop_mode()
	{
		static display_settings temp{};
		static bool once { true };
		if (once && !(once = false))
		{
#ifdef ML_OS_WINDOWS
			DEVMODE dm; dm.dmSize = sizeof(dm);
			EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);
			temp = display_settings{ vec2i{
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

	pmr::vector<display_settings> const & window::get_fullscreen_modes()
	{
		static pmr::vector<display_settings> temp{};
		static bool once { true };
		if (once && !(once = false))
		{
#ifdef ML_OS_WINDOWS
			DEVMODE dm; dm.dmSize = sizeof(dm);
			for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &dm); ++count)
			{
				auto const vm{ display_settings{ vec2i{
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
		static pmr::vector<void *> temp{};
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

	void window::install_default_callbacks(window * win)
	{
		if (!win || !win->is_open()) return;

		win->set_char_callback([](auto, auto ... args)
		{
			event_system::fire_event<char_event>(ML_forward(args)...);
		});

		win->set_cursor_enter_callback([](auto, auto ... args)
		{
			event_system::fire_event<cursor_enter_event>(ML_forward(args)...);
		});

		win->set_cursor_pos_callback([](auto, auto ... args)
		{
			event_system::fire_event<cursor_pos_event>(ML_forward(args)...);
		});

		win->set_error_callback([](auto ... args)
		{
			event_system::fire_event<window_error_event>(ML_forward(args)...);
		});

		win->set_frame_size_callback([](auto, auto ... args)
		{
			event_system::fire_event<frame_size_event>(ML_forward(args)...);
		});

		win->set_key_callback([](auto, auto ... args)
		{
			event_system::fire_event<key_event>(ML_forward(args)...);
		});

		win->set_mouse_callback([](auto, auto ... args)
		{
			event_system::fire_event<mouse_event>(ML_forward(args)...);
		});

		win->set_scroll_callback([](auto, auto ... args)
		{
			event_system::fire_event<scroll_event>(ML_forward(args)...);
		});

		win->set_window_close_callback([](auto)
		{
			event_system::fire_event<window_close_event>();
		});

		win->set_window_focus_callback([](auto, auto ... args)
		{
			event_system::fire_event<window_focus_event>(ML_forward(args)...);
		});

		win->set_window_pos_callback([](auto, auto ... args)
		{
			event_system::fire_event<window_pos_event>(ML_forward(args)...);
		});

		win->set_window_size_callback([](auto, auto ... args)
		{
			event_system::fire_event<window_size_event>(ML_forward(args)...);
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}