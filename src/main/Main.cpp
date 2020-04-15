#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>

using namespace ml;

#ifndef MEM_RESERVED
#define MEM_RESERVED 128._MiB
#endif

#ifndef CONFIG_FILE
#define CONFIG_FILE L"../../../../assets/libmeme.json"
#endif

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// setup memory
	static struct memory_config final : non_copyable
	{
		ds::array<byte_t, MEM_RESERVED>		data{};
		pmr::monotonic_buffer_resource		mono{ data.data(), data.size() };
		pmr::unsynchronized_pool_resource	pool{ &mono };
		util::test_resource					test{ &pool, data.data(), data.size() };

		memory_config() noexcept
		{
			ML_assert(pmr::set_default_resource(&test));
			ML_assert(memory_manager::set_test_resource(&test));
		}

	} g_memcfg;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// read config file
	auto config = ([&j = json{}, &file = std::ifstream{ CONFIG_FILE }]() noexcept
	{
		if (file) { file >> j; }
		file.close();
		return j;
	})();

	// create engine context
	ML_assert(engine::create_context(config)); ML_defer{ ML_assert(engine::destroy_context()); };

	// startup engine
	ML_assert(engine::startup()); ML_defer{ ML_assert(engine::shutdown()); };

	// nothing to do here
	if (!engine::window().is_open()) { return EXIT_SUCCESS; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	event_system::fire_event<load_event>();

	ML_defer{ event_system::fire_event<unload_event>(); };

	// main loop
	while (engine::window().is_open())
	{
		ML_defer{ performance_tracker::refresh(); };
		{
			ML_benchmark("| begin loop");
			engine::begin_loop();
			event_system::fire_event<loop_begin_event>();
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
			engine::gui().new_frame();
			event_system::fire_event<gui_begin_event>();
		}
		{
			ML_benchmark("|    gui");
			engine::gui().render();
			event_system::fire_event<gui_draw_event>();
		}
		{
			ML_benchmark("|   end gui");
			engine::gui().render_frame();
			event_system::fire_event<gui_end_event>();
		}
		{
			ML_benchmark("|  end draw");
			engine::end_draw();
			event_system::fire_event<end_draw_event>();
		}
		{
			ML_benchmark("| end loop");
			engine::end_loop();
			event_system::fire_event<loop_end_event>();
		}
	}
	
	// goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}