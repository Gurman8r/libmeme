#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/ScopeGuard.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>

#if ML_DEBUG
#	define CONF_NAME "debug"
#else
#	define CONF_NAME "release"
#endif
#define WINDOW_TITLE "libmeme | " CONF_NAME " | " ML_STRINGIFY(ML_ARCH) "-bit"

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using namespace ml;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// configure memory
	static struct memory_config final : non_copyable
	{
		ds::array<byte_t, 16_MiB>			data{};
		pmr::monotonic_buffer_resource		mono{ data.data(), data.size() };
		pmr::unsynchronized_pool_resource	pool{ &mono };
		detail::test_resource				test{ &pool, data.data(), data.size() };

		memory_config() noexcept
		{
			ML_ASSERT(pmr::set_default_resource(&test));
			ML_ASSERT(memory_manager::startup(&test));
		}

	} g_mem;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// create context
	ML_ASSERT(engine::create_context()); ML_DEFER{ ML_ASSERT(engine::destroy_context()); };
	ML_ASSERT(editor::create_context()); ML_DEFER{ ML_ASSERT(editor::destroy_context()); };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// configure engine
	engine::config & engine_cfg	= engine::get_config();
	engine_cfg.command_line		= { ML_ARGV, ML_ARGV + ML_ARGC };
	engine_cfg.program_name		= fs::path{ ML_ARGV[0] }.filename();
	engine_cfg.program_path		= fs::current_path();
	engine_cfg.content_path		= "../../../../";
	engine_cfg.library_path		= "";

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// configure editor
	editor::config & editor_cfg	= editor::get_config();
	editor_cfg.api_version		= "#version 130";
	editor_cfg.style			= "dark";
	editor_cfg.ini_file			= nullptr;
	editor_cfg.log_file			= nullptr;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// create window
	ML_ASSERT(engine::get_window().create
	(
		pmr::string{ WINDOW_TITLE },	// title
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
		WindowFlags_DefaultMaximized	// flags
	));

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// startup
	ML_ASSERT(engine::startup(true)); ML_DEFER{ ML_ASSERT(engine::shutdown()); };
	ML_ASSERT(editor::startup(true)); ML_DEFER{ ML_ASSERT(editor::shutdown()); };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// run main script
	script{ engine::path_to("main.py") }();

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// enter event
	event_system::fire_event<enter_event>();

	// main loop
	while (engine::get_window().is_open())
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