#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// setup memory

	static auto g_block = ds::array<byte_t, 100._MiB>{ 0 };
	static auto g_buff = pmr::monotonic_buffer_resource{ g_block.data(), g_block.size() };
	static auto g_pool = pmr::unsynchronized_pool_resource{ &g_buff };
	
	pmr::set_default_resource(&g_pool);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// startup engine
	if (!Engine::startup({ ML_ARGV[0], "../../../" }))
	{
		return debug::log_error("Failed initializing Engine") | debug::pause(1);
	}

	// enter event
	EventSystem::fire_event<EnterEvent>(ML_ARGC, ML_ARGV);

	// create window
	if (!Engine::init_window(window_settings))
	{
		return debug::log_error("Failed initializing Window") | debug::pause(1);
	}

	// startup editor
	if (!Editor::startup({ Engine::window().get_handle(), true }))
	{
		return debug::log_error("Failed initializing Editor") | debug::pause(1);
	}

	// load plugins
	Engine::load_plugin("demo.dll");

	// load event
	EventSystem::fire_event<LoadEvent>();

	// main loop
	while (Engine::running())
	{
		static float64_t const // timers
			& tt = Engine::time().total,
			& dt = Engine::time().delta;

		// begin loop
		{
			ML_BENCHMARK("LOOP_BEGIN");
			Engine::begin_loop();
			EventSystem::fire_event<BeginLoopEvent>();
		}
		// update
		{
			ML_BENCHMARK("\tUPDATE");
			EventSystem::fire_event<UpdateEvent>(tt, dt);
		}
		// draw
		{
			ML_BENCHMARK("\tDRAW");
			Engine::begin_draw();
			EventSystem::fire_event<DrawEvent>(tt, dt);
		}
		// begin gui
		{
			ML_BENCHMARK("\tGUI_BEGIN");
			Editor::new_frame();
			EventSystem::fire_event<BeginGuiEvent>();
		}
		// gui
		{
			ML_BENCHMARK("\t\tGUI");
			Editor::main_menu().render();
			Editor::dockspace().render();
			EventSystem::fire_event<GuiEvent>(tt, dt);
		}
		// end gui
		{
			ML_BENCHMARK("\tGUI_END");
			Editor::render_frame();
			EventSystem::fire_event<EndGuiEvent>();
		}
		// end loop
		{
			ML_BENCHMARK("LOOP_END");
			Engine::end_loop();
			EventSystem::fire_event<EndLoopEvent>();
		}
		PerformanceTracker::swap();
	}

	// unload event
	EventSystem::fire_event<UnloadEvent>();

	// shutdown
	Editor::shutdown();
	Engine::shutdown();

	// exit event
	EventSystem::fire_event<ExitEvent>();

	// goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */