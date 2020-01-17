#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/TypeOf.hpp>
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

// Window Options
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr Engine::WindowSettings window_settings
	{
		"libmeme",				// title
		make_video_mode(
			vec2u{ 1280, 720 },	// resolution
			32u					// color depth
		),
		make_context_settings(
			Client_API::OpenGL,	// api
			4,					// major version
			6,					// minor version
			Client_API::Compat,	// profile
			24,					// depth bits
			8,					// stencil bits
			false,				// multisample
			false				// sRGB capable
		),
		WindowFlags_Default,	// flags
		true					// install callbacks
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Main
ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using namespace ml;

	// Startup Engine
	if (!Engine::startup({ ML_ARGV[0], "../../../" }))
	{
		return debug::log_error("Failed initializing Engine") | debug::pause(1);
	}

	// Enter Event
	EventSystem::fire_event<EnterEvent>(ML_ARGC, ML_ARGV);

	// Load Plugins
	Engine::load_plugin("demo.dll");

	// Create Window
	if (!Engine::init_window(window_settings))
	{
		return debug::log_error("Failed initializing Window") | debug::pause(1);
	}

	// Startup Editor
	if (!Editor::startup({ Engine::window().get_handle(), true }))
	{
		return debug::log_error("Failed initializing Editor") | debug::pause(1);
	}

	// Load Event
	EventSystem::fire_event<LoadEvent>();

	// Main Loop
	while (Engine::running())
	{
		static float64_t const // Timers
			& tt = Engine::time().total,
			& dt = Engine::time().delta;

		// Begin Loop
		{
			ML_BENCHMARK("LOOP_BEGIN");
			Engine::begin_loop();
			EventSystem::fire_event<BeginLoopEvent>();
		}
		// Update
		{
			ML_BENCHMARK("\tUPDATE");
			EventSystem::fire_event<UpdateEvent>(tt, dt);
		}
		// Draw
		{
			ML_BENCHMARK("\tDRAW");
			Engine::begin_draw();
			EventSystem::fire_event<DrawEvent>(tt, dt);
		}
		// Begin Gui
		{
			ML_BENCHMARK("\tGUI_BEGIN");
			Editor::new_frame();
			EventSystem::fire_event<BeginGuiEvent>();
		}
		// Gui
		{
			ML_BENCHMARK("\t\tGUI");
			Editor::main_menu().render();
			Editor::dockspace().render();
			EventSystem::fire_event<GuiEvent>(tt, dt);
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
	}

	// Unload Event
	EventSystem::fire_event<UnloadEvent>();

	// Shutdown
	Editor::shutdown();
	Engine::shutdown();

	// Exit Event
	EventSystem::fire_event<ExitEvent>();

	// Goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */