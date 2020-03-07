/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/JSON.hpp>
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

static struct memory_setup final : non_copyable
{
	ds::array<byte_t, MEM_RESERVED>		m_data{};
	pmr::monotonic_buffer_resource		m_mono{ m_data.data(), m_data.size() };
	pmr::unsynchronized_pool_resource	m_pool{ &m_mono };
	detail::test_resource				m_test{ &m_pool, m_data.data(), m_data.size() };

	memory_setup() noexcept
	{
		ML_assert(pmr::set_default_resource(&m_test));
		ML_assert(memory_manager::set_test_resource(&m_test));
	}

} g_memcfg;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static auto const g_config{ R"(
{
	"content_home":		"../../../../",
	"library_home":		"../../../../",
	"setup_script":		"assets/scripts/setup.py",

	"win_title":		"libmeme",
	"win_resolution":	[ 1280, 720 ],
	"win_color_depth":	32,
	"win_hints":		93,
	"win_api":			1,
	"win_api_major":	4,
	"win_api_minor":	6,
	"win_api_profile":	2,
	"win_depth_bits":	24,
	"win_stencil_bits": 8,
	"win_multisample":	false,
	"win_srgb_capable": false,

	"imgui_shading":	"#version 130",
	"use_imgui_ini":	false,
	"use_imgui_log":	false
}
)"_json };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// create contexts
	ML_assert(engine::create_context()); ML_defer{ ML_assert(engine::destroy_context()); };
	ML_assert(editor::create_context()); ML_defer{ ML_assert(editor::destroy_context()); };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// configure engine
	auto & engine_cfg			{ engine::get_config() };
	engine_cfg.command_line		= { ML_argv, ML_argv + ML_argc };
	engine_cfg.program_path		= filesystem::current_path();
	engine_cfg.program_name		= filesystem::path{ ML_argv[0] }.filename();
	engine_cfg.content_home		= g_config["content_home"].get<pmr::string>();
	engine_cfg.library_home		= g_config["library_home"].get<pmr::string>();

	// configure editor
	auto & editor_cfg			{ editor::get_config() };
	editor_cfg.api_version		= g_config["imgui_shading"].get<pmr::string>();
	editor_cfg.ini_filename		= g_config["use_imgui_ini"].get<bool>() ? "imgui.ini" : nullptr;
	editor_cfg.log_filename		= g_config["use_imgui_ini"].get<bool>() ? "imgui.log" : nullptr;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// start engine
	ML_assert(engine::startup()); ML_defer{ ML_assert(engine::shutdown()); };

	// create window
	ML_assert(engine::get_window().create(make_window_settings
	(
		g_config["win_title"].get<pmr::string>(),
		make_video_mode
		(
			g_config["win_resolution"].get<vec2i>(),
			g_config["win_color_depth"].get<uint32_t>()
		),
		make_context_settings
		(
			g_config["win_api"].get<int32_t>(),
			g_config["win_api_major"].get<int32_t>(),
			g_config["win_api_minor"].get<int32_t>(),
			g_config["win_api_profile"].get<int32_t>(),
			g_config["win_depth_bits"].get<int32_t>(),
			g_config["win_stencil_bits"].get<int32_t>(),
			g_config["win_multisample"].get<bool>(),
			g_config["win_srgb_capable"].get<bool>()
		),
		g_config["win_hints"].get<int32_t>()
	)));

	// install callbacks
	window::install_default_callbacks(&engine::get_window());

	// start editor
	ML_assert(editor::startup()); ML_defer{ ML_assert(editor::shutdown()); };

	// run setup script
	engine::do_script(engine::path_to(g_config["setup_script"].get<pmr::string>()));

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}