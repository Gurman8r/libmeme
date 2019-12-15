/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/Cx.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Platform/WindowEvents.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/SharedLibrary.hpp>


// Tests
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
namespace ml::testing
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static_assert("Unit Tests"

		&& sizeof(mat4b) == (sizeof(unsigned char) * 16)
		&& sizeof(mat4u) == (sizeof(unsigned int) * 16)
		&& sizeof(mat4i) == (sizeof(int) * 16)
		&& sizeof(mat4f) == (sizeof(float) * 16)
		&& sizeof(mat4d) == (sizeof(double) * 16)

		&& nameof_v<bool>			== "bool"
		&& nameof_v<char>			== "char"
		&& nameof_v<wchar_t>		== "wchar_t"
		&& nameof_v<char16_t>		== "char16_t"
		&& nameof_v<char32_t>		== "char32_t"
		&& nameof_v<C_String>		== "const char*"
		&& nameof_v<int8_t>			== "signed char"
		&& nameof_v<int16_t>		== "short"
		&& nameof_v<int32_t>		== "int"
#ifdef ML_CC_MSC
		&& nameof_v<int64_t>		== "__int64"
#else
		&& nameof_v<int64_t>		== "long long"
#endif
		&& nameof_v<uint8_t>		== "unsigned char"
		&& nameof_v<uint16_t>		== "unsigned short"
		&& nameof_v<uint32_t>		== "unsigned int"
#ifdef ML_CC_MSC
		&& nameof_v<uint64_t>		== "unsigned __int64"
#else
		&& nameof_v<uint64_t>		== "unsigned long long"
#endif
		&& nameof_v<float32_t>		== "float"
		&& nameof_v<float64_t>		== "double"
		&& nameof_v<float80_t>		== "long double"
#ifdef ML_HAS_CONSTEXPR_17
		&& nameof_v<std::string>	== "std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
		&& nameof_v<std::wstring>	== "std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >"
		&& nameof_v<std::u16string>	== "std::basic_string<char16_t,struct std::char_traits<char16_t>,class std::allocator<char16_t> >"
		&& nameof_v<std::u32string>	== "std::basic_string<char32_t,struct std::char_traits<char32_t>,class std::allocator<char32_t> >"
#else
		&& nameof_v<std::string>	== "std::string"
		&& nameof_v<std::wstring>	== "std::wstring"
		&& nameof_v<std::u16string>	== "std::u16string"
		&& nameof_v<std::u32string>	== "std::u32string"
#endif
	);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


// Settings
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
namespace ml
{
	static constexpr auto const window_title{ StringView {
		"libmeme"					// Title
	} };
	static constexpr auto const window_video{ Window::DisplayMode {
		1280,						// Width
		720,						// Height
		32							// Bits-per-Pixel
	} };
	static constexpr auto const window_style{ Window::Style {
		true,						// Resizable
		true,						// Visible
		true,						// Decorated
		true,						// Focused
		true,						// Auto Iconify
		false,						// Floating
		false,						// Maximized
		false,						// Fullscreen
		false,						// Vertical Sync
	} };
	static constexpr auto const window_context{ Window::Context {
		Window::Context::OpenGL,	// API
		4,							// Major Version
		6,							// Minor Version
		Window::Context::Compat,	// Profile
		24,							// Depth Bits
		8,							// Stencil Bits
		false,						// Multisample
		false						// sRGB Capable
	} };

	// State
	static struct Monostate final
	{
		RenderWindow window{};

		struct Time final
		{
			Timer main{ true };

			Timer loop{ false };

			float64_t delta{ 0.0 };

		} time{};

	} state{};
}


// Main
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ml::int32_t main()
{
	using namespace ml;

	// Init Systems
	ML_MemoryTracker; ML_EventSystem; ML_PerformanceTracker;

	// Enter Event
	ML_EventSystem.fireEvent<EnterEvent>(ML_ARGC, ML_ARGV);

	// Create Window
	if (!state.window.create(window_title.str(), window_video, window_style, window_context))
	{
		return Debug::logError("Failed initializing Window") | Debug::pause(1);
	}
	else
	{
		// Install Callbacks
		/* * * * * * * * * * * * * * * * * * * * */
		state.window.setCharCallback([](auto, auto ch)
		{
			ML_EventSystem.fireEvent<CharEvent>(ch);
		});
		state.window.setCursorEnterCallback([](auto, auto entered)
		{
			ML_EventSystem.fireEvent<CursorEnterEvent>(entered);
		});
		state.window.setCursorPosCallback([](auto, auto x, auto y)
		{
			ML_EventSystem.fireEvent<CursorPosEvent>(x, y);
		});
		state.window.setErrorCallback([](auto code, auto desc)
		{
			ML_EventSystem.fireEvent<WindowErrorEvent>(code, desc);
		});
		state.window.setFrameSizeCallback([](auto, auto w, auto h)
		{
			ML_EventSystem.fireEvent<FrameSizeEvent>(w, h);
		});
		state.window.setKeyCallback([](auto, auto button, auto scan, auto action, auto mods)
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
		state.window.setMouseCallback([](auto, auto button, auto action, auto mods)
		{
			ML_EventSystem.fireEvent<MouseEvent>(button, action, mods);
		});
		state.window.setScrollCallback([](auto, auto x, auto y)
		{
			ML_EventSystem.fireEvent<ScrollEvent>(x, y);
		});
		state.window.setWindowCloseCallback([](auto)
		{
			ML_EventSystem.fireEvent<WindowCloseEvent>();
		});
		state.window.setWindowFocusCallback([](auto, auto focused)
		{
			ML_EventSystem.fireEvent<WindowFocusEvent>(focused);
		});
		state.window.setWindowPosCallback([](auto, auto x, auto y)
		{
			ML_EventSystem.fireEvent<WindowPosEvent>(x, y);
		});
		state.window.setWindowSizeCallback([](auto, auto w, auto h)
		{
			ML_EventSystem.fireEvent<WindowSizeEvent>(w, h);
		});
	}

	// Start Editor
	if (!Editor::startup(state.window.getHandle()))
	{
		return Debug::logError("Failed initializing Editor") | Debug::pause(1);
	}

	// Load Event
	ML_EventSystem.fireEvent<LoadEvent>();

	// Load Plugins
	auto load_plugin = [](auto filename)
	{
		auto library{ new SharedLibrary{ ML_FS.pathTo(filename) } };
		auto plugin{ library->callFunction<Plugin *>("ML_Plugin_Main") };
		return _STD make_pair(library, plugin);
	};
	std::vector<std::pair<SharedLibrary *, Plugin *>> plugins;
	plugins.push_back(load_plugin("demo.dll"));


	// Loop
	/* * * * * * * * * * * * * * * * * * * * */
	while (state.window.isOpen())
	{
		state.time.loop.start();
		
		// Begin Step
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("STEP_BEGIN");
			Window::pollEvents();
			ML_EventSystem.fireEvent<BeginStepEvent>();
		}
		// Update
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("\tUPDATE");
			ML_EventSystem.fireEvent<UpdateEvent>();
		}
		// Draw
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("\tDRAW");
			ML_EventSystem.fireEvent<DrawEvent>();
		}
		// Begin Gui
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("\tGUI_BEGIN");
			Editor::new_frame();
			ML_EventSystem.fireEvent<BeginGuiEvent>();
		}
		// Gui
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("\t\tGUI");
			ML_EventSystem.fireEvent<GuiEvent>();
		}
		// End Gui
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("\tGUI_END");
			Editor::render_frame();
			ML_EventSystem.fireEvent<EndGuiEvent>();
		}
		// End Step
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("STEP_END");
			ML_EventSystem.fireEvent<EndStepEvent>();
			if (state.window.getStyle().vertical_sync()) { state.window.swapBuffers(); } // Vsync
			else { GL::flush(); } // Uncapped
		}
		ML_PerformanceTracker.end_frame();
		state.time.delta = state.time.loop.stop().elapsed().count();
	}


	// Cleanup
	ML_EventSystem.fireEvent<UnloadEvent>();
	
	for (auto & pair : plugins)
	{
		delete pair.second;
		delete pair.first;
	}
	plugins.clear();

	Editor::shutdown();
	
	state.window.dispose();
	
	ML_EventSystem.fireEvent<ExitEvent>();

	// Goodbye!
	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */