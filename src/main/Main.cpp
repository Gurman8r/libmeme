#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>

using namespace ml;
using namespace ml::size_literals;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MEM_RESERVED
#define MEM_RESERVED 128_MiB
#endif

#ifndef CONFIG_PATH
#define CONFIG_PATH "../../../../assets/libmeme.json"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class memcfg final : public singleton<memcfg>
{
	friend singleton<memcfg>;

	ds::array<byte_t, MEM_RESERVED>		data{};
	pmr::monotonic_buffer_resource		mono{ data.data(), data.size() };
	pmr::unsynchronized_pool_resource	pool{ &mono };
	util::test_resource					test{ &pool, data.data(), data.size() };

	memcfg() noexcept
	{
		ML_assert(pmr::set_default_resource(&test));
		ML_assert(memory_manager::configure(&test));
	}

} &ML_anon{ memcfg::get_instance() };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static auto const default_settings{ R"(
{
	"content_home": "../../../../",
	"library_home": "../../../../",
	"setup_script": "assets/scripts/setup.py"
}
)"_json };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// setup engine
	ML_assert(engine::initialize(([&j = json{}, &f = std::ifstream{ CONFIG_PATH }]()
	{
		ML_defer{ f.close(); };
		if (f) { f >> j; } else { j = default_settings; }
		return j;
	})()));
	ML_defer{ ML_assert(engine::finalize()); };
	if (!engine::window().is_open()) { return EXIT_FAILURE; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// application loop
	event_system::fire_event<load_event>();
	ML_defer{ event_system::fire_event<unload_event>(); };
	while (engine::window().is_open())
	{
		engine::time().begin_loop();
		ML_defer{ engine::time().end_loop(); };
		ML_defer{ performance_tracker::swap(); };
		{
			ML_benchmark("| begin loop");
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
			engine::gui().new_frame();
			event_system::fire_event<begin_gui_event>();
		}
		{
			ML_benchmark("|    draw gui");
			engine::gui().draw_default();
			event_system::fire_event<draw_gui_event>();
		}
		{
			ML_benchmark("|   end gui");
			engine::gui().render_frame();
			event_system::fire_event<end_gui_event>();
		}
		{
			ML_benchmark("|  end draw");
			engine::window().swap_buffers();
			event_system::fire_event<end_draw_event>();
		}
		{
			ML_benchmark("| end loop");
			event_system::fire_event<end_loop_event>();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	return EXIT_SUCCESS; // goodbye!

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}