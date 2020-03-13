#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/JSON.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>

using namespace ml;

#ifndef MEM_RESERVED
#define MEM_RESERVED 64._MiB
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
		util::test_resource					m_test{ &m_pool, m_data.data(), m_data.size() };

		memory_config() noexcept
		{
			ML_assert(pmr::set_default_resource(&m_test));
			ML_assert(memory_manager::initialize(&m_test));
		}

	} g_memcfg;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// load config
	auto config = ([&temp = json{}, &file = std::ifstream{ CONFIG_FILE }]()
	{
		if (file) { file >> temp; }
		file.close();
		return temp;
	})();

	// create context
	ML_assert(engine::create_context(config)); ML_defer{ ML_assert(engine::destroy_context()); };
	ML_assert(editor::create_context(config)); ML_defer{ ML_assert(editor::destroy_context()); };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// start engine
	ML_assert(engine::startup()); ML_defer{ ML_assert(engine::shutdown()); };
	
	// awake
	engine::run_callback("awake");
	
	// start editor
	ML_assert(editor::startup()); ML_defer{ ML_assert(editor::shutdown()); };

	// load
	engine::run_callback("load");
	event_system::fire_event<load_event>();
	
	// unload
	ML_defer{
		engine::run_callback("unload");
		event_system::fire_event<unload_event>();
	};

	// start
	engine::run_callback("start");

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// main loop
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

	// goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}