
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Random.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/Cx.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Platform/WindowEvents.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>
#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/Texture.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/Python.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/SharedLibrary.hpp>
#include <libmeme/Renderer/Color.hpp>

// Unit Tests
namespace ml::unit_tests
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static_assert("Unit Tests"

		&& sizeof(mat4b) == (sizeof(unsigned char) * 16)
		&& sizeof(mat4u) == (sizeof(unsigned int) * 16)
		&& sizeof(mat4i) == (sizeof(int) * 16)
		&& sizeof(mat4f) == (sizeof(float) * 16)
		&& sizeof(mat4d) == (sizeof(double) * 16)

		&& nameof_v<bool> == "bool"
		&& nameof_v<char> == "char"
		&& nameof_v<wchar_t> == "wchar_t"
		&& nameof_v<char16_t> == "char16_t"
		&& nameof_v<char32_t> == "char32_t"
		&& nameof_v<C_String> == "const char*"
		&& nameof_v<int8_t> == "signed char"
		&& nameof_v<int16_t> == "short"
		&& nameof_v<int32_t> == "int"
#ifdef ML_CC_MSC
		&& nameof_v<int64_t> == "__int64"
#else
		&& nameof_v<int64_t> == "long long"
#endif
		&& nameof_v<uint8_t> == "unsigned char"
		&& nameof_v<uint16_t> == "unsigned short"
		&& nameof_v<uint32_t> == "unsigned int"
#ifdef ML_CC_MSC
		&& nameof_v<uint64_t> == "unsigned __int64"
#else
		&& nameof_v<uint64_t> == "unsigned long long"
#endif
		&& nameof_v<float32_t> == "float"
		&& nameof_v<float64_t> == "double"
		&& nameof_v<float80_t> == "long double"
#ifdef ML_HAS_CONSTEXPR_17
		&& nameof_v<std::string> == "std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
		&& nameof_v<std::wstring> == "std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >"
		&& nameof_v<std::u16string> == "std::basic_string<char16_t,struct std::char_traits<char16_t>,class std::allocator<char16_t> >"
		&& nameof_v<std::u32string> == "std::basic_string<char32_t,struct std::char_traits<char32_t>,class std::allocator<char32_t> >"
#else
		&& nameof_v<std::string> == "std::string"
		&& nameof_v<std::wstring> == "std::wstring"
		&& nameof_v<std::u16string> == "std::u16string"
		&& nameof_v<std::u32string> == "std::u32string"
#endif
	);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


// Settings
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr auto const window_title{ C_String {
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


// Main
ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using namespace ml;

	// Time
	struct Time final
	{
		Timer main{ true };
		Timer loop{ false };
		float64_t delta{ 0.0 };
	} time{};
	
	// Init Systems
	ML_MemoryTracker;
	ML_Random;
	ML_EventSystem;
	ML_PerformanceTracker;
	ML_Lua.init();
	ML_Python.init(ML_ARGV[0], "../../../");

	auto a = make_uniform<mat4>("", mat4{});
	auto c = a.exchange(mat4());

	Script{ Script::Language::Python, "import LIBMEME as ml" }();

	// Load Plugins
	std::vector<std::pair<SharedLibrary *, Plugin *>> plugins;
	auto load_plugin = [&plugins](auto && filename)
	{
		auto library{ new SharedLibrary{ filename } };
		plugins.push_back(std::make_pair(
			library,
			library->callFunction<Plugin *>("ML_Plugin_Main")
		));
	};
	load_plugin("demo.dll");

	// Enter Event
	ML_EventSystem.fireEvent<EnterEvent>(ML_ARGC, ML_ARGV);

	// Create Window
	RenderWindow window{};
	if (!window.create(window_title, window_video, window_style, window_context))
	{
		return Debug::logError("Failed initializing Window") | Debug::pause(1);
	}
	else
	{
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
		window.setErrorCallback([](auto code, auto desc)
		{
			ML_EventSystem.fireEvent<WindowErrorEvent>(code, desc);
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
	}

	// Start Editor
	if (!Editor::startup(window.getHandle()))
	{
		return Debug::logError("Failed initializing Editor") | Debug::pause(1);
	}

	// Load Event
	ML_EventSystem.fireEvent<LoadEvent>();

	static std::vector<Image>		images{};
	static std::vector<Texture>		textures{};
	static std::vector<Shader>		shaders{};
	static std::vector<Material>	materials{};

	materials.push_back(make_material(
		make_uniform<bool>("bool", true),
		make_uniform<int>("int", 123),
		make_uniform<float>("float", 4.56f),
		make_uniform<vec2>("vec2", vec2{ 1, 2 }),
		make_uniform<vec3>("vec3", vec3{ 3, 4, 5 }),
		make_uniform<vec4>("vec4", vec4{ 6, 7, 8, 9 }),
		make_uniform<Color>("col", colors::magenta),
		make_uniform<mat2>("mat2", mat2::identity()),
		make_uniform<mat3>("mat3", mat3::identity()),
		make_uniform<mat4>("mat4", mat4::identity()),
		make_uniform<Texture *>("tex", nullptr)
	));

	// Loop
	/* * * * * * * * * * * * * * * * * * * * */
	while (window.isOpen())
	{
		time.loop.start();
		
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
			if (window.getStyle().vertical_sync()) { window.swapBuffers(); } // Vsync
			else { GL::flush(); } // Uncapped
		}
		ML_PerformanceTracker.end_frame();
		time.delta = time.loop.stop().elapsed().count();
	}


	// Cleanup
	ML_EventSystem.fireEvent<UnloadEvent>();
	for (auto & pair : plugins)
	{
		if (pair.second) { delete pair.second; }
		if (pair.first) { delete pair.first; }
	}
	plugins.clear();
	Editor::shutdown();
	window.dispose();
	ML_Python.dispose();
	ML_Lua.dispose();
	ML_EventSystem.fireEvent<ExitEvent>();

	// Goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}