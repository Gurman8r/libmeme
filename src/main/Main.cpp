#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>

using namespace ml;

#ifndef MAIN_MEMORY
#define MAIN_MEMORY 128.0_MiB
#endif

#ifndef MAIN_CONFIG
#define MAIN_CONFIG L"../../../../assets/libmeme.json"
#endif

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// setup memory
	static struct memcfg final
	{
		ds::array<byte_t, MAIN_MEMORY>		data{};
		pmr::monotonic_buffer_resource		mono{ data.data(), data.size() };
		pmr::unsynchronized_pool_resource	pool{ &mono };
		util::test_resource					test{ &pool, data.data(), data.size() };

		memcfg() noexcept
		{
			ML_assert(pmr::set_default_resource(&test));
			ML_assert(memory_manager::configure(&test));
		}

	} ML_anon;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// load config
	auto config = ([&j = json{}, &f = std::ifstream{ MAIN_CONFIG }]()
	{
		if (f) { f >> j; }
		f.close();
		return j;
	})();

	// create/destroy context
	ML_assert(engine::create_context(config)); ML_defer{ ML_assert(engine::destroy_context()); };

	// startup/shutdown
	ML_assert(engine::startup()); ML_defer{ ML_assert(engine::shutdown()); };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// nothing to do
	if (!engine::window().is_open()) { return EXIT_SUCCESS; }

	// load content
	event_system::fire_event<load_event>();

	// unload content
	ML_defer{ event_system::fire_event<unload_event>(); };

	// main loop
	while (engine::window().is_open())
	{
		ML_defer{ performance_tracker::swap(); };
		{
			ML_benchmark("| begin loop");
			engine::time().begin_loop();
			engine::window().poll_events();
			event_system::fire_event<begin_loop_event>();
		}
		{
			ML_benchmark("|  update");
			event_system::fire_event<update_event>();
		}
		{
			ML_benchmark("|  begin draw");
			engine::window().clear_color(colors::black);
			engine::window().viewport(engine::window().get_framebuffer_size());
			event_system::fire_event<begin_draw_event>();
		}
		{
			ML_benchmark("|   draw");
			event_system::fire_event<draw_event>();
		}
		{
			ML_benchmark("|   begin gui");
			engine::gui().begin_frame();
			event_system::fire_event<begin_gui_event>();
		}
		{
			ML_benchmark("|    draw gui");
			engine::gui().draw_builtin();
			event_system::fire_event<draw_gui_event>();
		}
		{
			ML_benchmark("|   end gui");
			engine::gui().end_frame();
			event_system::fire_event<end_gui_event>();
		}
		{
			ML_benchmark("|  end draw");
			engine::window().swap_buffers();
			event_system::fire_event<end_draw_event>();
		}
		{
			ML_benchmark("|  late update");
			event_system::fire_event<late_update_event>();
		}
		{
			ML_benchmark("| end loop");
			engine::time().end_loop();
			event_system::fire_event<end_loop_event>();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}