/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Graphics/Color.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Graphics/OpenGL.hpp>

// Unit Tests
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr bool unit_tests{ "Unit Tests"

		&& sizeof(mat4b) == (sizeof(unsigned char) * 16)
		&& sizeof(mat4u) == (sizeof(unsigned int) * 16)
		&& sizeof(mat4i) == (sizeof(int) * 16)
		&& sizeof(mat4f) == (sizeof(float) * 16)
		&& sizeof(mat4d) == (sizeof(double) * 16)

		&& typeof<>{ typeof_v<int> }.name() == "int"

		&& typeof<bool>::name()				== "bool"
		&& typeof<char>::name()				== "char"
		&& typeof<wchar_t>::name()			== "wchar_t"
		&& typeof<char16_t>::name()			== "char16_t"
		&& typeof<char32_t>::name()			== "char32_t"
		&& typeof<C_String>::name()			== "const char*"
		&& typeof<int8_t>::name()			== "signed char"
		&& typeof<int16_t>::name()			== "short"
		&& typeof<int32_t>::name()			== "int"
#ifdef ML_CC_MSC
		&& typeof<int64_t>::name()			== "__int64"
#else
		&& typeof<int64_t>::name()			== "long long"
#endif
		&& typeof<uint8_t>::name()			== "unsigned char"
		&& typeof<uint16_t>::name()			== "unsigned short"
		&& typeof<uint32_t>::name()			== "unsigned int"
#ifdef ML_CC_MSC
		&& typeof<uint64_t>::name()			== "unsigned __int64"
#else
		&& typeof<uint64_t>::name()			== "unsigned long long"
#endif
		&& typeof<float32_t>::name()		== "float"
		&& typeof<float64_t>::name()		== "double"
		&& typeof<float80_t>::name()		== "long double"
#ifdef ML_HAS_CONSTEXPR_17
		&& typeof<std::string>::name()		== "std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
		&& typeof<std::wstring>::name()		== "std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >"
		&& typeof<std::u16string>::name()	== "std::basic_string<char16_t,struct std::char_traits<char16_t>,class std::allocator<char16_t> >"
		&& typeof<std::u32string>::name()	== "std::basic_string<char32_t,struct std::char_traits<char32_t>,class std::allocator<char32_t> >"
#else
		&& typeof<std::string>::name()		== "std::string"
		&& typeof<std::wstring>::name()		== "std::wstring"
		&& typeof<std::u16string>::name()	== "std::u16string"
		&& typeof<std::u32string>::name()	== "std::u32string"
#endif
	};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


// Window Settings
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static constexpr auto video_mode{ ml::VideoMode
{
	1280,	// Width
	720,	// Height
	32		// Bits-Per-Pixel
} };

static constexpr auto window_style{ ml::WindowStyle
{
	1,		// Resizable
	1,		// Visible
	1,		// Decorated
	1,		// Focused
	1,		// Auto Iconify
	0,		// Floating
	0,		// Maximized
	0,		// Fullscreen
	1,		// Vertical Sync
} };

static constexpr auto context_settings{ ml::ContextSettings
{
	4,		// Major
	6,		// Minor
	2,		// Profile (Any, Core, Compat, Debug)
	24,		// Depth Bits
	8,		// Stencil Bits
	false,	// Multisample
	false	// sRGB Capable
} };


// Main
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ml::int32_t main()
{
	using namespace ml;

	static_assert(unit_tests);

	// Init Systems
	/* * * * * * * * * * * * * * * * * * * * */

	ML_EventSystem;

	// Create Window
	/* * * * * * * * * * * * * * * * * * * * */

	RenderWindow window{};

	Window::setErrorCallback([](auto code, auto desc)
	{
		ML_EventSystem.fireEvent<WindowErrorEvent>(code, desc);
	});

	if (!window.create("libmeme", video_mode, window_style, context_settings))
	{
		return Debug::logError("Failed creating window")
			| Debug::pause(EXIT_FAILURE);
	}

	// Install Callbacks
	/* * * * * * * * * * * * * * * * * * * * */

	window.setCharCallback([](auto, auto ch)
	{
		ML_EventSystem.fireEvent<CharEvent>(ch);
	});
	window.setCursorEnterCallback([](auto, auto entered)
	{
		ML_EventSystem.fireEvent<CursorEnterEvent>(entered);
	});
	window.setCursorPosCallback([](auto, auto x, auto y)
	{
		ML_EventSystem.fireEvent<CursorPosEvent>(x, y);
	});
	window.setFrameSizeCallback([](auto, auto w, auto h)
	{
		ML_EventSystem.fireEvent<FrameSizeEvent>(w, h);
	});
	window.setKeyCallback([](auto, auto button, auto scan, auto action, auto mods)
	{
		ML_EventSystem.fireEvent<KeyEvent>(button, scan, action, mask8_t{ {
			(mods & ML_MOD_SHIFT),
			(mods & ML_MOD_CTRL),
			(mods & ML_MOD_ALT),
			(mods & ML_MOD_SUPER),
			(mods & ML_MOD_CAPSLOCK),
			(mods & ML_MOD_NUMLOCK)
		} });
	});
	window.setMouseCallback([](auto, auto button, auto action, auto mods)
	{
		ML_EventSystem.fireEvent<MouseEvent>(button, action, mods);
	});
	window.setScrollCallback([](auto, auto x, auto y)
	{
		ML_EventSystem.fireEvent<ScrollEvent>(x, y);
	});
	window.setWindowCloseCallback([](auto)
	{
		ML_EventSystem.fireEvent<WindowCloseEvent>();
	});
	window.setWindowFocusCallback([](auto, auto focused)
	{
		ML_EventSystem.fireEvent<WindowFocusEvent>(focused);
	});
	window.setWindowPosCallback([](auto, auto x, auto y)
	{
		ML_EventSystem.fireEvent<WindowPosEvent>(x, y);
	});
	window.setWindowSizeCallback([](auto, auto w, auto h)
	{
		ML_EventSystem.fireEvent<WindowSizeEvent>(w, h);
	});

	// Loop
	/* * * * * * * * * * * * * * * * * * * * */

	while (window.isOpen())
	{
		window.beginFrame();
		{
			// do stuff, etc...
			
			static Color bg{ colors::magenta };

			ML_GL.clear(GL::DepthBufferBit | GL::ColorBufferBit);

			ML_GL.clearColor(bg[0], bg[1], bg[2], bg[3]);
		}
		window.endFrame();
	}

	// Cleanup
	/* * * * * * * * * * * * * * * * * * * * */

	window.dispose();

	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */