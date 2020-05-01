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
	static struct memcfg final : non_copyable
	{
		ds::array<byte_t, MEM_RESERVED>		data{};
		pmr::monotonic_buffer_resource		mono{ data.data(), data.size() };
		pmr::unsynchronized_pool_resource	pool{ &mono };
		util::test_resource					test{ &pool, data.data(), data.size() };

		memcfg() noexcept
		{
			ML_assert(pmr::set_default_resource(&test));
			ML_assert(memory_manager::set_test_resource(&test));
		}

	} g_memcfg;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// read config
	auto config = ([&j = json{}, &file = std::ifstream{ CONFIG_FILE }]()
	{
		if (file) { file >> j; }
		file.close();
		return j;
	})();

	// create/destroy context
	ML_assert(engine::create_context(config)); ML_defer{ ML_assert(engine::destroy_context()); };

	// startup/shutdown engine
	ML_assert(engine::startup()); ML_defer{ ML_assert(engine::shutdown()); };

	// load/unload content
	event_system::fire_event<load_event>(); ML_defer{ event_system::fire_event<unload_event>(); };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window isn't open, nothing to do here
	if (!engine::window().is_open())
	{
		return EXIT_SUCCESS;
	}

	do // main loop
	{
		ML_defer{ performance_tracker::refresh(); };
		{
			ML_benchmark("| pre-loop");
			engine::begin_loop();
			event_system::fire_event<pre_loop_event>();
		}
		{
			ML_benchmark("|  update");
			event_system::fire_event<update_event>();
		}
		{
			ML_benchmark("|  pre-draw");
			engine::pre_render();
			event_system::fire_event<pre_draw_event>();
		}
		{
			ML_benchmark("|   draw");
			event_system::fire_event<draw_event>();
		}
		{
			ML_benchmark("|   pre-gui");
			engine::gui().new_frame();
			event_system::fire_event<pre_gui_event>();
		}
		{
			ML_benchmark("|    gui-draw");
			engine::gui().draw();
			event_system::fire_event<gui_draw_event>();
		}
		{
			ML_benchmark("|   post-gui");
			engine::gui().end_frame();
			event_system::fire_event<post_gui_event>();
		}
		{
			ML_benchmark("|  post-draw");
			engine::post_render();
			event_system::fire_event<post_draw_event>();
		}
		{
			ML_benchmark("|  late update");
			event_system::fire_event<late_update_event>();
		}
		{
			ML_benchmark("| post-loop");
			engine::end_loop();
			event_system::fire_event<post_loop_event>();
		}
	} while (engine::window().is_open());
	
	// goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}