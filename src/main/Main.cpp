/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Scripting/Script.hpp>

using namespace ml;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MEM_RESERVED
#define MEM_RESERVED 64.0_MiB
#endif

#ifndef WINDOW_TITLE
#define WINDOW_TITLE (ML__NAME " | " ML_STRINGIFY(ML_ARCH) "-bit | " ML_CONFIGURATION)
#endif

#ifndef SETUP_SCRIPT
#define SETUP_SCRIPT L"assets/scripts/setup.py"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// setup memory
	static struct memory_config final : non_copyable
	{
		ds::array<byte_t, MEM_RESERVED>		m_data	{};
		pmr::monotonic_buffer_resource		m_mono	{ m_data.data(), m_data.size() };
		pmr::unsynchronized_pool_resource	m_pool	{ &m_mono };
		detail::test_resource				m_test	{ &m_pool, m_data.data(), m_data.size() };

		memory_config()
		{
			ML_ASSERT(pmr::set_default_resource(&m_test));
			ML_ASSERT(memory_manager::set_test_resource(&m_test));
		}

	} g_memcfg;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// create engine context
	ML_ASSERT(engine::create_context()); ML_DEFER{ ML_ASSERT(engine::destroy_context()); };

	// configure engine
	engine::config & engine_cfg	{ engine::get_config() };
	engine_cfg.command_line		= { ML_ARGV, ML_ARGV + ML_ARGC };
	engine_cfg.program_name		= filesystem::path{ ML_ARGV[0] }.filename();
	engine_cfg.program_path		= filesystem::current_path();
	engine_cfg.content_path		= L"../../../../";
	engine_cfg.library_home		= L"../../../../";
	engine_cfg.window_title		= WINDOW_TITLE;
	engine_cfg.window_display	= make_video_mode(
		vec2i{ 1280, 720 },		// resolution
		32u						// color depth
	);
	engine_cfg.window_context	= make_context_settings(
		client_api::opengl,		// api
		4,						// major version
		6,						// minor version
		client_api::compat,		// profile
		24,						// depth bits
		8,						// stencil bits
		false,					// multisample
		false					// sRGB capable
	);
	engine_cfg.window_hints		= WindowHints_DefaultMaximized;

	// initialize scripting
	ML_ASSERT(script::startup()); ML_DEFER{ ML_ASSERT(script::shutdown()); };
	
	// initialize engine
	ML_ASSERT(engine::startup()); ML_DEFER{ ML_ASSERT(engine::shutdown()); };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// create editor context
	ML_ASSERT(editor::create_context()); ML_DEFER{ ML_ASSERT(editor::destroy_context()); };

	// configure editor
	editor::config & editor_cfg	{ editor::get_config() };
	editor_cfg.api_version		= "#version 130";
	editor_cfg.style			= L"assets/styles/obsidian.style";
	editor_cfg.ini_file			= nullptr;
	editor_cfg.log_file			= nullptr;
	
	// initialize editor
	ML_ASSERT(editor::startup()); ML_DEFER{ ML_ASSERT(editor::shutdown()); };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// run setup script
	std::invoke(script{ engine::path_to(SETUP_SCRIPT) });

	// enter event
	event_system::fire_event<enter_event>();

	// main loop
	while (engine::get_window()->is_open())
	{
		ML_DEFER{ performance_tracker::swap_frames(); };

		// begin frame
		{
			ML_BENCHMARK("| begin frame");
			engine::begin_loop();
			event_system::fire_event<frame_begin_event>();
		}
		// update
		{
			ML_BENCHMARK("|  update");
			event_system::fire_event<update_event>();
		}
		// begin draw
		{
			ML_BENCHMARK("|  begin draw");
			engine::begin_draw();
			event_system::fire_event<begin_draw_event>();
		}
		// draw
		{
			ML_BENCHMARK("|   draw");
			event_system::fire_event<draw_event>();
		}
		// begin gui
		{
			ML_BENCHMARK("|   begin gui");
			editor::new_frame();
			event_system::fire_event<gui_begin_event>();
		}
		// gui
		{
			ML_BENCHMARK("|    gui");
			editor::render();
			event_system::fire_event<gui_draw_event>();
		}
		// end gui
		{
			ML_BENCHMARK("|   end gui");
			editor::render_frame();
			event_system::fire_event<gui_end_event>();
		}
		// end draw
		{
			ML_BENCHMARK("|  end draw");
			engine::end_draw();
			event_system::fire_event<end_draw_event>();
		}
		// end frame
		{
			ML_BENCHMARK("| end frame");
			engine::end_loop();
			event_system::fire_event<frame_end_event>();
		}
	}

	// exit event
	event_system::fire_event<exit_event>();

	// goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}