#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>

using namespace ml;
using namespace ml::size_literals;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static class memcfg : public singleton<memcfg>
{
	friend singleton;

	ds::array<byte_t, 120_MiB>			data{};
	pmr::monotonic_buffer_resource		mono{ data.data(), data.size() };
	pmr::unsynchronized_pool_resource	pool{ &mono };
	util::test_resource					test{ &pool, data.data(), data.size() };

	memcfg()
	{
		ML_assert(pmr::set_default_resource(&test));
		ML_assert(memory::set_test_resource(&test));
	}

} &ML_anon{ memcfg::get_instance() };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef CONFIG_FILE
#define CONFIG_FILE "../../../../assets/libmeme.json"
#endif

static auto const default_settings{ R"(
{
	"content_home"	: "../../../../",
	"library_home"	: "../../../../",
	"setup_script"	: "assets/scripts/setup.py"
}
)"_json };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD ctx_config final // testing
	{
		int32_t	client		{ 0 };
		int32_t major		{ 1 },
				minor		{ 0 },
				revision	{ 0 };
		int32_t profile		{ 0 };
		int32_t source		{ 0 },
				robustness	{ 0 },
				release		{ 0 };
		bool	forward		{ 0 },
				debug		{ 0 },
				noerror		{ 0 };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// setup engine
	ML_assert(engine::initialize(std::invoke([&j = json{}, &f = std::ifstream{ CONFIG_FILE }]()
	{
		ML_defer(&){ f.close(); };
		if (f) { f >> j; } else { j = default_settings; }
		return j;
	})));
	
	ML_defer(&){ engine::finalize(); };

	if (engine::config().contains("setup_script"))
	{
		engine::scripts().do_file(
			engine::fs().path2(
				engine::config()["setup_script"]));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// application sequence
	if (!engine::window().is_open())
	{
		return EXIT_FAILURE;
	}
	
	event_system::fire_event<load_event>();
	
	ML_defer(&){ event_system::fire_event<unload_event>(); };
	
	while (engine::window().is_open())
	{
		engine::time().begin_loop(); ML_defer(&){ engine::time().end_loop(); };

		ML_benchmark_L("| begin loop")	{ event_system::fire_event<	begin_loop_event	>(); };
		ML_benchmark_L("|  update")		{ event_system::fire_event<	update_event		>(); };
		ML_benchmark_L("|  begin draw")	{ event_system::fire_event<	begin_draw_event	>(); };
		ML_benchmark_L("|   draw")		{ event_system::fire_event<	draw_event			>(); };
		ML_benchmark_L("|   begin gui")	{ event_system::fire_event<	begin_gui_event		>(); };
		ML_benchmark_L("|    draw gui")	{ event_system::fire_event<	draw_gui_event		>(); };
		ML_benchmark_L("|   end gui")	{ event_system::fire_event<	end_gui_event		>(); };
		ML_benchmark_L("|  end draw")	{ event_system::fire_event<	end_draw_event		>(); };
		ML_benchmark_L("| end loop")	{ event_system::fire_event<	end_loop_event		>(); };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	return EXIT_SUCCESS; // goodbye!

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}