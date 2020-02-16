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
	static auto g_buff = ds::array<byte_t, 64._MiB>{};
	static auto g_mono = pmr::monotonic_buffer_resource{ g_buff.data(), g_buff.size() };
	static auto g_pool = pmr::unsynchronized_pool_resource{ &g_mono };
	pmr::set_default_resource(&g_pool);
	memory_tracker::set_buffer(g_buff.data(), g_buff.size());

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// setup engine
	if (!engine::create_context() || !([&]()
	{
		engine::config & config	= engine::get_config();
		config.program_name		= ML_ARGV[0];
		config.command_line		= { ML_ARGV, ML_ARGV + ML_ARGC };
		config.library_path		= "../../../../";
		config.content_path		= "../../../../assets/";
		config.script_list		= { "../../../../libmeme.py" };
		config.plugin_list		= {};
		config.window_title		= "libmeme";
		config.window_flags		= WindowFlags_Default;
		config.window_video		= make_video_mode(
			vec2u{ 1280, 720 },	// resolution
			32u					// color depth
		);
		config.window_context	= make_context_settings(
			client_api::opengl,	// api
			4,					// major version
			6,					// minor version
			client_api::compat,	// profile
			24,					// depth bits
			8,					// stencil bits
			false,				// multisample
			false				// sRGB capable
		);
		return engine::startup(true);
	})())
	{
		engine::shutdown();
		return debug::log_error("failed initializing engine") | debug::pause(1);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// setup editor
	if (!editor::create_context() || !([&]()
	{
		editor::config & config	= editor::get_config();
		config.window_handle	= engine::get_window().get_handle();
		config.api_version		= "#version 130";
		config.style			= "classic";
		config.ini_file			= nullptr;
		config.log_file			= nullptr;
		return editor::startup(true);
	})())
	{
		editor::shutdown();
		engine::shutdown();
		return debug::log_error("failed initializing editor") | debug::pause(1);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// enter event
	event_system::fire_event<enter_event>(ML_ARGC, ML_ARGV);

	// main loop
	while (engine::running())
	{
		ML_DEFER{ performance_tracker::refresh_frames(); };

		// begin frame
		{
			ML_BENCHMARK("FRAME_BEGIN");
			engine::begin_loop();
			event_system::fire_event<frame_begin_event>();
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
			event_system::fire_event<draw_event>(engine::get_window());
		}
		// begin gui
		{
			ML_BENCHMARK("\tGUI_BEGIN");
			editor::new_frame();
			editor::get_main_menu().render();
			editor::get_dockspace().render();
			event_system::fire_event<gui_begin_event>();
		}
		// gui
		{
			ML_BENCHMARK("\t\tGUI");
			event_system::fire_event<gui_draw_event>();
		}
		// end gui
		{
			ML_BENCHMARK("\tGUI_END");
			editor::render_frame();
			event_system::fire_event<gui_end_event>();
		}
		// end frame
		{
			ML_BENCHMARK("FRAME_END");
			engine::end_loop();
			event_system::fire_event<frame_end_event>();
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