#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/ScopeGuard.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using namespace ml;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// setup memory
	static auto g_raw = ds::array<byte_t, 64._MiB>{};
	static auto g_buf = pmr::monotonic_buffer_resource{ g_raw.data(), g_raw.size() };
	static auto g_pool = pmr::unsynchronized_pool_resource{ &g_buf };
	pmr::set_default_resource(&g_pool);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// engine configuration
	if (!engine::create_context())
	{
		return debug::log_error("failed creating engine context") | debug::pause(1);
	}
	else
	{
		engine::config & config	= engine::get_config();
		config.program_name		= ML_ARGV[0];
		config.library_path		= "../../../"s;
		config.script_list		= { "../../../libmeme.py"s };
		config.plugin_list		= {};
		config.window_title		= "libmeme";
		config.window_flags		= WindowFlags_Default;
		config.window_video		= make_video_mode(
			vec2u{ 1280, 720 },	// resolution
			32u					// color depth
		);
		config.window_context	= make_context_settings(
			client_api::OpenGL,	// api
			4,					// major version
			6,					// minor version
			client_api::Compat,	// profile
			24,					// depth bits
			8,					// stencil bits
			false,				// multisample
			false				// sRGB capable
		);
	}

	// start engine
	if (!engine::startup(true))
	{
		return debug::log_error("failed starting engine") | debug::pause(1);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// editor configuration
	if (!editor::create_context())
	{
		return debug::log_error("failed creating editor context") | debug::pause(1);
	}
	else
	{
		editor::config & config	= editor::get_config();
		config.window_handle	= engine::get_window().get_handle();
		config.api_version		= "#version 130";
		config.style			= "dark";
		config.ini_file			= "";
		config.log_file			= "";
	}

	// start editor
	if (!editor::startup(true))
	{
		return debug::log_error("failed starting editor") | debug::pause(1);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// enter event
	event_system::fire_event<enter_event>(ML_ARGC, ML_ARGV);

	// main loop
	while (engine::running())
	{
		ML_DEFER{ performance_tracker::refresh(); };

		// begin loop
		{
			ML_BENCHMARK("LOOP_BEGIN");
			engine::begin_loop();
			event_system::fire_event<begin_loop_event>();
		}
		// update
		{
			ML_BENCHMARK("\tUPDATE");
			event_system::fire_event<update_event>();
		}
		// draw
		{
			ML_BENCHMARK("\tDRAW");
			engine::begin_draw();
			event_system::fire_event<draw_event>();
		}
		// begin gui
		{
			ML_BENCHMARK("\tGUI_BEGIN");
			editor::new_frame();
			editor::get_main_menu().render();
			editor::get_dockspace().render();
			event_system::fire_event<begin_gui_event>();
		}
		// gui
		{
			ML_BENCHMARK("\t\tGUI");
			event_system::fire_event<gui_event>();
		}
		// end gui
		{
			ML_BENCHMARK("\tGUI_END");
			editor::render_frame();
			event_system::fire_event<end_gui_event>();
		}
		// end loop
		{
			ML_BENCHMARK("LOOP_END");
			engine::end_loop();
			event_system::fire_event<end_loop_event>();
		}
	}

	// exit event
	event_system::fire_event<exit_event>();

	// shutdown
	editor::shutdown();
	engine::shutdown();

	// goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}