/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/JSON.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>

using namespace ml;

#ifndef MEM_RESERVED
#define MEM_RESERVED 64.0_MiB
#endif

#ifndef CONFIG_FILE
#define CONFIG_FILE L"../../../../libmeme.json"
#endif

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// setup memory
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

	// load config
	json g_config{};
	std::ifstream file{ CONFIG_FILE };
	ML_assert("CONFIG FILE NOT FOUND" && file);
	file >> g_config;
	file.close();

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// create engine
	ML_assert(engine::create_context()); ML_defer{ ML_assert(engine::destroy_context()); };

	// configure engine
	auto & engine_cfg			{ engine::get_config() };
	engine_cfg.command_line		= { ML_argv, ML_argv + ML_argc };
	engine_cfg.program_path		= filesystem::current_path();
	engine_cfg.program_name		= filesystem::path{ ML_argv[0] }.filename();
	engine_cfg.content_home		= g_config["content_home"].get<pmr::string>();
	engine_cfg.library_home		= g_config["library_home"].get<pmr::string>();

	// start engine
	ML_assert(engine::startup()); ML_defer{ ML_assert(engine::shutdown()); };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// create window
	ML_assert(engine::get_window().create(g_config["window"].get<window_settings>()));

	// install window callbacks
	window::install_default_callbacks(&engine::get_window());

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// create editor
	ML_assert(editor::create_context()); ML_defer{ ML_assert(editor::destroy_context()); };

	// configure editor
	auto & editor_cfg			{ editor::get_config() };
	editor_cfg.api_version 		= g_config["imgui_shading"].get<pmr::string>();
	editor_cfg.ini_filename 	= g_config["use_imgui_ini"].get<bool>() ? "imgui.ini" : nullptr;
	editor_cfg.log_filename 	= g_config["use_imgui_ini"].get<bool>() ? "imgui.log" : nullptr;

	// start editor
	ML_assert(editor::startup()); ML_defer{ ML_assert(editor::shutdown()); };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// run setup script
	engine::do_script(engine::path_to(g_config["setup_script"].get<pmr::string>()));

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// main sequence
	event_system::fire_event<enter_event>();
	ML_defer{ event_system::fire_event<exit_event>(); };
	while (engine::is_running())
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}