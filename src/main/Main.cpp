#include <libmeme/System/EventBus.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/Application.hpp>

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

} const & ML_anon{ memcfg::get_singleton() };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef CONFIG_FILE
#define CONFIG_FILE "../../../../libmeme.json"
#endif
 
static auto const default_settings{ R"(
{
    "path": {
        "content": "../../../../",
        "style": "assets/styles/obsidian.style"
    },
    "plugins": {
        "files": [
            "plugins/demo"
        ]
    },
    "scripts": {
        "library": "../../../../",
        "files": [
            "assets/scripts/setup.py"
        ]
    },
    "window": {
        "title": "libmeme",
        "video": {
            "resolution": [ 1280, 720 ],
            "bits_per_pixel": [ 8, 8, 8, 8 ],
            "refresh_rate": -1
        },
        "context": {
            "client": "opengl",
            "major": 4,
            "minor": 6,
            "profile": "compat",
            "depth_bits": 24,
            "stencil_bits": 8,
            "multisample": true,
            "srgb_capable": false
        },
        "hints": {
            "auto_iconify": true,
            "decorated": true,
            "doublebuffer": false,
            "center_cursor": false,
            "floating": false,
            "focus_on_show": true,
            "focused": true,
            "maximized": true,
            "resizable": true,
            "visible": false
        }
    }
}
)"_json };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto app = alloc_new<application>(std::invoke([&f = std::ifstream{ CONFIG_FILE }]()
    {
        ML_defer(&) { f.close(); };

        return f ? json::parse(f) : default_settings;
    }));

    ML_defer(&) { delete app; };
	
	event_bus::fire<load_event>();
	
	ML_defer(&){ event_bus::fire<unload_event>(); };
	
	while (app->window().is_open())
	{
		ML_benchmark_L("| begin loop")	{ event_bus::fire<	begin_loop_event	>(); };
		ML_benchmark_L("|  update")		{ event_bus::fire<	update_event		>(); };
		ML_benchmark_L("|  begin draw")	{ event_bus::fire<	begin_draw_event	>(); };
		ML_benchmark_L("|   draw")		{ event_bus::fire<	draw_event			>(); };
		ML_benchmark_L("|   begin gui")	{ event_bus::fire<	begin_gui_event		>(); };
		ML_benchmark_L("|    draw gui")	{ event_bus::fire<	draw_gui_event		>(); };
		ML_benchmark_L("|   end gui")	{ event_bus::fire<	end_gui_event		>(); };
		ML_benchmark_L("|  end draw")	{ event_bus::fire<	end_draw_event		>(); };
		ML_benchmark_L("| end loop")	{ event_bus::fire<	end_loop_event		>(); };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	return EXIT_SUCCESS; // goodbye!

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}