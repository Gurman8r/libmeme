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
		"content"	: "../../../../",
		"library"	: "../../../../",
		"guistyle"	: "assets/styles/obsidian.style"
	},
	"window": {
		"title": "libmeme",
		"video": {
			"resolution"	: [ 1280, 720 ],
			"bits_per_pixel": [ 8, 8, 8, 8 ],
			"refresh_rate"	: -1
		},
		"context": {
			"client"		: "opengl",
			"major"			: 4,
			"minor"			: 6,
			"profile"		: "compat",
			"depth_bits"	: 24,
			"stencil_bits"	: 8,
			"multisample"	: true,
			"srgb_capable"	: false
		},
		"hints": {
			"auto_iconify"	: true,
			"decorated"		: true,
			"doublebuffer"	: false,
			"center_cursor"	: false,
			"floating"		: false,
			"focus_on_show"	: true,
			"focused"		: true,
			"maximized"		: true,
			"resizable"		: true,
			"visible"		: false
		}
	},
	"plugins": {
		"files": [
			"plugins/demo"
		]
	},
	"scripts": {
		"files": [
			"assets/scripts/setup.py"
		]
	}
}
)"_json };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto app = alloc_new<application>(std::invoke([&f = std::ifstream{ CONFIG_FILE }]()
	{
		ML_defer(&f) { f.close(); };

		return f ? json::parse(f) : default_settings;
	}));

	ML_defer(&app) { delete app; };
	
	event_bus::fire<load_event>();
	
	ML_defer(){ event_bus::fire<unload_event>(); };

	return app->run([]() noexcept
	{
		ML_benchmark_L("| update"		) { event_bus::fire< update_event		>(); };
		ML_benchmark_L("| begin draw"	) { event_bus::fire< draw_begin_event	>(); };
		ML_benchmark_L("|  draw"		) { event_bus::fire< draw_event			>(); };
		ML_benchmark_L("|  begin gui"	) { event_bus::fire< gui_begin_event	>(); };
		ML_benchmark_L("|   draw gui"	) { event_bus::fire< gui_draw_event		>(); };
		ML_benchmark_L("|  end gui"		) { event_bus::fire< gui_end_event		>(); };
		ML_benchmark_L("| end draw"		) { event_bus::fire< draw_end_event		>(); };
	});

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}