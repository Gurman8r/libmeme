#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/EventHandler.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/Cx.hpp>
#include <libmeme/Core/DenseMap.hpp>
#include <libmeme/Core/FlatSet.hpp>
#include <libmeme/Platform/WindowEvents.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/Python.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/SharedLibrary.hpp>
#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

// Tests
namespace ml::tests
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static_assert("Tests"

		&& sizeof(mat4b) == (sizeof(byte_t) * 16)
		&& sizeof(mat4u) == (sizeof(uint32_t) * 16)
		&& sizeof(mat4i) == (sizeof(int32_t) * 16)
		&& sizeof(mat4f) == (sizeof(float32_t) * 16)
		&& sizeof(mat4d) == (sizeof(float64_t) * 16)
		&& sizeof(mat4s) == (sizeof(size_t) * 16)

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

		&& nameof_v<vec2>			== "struct ml::Matrix<float,2,1>"
		&& nameof_v<vec3>			== "struct ml::Matrix<float,3,1>"
		&& nameof_v<vec4>			== "struct ml::Matrix<float,4,1>"
		&& nameof_v<mat2>			== "struct ml::Matrix<float,2,2>"
		&& nameof_v<mat3>			== "struct ml::Matrix<float,3,3>"
		&& nameof_v<mat4>			== "struct ml::Matrix<float,4,4>"

		&& nameof_v<std::string>	== "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
		&& nameof_v<std::wstring>	== "class std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >"
		&& nameof_v<std::u16string> == "class std::basic_string<char16_t,struct std::char_traits<char16_t>,class std::allocator<char16_t> >"
		&& nameof_v<std::u32string> == "class std::basic_string<char32_t,struct std::char_traits<char32_t>,class std::allocator<char32_t> >"
	);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


// Window Options
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr auto const window_title{ C_String {
		"libmeme"					// Title
	} };
	static constexpr auto const window_mode{ DisplayMode {
		1280,						// Width
		720,						// Height
		32							// Bits-per-Pixel
	} };
	static constexpr auto const window_settings{ WindowSettings {
		1,							// Resizable
		1,							// Visible
		1,							// Decorated
		1,							// Focused
		1,							// Auto Iconify
		0,							// Floating
		0,							// Maximized
		0,							// Fullscreen
		0,							// Vertical Sync
	} };
	static constexpr auto const window_context{ ContextSettings {
		ContextSettings::OpenGL,	// API
		4,							// Major Version
		6,							// Minor Version
		ContextSettings::Compat,	// Profile
		24,							// Depth Bits
		8,							// Stencil Bits
		false,						// Multisample
		false						// sRGB Capable
	} };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


// Plugin Loader
namespace ml
{
	struct PluginLoader final : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		PluginLoader() noexcept : m_files{}, m_plugins{}
		{
		}

		~PluginLoader()
		{
			for (auto & elem : m_plugins)
			{
				delete elem.second;
				delete elem.first;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class P = path_t> inline decltype(auto) load(P && path)
		{
			if (auto const file{ m_files.insert(std::forward<P>(path)) }; file.second)
			{
				if (auto * const library{ new SharedLibrary{ *file.first } }; library->good())
				{
					if (auto * const plugin{ library->call_function<Plugin *>("ML_Plugin_Main") })
					{
						return m_plugins.insert({ library, plugin }).first;
					}
				}
				else
				{
					delete library;
				}
			}
			return m_plugins.end();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ds::set<path_t> m_files;

		pair_map<SharedLibrary *, Plugin *> m_plugins;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}


// Main
ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using namespace ml;

	// Timers
	static struct Time final
	{
		Timer main{ true }, loop{ false };

		float64_t delta{ 0.0 };
	}
	time{};

	// Testing
	EventHandler::Pool flow;
	flow.add_listener<EnterEvent>([](Event const & value)
	{
		if (auto const ev = value.as<EnterEvent>())
		{
			for (int32_t i = 0; i < ev->argc; ++i)
			{
				printf("%s\n", ev->argv[i]);
			}
		}
	});

	// Enter
	EventSystem::fire_event<EnterEvent>(ML_ARGC, ML_ARGV);
	{
		// Startup Engine
		if (!Engine::startup(ML_ARGV[0], "../../../"))
		{
			return Debug::logError("Failed initializing Engine") | Debug::pause(1);
		}

		// Load Plugins
		PluginLoader plugins{};
		plugins.load("demo.dll");

		// Create Window
		if (!Engine::create_window({ window_title, window_mode, window_settings, window_context }))
		{
			return Debug::logError("Failed initializing Window") | Debug::pause(1);
		}

		// Startup Editor
		if (!Editor::startup(Engine::window().get_handle()))
		{
			return Debug::logError("Failed initializing Editor") | Debug::pause(1);
		}

		// Load
		EventSystem::fire_event<LoadEvent>();

		// Main Loop
		while (Engine::running())
		{
			time.loop.start();

			// Begin Loop
			/* * * * * * * * * * * * * * * * * * * * */
			{
				ML_BENCHMARK("LOOP_BEGIN");
				Engine::begin_loop();
				EventSystem::fire_event<BeginLoopEvent>();
			}
			// Update
			/* * * * * * * * * * * * * * * * * * * * */
			{
				ML_BENCHMARK("\tUPDATE");
				EventSystem::fire_event<UpdateEvent>();
			}
			// Draw
			/* * * * * * * * * * * * * * * * * * * * */
			{
				ML_BENCHMARK("\tDRAW");
				EventSystem::fire_event<DrawEvent>();
			}
			// Begin Gui
			/* * * * * * * * * * * * * * * * * * * * */
			{
				ML_BENCHMARK("\tGUI_BEGIN");
				Editor::new_frame();
				EventSystem::fire_event<BeginGuiEvent>();
			}
			// Gui
			/* * * * * * * * * * * * * * * * * * * * */
			{
				ML_BENCHMARK("\t\tGUI");
				Editor::main_menu().render();
				Editor::dockspace().render();
				EventSystem::fire_event<GuiEvent>();
			}
			// End Gui
			/* * * * * * * * * * * * * * * * * * * * */
			{
				ML_BENCHMARK("\tGUI_END");
				Editor::render_frame();
				EventSystem::fire_event<EndGuiEvent>();
			}
			// End Loop
			/* * * * * * * * * * * * * * * * * * * * */
			{
				ML_BENCHMARK("LOOP_END");
				EventSystem::fire_event<EndLoopEvent>();
				Engine::end_loop();
			}
			PerformanceTracker::swap();
			time.delta = time.loop.stop().elapsed().count();
		}

		// Unload
		EventSystem::fire_event<UnloadEvent>();
		Editor::shutdown();
		Engine::shutdown();
	}
	// Exit
	EventSystem::fire_event<ExitEvent>();

	// Goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}