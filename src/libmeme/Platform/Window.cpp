#include <libmeme/Platform/Window.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Core/EventSystem.hpp>
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
	static GLFWimage const * glfw_image(vec2s const & size, byte_t const * pixels) noexcept
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

	bool window::open(window_settings const & ws)
	{
		if (is_open())			{ return debug::log::error("window is already open"); }
		if (!initialize())		{ return debug::log::error("failed initializing backend"); }
		if (ws.title.empty())	{ return debug::log::error("invalid window title"); }
		if (!ws.video)			{ return debug::log::error("invalid video mode"); }
		if (!ws.context)		{ return debug::log::error("invalid context settings"); }
		
		m_settings = ws;

		// api hints
		glfwWindowHint(GLFW_CLIENT_API, ([&]() noexcept
		{
			switch (m_settings.context.api)
			{
			case platform_api::opengl	: return GLFW_OPENGL_API;
			case platform_api::vulkan	:
			case platform_api::directx	:
			default						: return GLFW_NO_API;
			}
		})());
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,	m_settings.context.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,	m_settings.context.minor);

		// profile hints
		glfwWindowHint(GLFW_OPENGL_PROFILE, ([&]() noexcept
		{
			switch (m_settings.context.profile)
			{
			case platform_api::core		: return GLFW_OPENGL_CORE_PROFILE;
			case platform_api::compat	: return GLFW_OPENGL_COMPAT_PROFILE;
			case platform_api::debug	: return GLFW_OPENGL_DEBUG_CONTEXT;
			case platform_api::any		:
			default						: return GLFW_OPENGL_ANY_PROFILE;
			}
		})());
		glfwWindowHint(GLFW_DEPTH_BITS,		m_settings.context.depth_bits);
		glfwWindowHint(GLFW_STENCIL_BITS,	m_settings.context.stencil_bits);
		glfwWindowHint(GLFW_SRGB_CAPABLE,	m_settings.context.srgb_capable);

		// style hints
		glfwWindowHint(GLFW_RESIZABLE,		has_hint(window_hints_resizable));
		glfwWindowHint(GLFW_VISIBLE,		has_hint(window_hints_visible));
		glfwWindowHint(GLFW_DECORATED,		has_hint(window_hints_decorated));
		glfwWindowHint(GLFW_FOCUSED,		has_hint(window_hints_focused));
		glfwWindowHint(GLFW_AUTO_ICONIFY,	has_hint(window_hints_auto_iconify));
		glfwWindowHint(GLFW_FLOATING,		has_hint(window_hints_floating));
		glfwWindowHint(GLFW_MAXIMIZED,		has_hint(window_hints_maximized));
		glfwWindowHint(GLFW_DOUBLEBUFFER,	has_hint(window_hints_double_buffered));
		
		// create window
		if (!([&]() noexcept
		{
			return m_window = glfwCreateWindow(
				m_settings.video.size[0],
				m_settings.video.size[1],
				m_settings.title.c_str(),
				static_cast<GLFWmonitor *>(m_monitor),
				static_cast<GLFWwindow *>(m_share)
			);
		})())
		{
			return debug::log::error("failed creating glfw window");
		}

		make_context_current();

		if (has_hint(window_hints_install_callbacks))
		{
			install_default_callbacks();
		}

		set_cursor_mode(cursor_mode_normal);

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

		return is_open();
	}

	bool window::close()
	{
		if (!is_open()) { return false; }

		set_should_close(true);

		destroy();

		window::terminate();

		m_window = m_monitor = m_share = nullptr;

		return !is_open();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::destroy()
	{
		if (!m_window) { return; }

		glfwDestroyWindow(static_cast<GLFWwindow *>(m_window));
	}

	void window::iconify()
	{
		if (!m_window) { return; }

		glfwIconifyWindow(static_cast<GLFWwindow *>(m_window));
	}

	void window::make_context_current()
	{
		if (!m_window) { return; }

		make_context_current(m_window);
	}
	
	void window::maximize()
	{
		if (!m_window) { return; }

		glfwMaximizeWindow(static_cast<GLFWwindow *>(m_window));

		m_settings.hints |= window_hints_maximized;
	}

	void window::restore()
	{
		if (!m_window) { return; }

		glfwRestoreWindow(static_cast<GLFWwindow *>(m_window));

		m_settings.hints &= ~window_hints_maximized;
	}

	void window::swap_buffers()
	{
		if (!m_window) { return; }

		if (has_hint(window_hints_double_buffered))
		{
			glfwSwapBuffers(static_cast<GLFWwindow *>(m_window));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::set_centered()
	{
		set_position((vec2i)(get_desktop_mode().size - get_size()) / 2);
	}

	void window::set_clipboard(cstring const & value)
	{
		if (!m_window) { return; }

		glfwSetClipboardString(static_cast<GLFWwindow *>(m_window), value);
	}

	void window::set_cursor(cursor_handle value)
	{
		if (!m_window) { return; }
		
		glfwSetCursor(static_cast<GLFWwindow *>(m_window), static_cast<GLFWcursor *>(value));
	}
	
	void window::set_cursor_mode(int32_t value)
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

	void window::set_cursor_pos(vec2d const & value)
	{
		if (!m_window) { return; }

		glfwSetCursorPos(static_cast<GLFWwindow *>(m_window), value[0], value[1]);
	}

	void window::set_fullscreen(bool value)
	{
		set_monitor(value ? glfwGetPrimaryMonitor() : nullptr);
	}

	void window::set_icon(size_t w, size_t h, byte_t const * pixels)
	{
		if (!m_window) { return; }

		if (!w || !h || !pixels) { return; }

		glfwSetWindowIcon(static_cast<GLFWwindow *>(m_window), 1, impl::glfw_image({ w, h }, pixels));
	}

	void window::set_input_mode(int32_t mode, int32_t value)
	{
		if (!m_window) { return; }

		glfwSetInputMode(static_cast<GLFWwindow *>(m_window), mode, value);
	}

	void window::set_position(vec2i const & value)
	{
		if (!m_window) { return; }
		
		glfwSetWindowPos(static_cast<GLFWwindow *>(m_window), value[0], value[1]);
	}

	void window::set_monitor(window_handle value)
	{
		if (!m_window || m_monitor == value) { return; }

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

	void window::set_should_close(bool value)
	{
		if (!m_window) { return; }

		glfwSetWindowShouldClose(static_cast<GLFWwindow *>(m_window), value);
	}

	void window::set_size(vec2i const & value)
	{
		if (!m_window) { return; }
		
		m_settings.video.size = value;
		
		glfwSetWindowSize(static_cast<GLFWwindow *>(m_window), get_width(), get_height());
	}

	void window::set_title(pmr::string const & value)
	{
		if (!m_window || value.empty()) { return; }

		m_settings.title = value;

		glfwSetWindowTitle(static_cast<GLFWwindow *>(m_window), get_title().c_str());
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
		if (!m_window) { return false; }

		return !glfwWindowShouldClose(static_cast<GLFWwindow *>(m_window));
	}
	
	int32_t window::get_attribute(int32_t value) const
	{
		if (!m_window) { return 0; }

		return glfwGetWindowAttrib(static_cast<GLFWwindow *>(m_window), value);
	}

	cstring window::get_clipboard() const
	{
		if (!m_window) { return nullptr; }

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

	window_handle window::get_handle() const
	{
		return m_window;
	}

	int32_t	window::get_key(int32_t key) const
	{
		if (!m_window) { return 0; }

		return glfwGetKey(static_cast<GLFWwindow *>(m_window), key);
	}

	int32_t window::get_input_mode(int32_t mode) const
	{
		if (!m_window) { return 0; }

		return glfwGetInputMode(static_cast<GLFWwindow *>(m_window), mode);
	}

	int32_t	window::get_mouse_button(int32_t button) const
	{
		if (!m_window) { return 0; }

		return glfwGetMouseButton(static_cast<GLFWwindow *>(m_window), button);
	}

	window_handle window::get_native_handle() const
	{
#ifdef ML_os_windows
		return glfwGetWin32Window(static_cast<GLFWwindow *>(m_window));
#else
		return m_window;
#endif
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void window::destroy_cursor(cursor_handle value)
	{
		if (!value) { return; }

		glfwDestroyCursor(static_cast<GLFWcursor *>(value));
	}

	void window::make_context_current(cursor_handle value)
	{
		if (!value) { return; }

		glfwMakeContextCurrent(static_cast<GLFWwindow *>(value));
	}

	void window::poll_events()
	{
		glfwPollEvents();
	}

	void window::swap_interval(int32_t value)
	{
		glfwSwapInterval(value);
	}

	void window::terminate()
	{
		glfwTerminate();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	cursor_handle window::create_custom_cursor(uint32_t w, uint32_t h, byte_t const * pixels)
	{
		return glfwCreateCursor(impl::glfw_image({ w, h }, pixels), w, h);
	}

	cursor_handle window::create_standard_cursor(int32_t value)
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

	int32_t window::extension_supported(cstring value)
	{
		return glfwExtensionSupported(value);
	}

	cursor_handle window::get_context_current()
	{
		return glfwGetCurrentContext();
	}

	video_mode const & window::get_desktop_mode()
	{
		static video_mode temp{};
		static ML_scope
		{
#ifdef ML_os_windows
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
		};
		return temp;
	}

	pmr::vector<video_mode> const & window::get_fullscreen_modes()
	{
		static pmr::vector<video_mode> temp{};
		static ML_scope
		{
#ifdef ML_os_windows
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
		};
		return temp;
	}

	window::proc_fn window::get_proc_address(cstring value)
	{
		return reinterpret_cast<window::proc_fn>(glfwGetProcAddress(value));
	}

	pmr::vector<window_handle> const & window::get_monitors()
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

	float64_t window::get_time()
	{
		return glfwGetTime();
	}

	bool window::initialize()
	{
		return glfwInit();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window::error_fn window::set_error_callback(error_fn value)
	{
		return glfwSetErrorCallback(
			reinterpret_cast<GLFWerrorfun>(value)) ? value : nullptr;
	}

	window::char_fn window::set_char_callback(char_fn value)
	{
		if (!m_window) { return nullptr; }
		
		return glfwSetCharCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWcharfun>(value)) ? value : nullptr;
	}
	
	window::cursor_enter_fn window::set_cursor_enter_callback(cursor_enter_fn value)
	{
		if (!m_window) { return nullptr; }

		return glfwSetCursorEnterCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWcursorenterfun>(value)) ? value : nullptr;
	}
	
	window::cursor_pos_fn window::set_cursor_pos_callback(cursor_pos_fn value)
	{
		if (!m_window) { return nullptr; }

		return glfwSetCursorPosCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWcursorposfun>(value)) ? value : nullptr;
	}

	window::frame_size_fn window::set_frame_size_callback(frame_size_fn value)
	{
		if (!m_window) { return nullptr; }

		return glfwSetFramebufferSizeCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWframebuffersizefun>(value)) ? value : nullptr;
	}
	
	window::key_fn window::set_key_callback(key_fn value)
	{
		if (!m_window) { return nullptr; }

		return glfwSetKeyCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWkeyfun>(value)) ? value : nullptr;
	}
	
	window::mouse_fn window::set_mouse_callback(mouse_fn value)
	{
		if (!m_window) { return nullptr; }

		return glfwSetMouseButtonCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWmousebuttonfun>(value)) ? value : nullptr;
	}
	
	window::scroll_fn window::set_scroll_callback(scroll_fn value)
	{
		if (!m_window) { return nullptr; }

		return glfwSetScrollCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWscrollfun>(value)) ? value : nullptr;
	}
	
	window::close_fn window::set_window_close_callback(close_fn value)
	{
		if (!m_window) { return nullptr; }

		return glfwSetWindowCloseCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWwindowclosefun>(value)) ? value : nullptr;
	}
	
	window::focus_fn window::set_window_focus_callback(focus_fn value)
	{
		if (!m_window) { return nullptr; }

		return glfwSetWindowFocusCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWwindowfocusfun>(value)) ? value : nullptr;
	}
	
	window::position_fn window::set_window_pos_callback(position_fn value)
	{
		if (!m_window) { return nullptr; }

		return glfwSetWindowPosCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWwindowposfun>(value)) ? value : nullptr;
	}
	
	window::size_fn window::set_window_size_callback(size_fn value)
	{
		if (!m_window) { return nullptr; }

		return glfwSetWindowSizeCallback(
			static_cast<GLFWwindow *>(m_window),
			reinterpret_cast<GLFWwindowposfun>(value)) ? value : nullptr;
	}

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