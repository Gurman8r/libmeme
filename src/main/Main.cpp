/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ml;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MEM_RESERVED
#define MEM_RESERVED 64.0_MiB
#endif

#ifndef CONTENT_PATH
#define CONTENT_PATH L"../../../../"
#endif

#ifndef LIBRARY_HOME
#define LIBRARY_HOME L"../../../../"
#endif

#ifndef WINDOW_TITLE
#define WINDOW_TITLE (ML__NAME " | " ML_STRINGIFY(ML_ARCH) "-bit | " ML_CONFIGURATION)
#endif

#ifndef SETUP_SCRIPT
#define SETUP_SCRIPT L"assets/scripts/setup.py"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// memory settings
static struct memory_config final : non_copyable
{
	ds::array<byte_t, MEM_RESERVED>		m_data{};
	pmr::monotonic_buffer_resource		m_mono{ m_data.data(), m_data.size() };
	pmr::unsynchronized_pool_resource	m_pool{ &m_mono };
	detail::test_resource				m_test{ &m_pool, m_data.data(), m_data.size() };

	memory_config() noexcept
	{
		ML_assert(pmr::set_default_resource(&m_test));
		ML_assert(memory_manager::set_test_resource(&m_test));
	}

} g_memcfg;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// window settings
static constexpr auto window_cfg = make_window_settings
(
	WINDOW_TITLE,					// title
	make_video_mode
	(
		vec2i{ 1280, 720 },			// resolution
		32u							// color depth
	),
	make_context_settings
	(
		client_api::opengl,			// api
		4,							// major version
		6,							// minor version
		client_api::compat,			// profile
		24,							// depth bits
		8,							// stencil bits
		false,						// multisample
		false						// sRGB capable
	),
	WindowHints_DefaultMaximized	// hints
);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// create engine
	ML_assert(engine::create_context()); ML_defer{ ML_assert(engine::destroy_context()); };

	// configure engine
	auto & engine_cfg			{ engine::get_config() };
	engine_cfg.command_line		= { ML_argv, ML_argv + ML_argc };
	engine_cfg.program_name		= filesystem::path{ ML_argv[0] }.filename();
	engine_cfg.program_path		= filesystem::current_path();
	engine_cfg.content_path		= CONTENT_PATH;
	engine_cfg.library_home		= LIBRARY_HOME;

	// start engine
	ML_assert(engine::startup()); ML_defer{ ML_assert(engine::shutdown()); };

	// create window
	ML_assert(engine::get_window().create(window_cfg));

	// install window callbacks
	window::install_default_callbacks(&engine::get_window());

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// create editor
	ML_assert(editor::create_context()); ML_defer{ ML_assert(editor::destroy_context()); };

	// configure editor
	auto & editor_cfg			{ editor::get_config() };
	editor_cfg.api_version		= "#version 130";
	editor_cfg.ini_file			= nullptr;
	editor_cfg.log_file			= nullptr;
	
	// start editor
	ML_assert(editor::startup()); ML_defer{ ML_assert(editor::shutdown()); };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// run setup script
	engine::do_script(engine::path_to(SETUP_SCRIPT));

	// main sequence
	event_system::fire_event<enter_event>();
	ML_defer{ event_system::fire_event<exit_event>(); };
	while (engine::is_open())
	{
		ML_defer{ performance_tracker::swap_frames(); };
		{
			ML_benchmark("| begin frame");
			engine::begin_loop();
			event_system::fire_event<frame_begin_event>();
		}
		{
			ML_benchmark("|  update");
			event_system::fire_event<update_event>();
		}
		{
			ML_benchmark("|  begin draw");
			engine::begin_draw();
			event_system::fire_event<begin_draw_event>();
		}
		{
			ML_benchmark("|   draw");
			event_system::fire_event<draw_event>();
		}
		{
			ML_benchmark("|   begin gui");
			editor::new_frame();
			event_system::fire_event<gui_begin_event>();
		}
		{
			ML_benchmark("|    gui");
			editor::render();
			event_system::fire_event<gui_draw_event>();
		}
		{
			ML_benchmark("|   end gui");
			editor::render_frame();
			event_system::fire_event<gui_end_event>();
		}
		{
			ML_benchmark("|  end draw");
			engine::end_draw();
			event_system::fire_event<end_draw_event>();
		}
		{
			ML_benchmark("| end frame");
			engine::end_loop();
			event_system::fire_event<frame_end_event>();
		}
	}
	
	// goodbye!
	return EXIT_SUCCESS;
}