#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/Cx.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/Python.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Engine/Script.hpp>
#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

// Tests
namespace ml::tests
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static_assert("Tests"
		&& nameof_t<bool>			== "bool"
		&& nameof_t<int8_t>			== "signed char"
		&& nameof_t<int16_t>		== "short"
		&& nameof_t<int32_t>		== "int"
		&& nameof_t<int64_t>		== "long long"
		&& nameof_t<uint8_t>		== "unsigned char"
		&& nameof_t<uint16_t>		== "unsigned short"
		&& nameof_t<uint32_t>		== "unsigned int"
		&& nameof_t<uint64_t>		== "unsigned long long"
		&& nameof_t<float32_t>		== "float"
		&& nameof_t<float64_t>		== "double"
		&& nameof_t<float80_t>		== "long double"
		&& nameof_t<char>			== "char"
		&& nameof_t<wchar_t>		== "wchar_t"
		&& nameof_t<char16_t>		== "char16_t"
		&& nameof_t<char32_t>		== "char32_t"
		&& nameof_t<C_string>		== "const char*"
		&& nameof_t<C_wstring>		== "const wchar_t*"
		&& nameof_t<C_u16string>	== "const char16_t*"
		&& nameof_t<C_u32string>	== "const char32_t*"
		&& nameof_t<vec2>			== "struct ml::Matrix<float,2,1>"
		&& nameof_t<vec3>			== "struct ml::Matrix<float,3,1>"
		&& nameof_t<vec4>			== "struct ml::Matrix<float,4,1>"
		&& nameof_t<mat2>			== "struct ml::Matrix<float,2,2>"
		&& nameof_t<mat3>			== "struct ml::Matrix<float,3,3>"
		&& nameof_t<mat4>			== "struct ml::Matrix<float,4,4>"
		&& nameof_t<std::string>	== "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
		&& nameof_t<std::wstring>	== "class std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >"
		&& nameof_t<std::u16string> == "class std::basic_string<char16_t,struct std::char_traits<char16_t>,class std::allocator<char16_t> >"
		&& nameof_t<std::u32string> == "class std::basic_string<char32_t,struct std::char_traits<char32_t>,class std::allocator<char32_t> >"
	);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Window Options
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr auto const window_title{
		"libmeme"sv			// Title
	};
	static constexpr auto const window_video{ DisplayMode{
		vec2u{ 1280, 720 },	// Size
		32u					// Bits-per-Pixel
	} };
	static constexpr auto const window_style{ WindowSettings{
		true,				// Resizable
		true,				// Visible
		true,				// Decorated
		true,				// Focused
		true,				// Auto Iconify
		false,				// Floating
		false,				// Maximized
		false,				// Fullscreen
		false				// Vertical Sync
	} };
	static constexpr auto const window_context{ ContextSettings{
		Client_API::OpenGL,	// API
		4,					// Major Version
		6,					// Minor Version
		Client_API::Compat,	// Profile
		24,					// Depth Bits
		8,					// Stencil Bits
		false,				// Multisample
		false				// sRGB Capable
	} };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Plugin Loader
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct PluginLoader final : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		PluginLoader() noexcept : m_filenames{}, m_plugins{}
		{
		}

		~PluginLoader()
		{
			m_plugins.for_each([](auto const &, auto & plugin)
			{
				delete plugin;
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Plugin * operator()(path_t && path)
		{
			if (auto const file{ m_filenames.insert(std::move(path)) }; file.second)
			{
				if (auto library{ make_shared_library(*file.first) })
				{
					if (auto const plugin{ library.call_function<Plugin *>("ML_Plugin_Main") })
					{
						return (*m_plugins.insert(std::move(library), plugin).first.second);
					}
				}
				m_filenames.erase(file.first);
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ds::flat_set<path_t> m_filenames;

		ds::flat_map<SharedLibrary, Plugin *> m_plugins;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
	{
		// Load Plugins
		PluginLoader plugin_loader{};
		plugin_loader("demo.dll");

		// Startup Engine
		if (!Engine::startup(
			{ ML_ARGV[0], "../../../" }
		))
		{
			return Debug::log_error("Failed initializing Engine") | Debug::pause(1);
		}

		// Enter Event
		EventSystem::fire_event<EnterEvent>(ML_ARGC, ML_ARGV);

		// Create Window
		if (!Engine::create_window(
			{ std::string{ window_title }, window_video, window_style, window_context }
		))
		{
			return Debug::log_error("Failed initializing Window") | Debug::pause(1);
		}

		// Startup Editor
		if (!Editor::startup(
			{ Engine::window().get_handle(), true }
		))
		{
			return Debug::log_error("Failed initializing Editor") | Debug::pause(1);
		}

		// Load Event
		EventSystem::fire_event<LoadEvent>();

		// Main Loop
		while (Engine::running())
		{
			time.loop.stop().start();

			// Begin Loop
			{
				ML_BENCHMARK("LOOP_BEGIN");
				Engine::begin_loop();
				EventSystem::fire_event<BeginLoopEvent>();
			}
			// Update
			{
				ML_BENCHMARK("\tUPDATE");
				EventSystem::fire_event<UpdateEvent>();
			}
			// Draw
			{
				ML_BENCHMARK("\tDRAW");
				EventSystem::fire_event<DrawEvent>();
			}
			// Begin Gui
			{
				ML_BENCHMARK("\tGUI_BEGIN");
				Editor::new_frame();
				Editor::main_menu().render();
				Editor::dockspace().render();
				EventSystem::fire_event<BeginGuiEvent>();
			}
			// Gui
			{
				ML_BENCHMARK("\t\tGUI");
				EventSystem::fire_event<GuiEvent>();
			}
			// End Gui
			{
				ML_BENCHMARK("\tGUI_END");
				Editor::render_frame();
				EventSystem::fire_event<EndGuiEvent>();
			}
			// End Loop
			{
				ML_BENCHMARK("LOOP_END");
				Engine::end_loop();
				EventSystem::fire_event<EndLoopEvent>();
			}
			PerformanceTracker::swap();

			time.delta = time.loop.elapsed().count();
		}

		// Unload Event
		EventSystem::fire_event<UnloadEvent>();
		Editor::shutdown();
		Engine::shutdown();

		// Exit Event
		EventSystem::fire_event<ExitEvent>();
	}

	// Goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */