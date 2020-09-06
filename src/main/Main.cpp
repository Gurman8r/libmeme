#include <libmeme/Core/EventBus.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/PluginManager.hpp>
#include <libmeme/Engine/API_Embed.hpp>
#include <libmeme/Engine/Application.hpp>

using namespace ml;
using namespace ml::size_literals;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static class memcfg final : public singleton<memcfg>
{
	friend singleton;

	ds::array<byte_t, 128_MiB>			data{};
	pmr::monotonic_buffer_resource		mono{ data.data(), data.size() };
	pmr::unsynchronized_pool_resource	pool{ &mono };
	passthrough_resource				test{ &pool, data.data(), data.size() };

	memcfg() noexcept { pmr::set_default_resource(&test); }

	~memcfg() noexcept { pmr::set_default_resource(nullptr); }

} const & ML_anon{ memcfg::get_singleton() };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static auto const default_settings{ R"(
{
	"content": {
		"path": "../../../../"
	},
	"window": {
		"title": "libmeme <3",
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
	"gui": {
		"style": "assets/styles/obsidian.style",
		"dockspace": {
			"visible": true,
			"menubar": true
		}
	},
	"plugins": {
		"files": [ "plugins/demo" ]
	},
	"scripts": {
		"files": [ "assets/scripts/setup.py" ]
	}
}
)"_json };

static auto load_settings(fs::path const & path = "../../../../libmeme.json")
{
	std::ifstream f{ path }; ML_defer(&f) { f.close(); };

	return f ? json::parse(f) : default_settings;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// context
	static memory			mem	{ pmr::get_default_resource() };
	static json				cfg	{ load_settings() };
	static timer_context	time{};
	static event_bus		bus	{};
	static io_context		io	{ __argv[0], fs::current_path(), cfg["content"]["path"] };
	static gui_window		win	{};
	static script_context	scr	{ io.program_name, io.content_path };
	static system_context	sys	{ &bus, &cfg, &io, &mem, &scr, &time, &win };
	static application		app	{ &sys };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// open window
	ML_assert(win.open(cfg["window"].get<window_settings>()));
	{
		win.set_char_callback([](auto ... x) noexcept { bus.fire<window_char_event>(x...); });
		win.set_char_mods_callback([](auto ... x) noexcept { bus.fire<window_char_mods_event>(x...); });
		win.set_close_callback([](auto ... x) noexcept { bus.fire<window_close_event>(x...); });
		win.set_cursor_enter_callback([](auto ... x) noexcept { bus.fire<window_cursor_enter_event>(x...); });
		win.set_cursor_position_callback([](auto ... x) noexcept { bus.fire<window_cursor_pos_event>(x...); });
		win.set_content_scale_callback([](auto ... x) noexcept { bus.fire<window_content_scale_event>(x...); });
		win.set_drop_callback([](auto ... x) noexcept { bus.fire<window_drop_event>(x...); });
		win.set_error_callback([](auto ... x) noexcept { bus.fire<window_error_event>(x...); });
		win.set_focus_callback([](auto ... x) noexcept { bus.fire<window_focus_event>(x...); });
		win.set_framebuffer_size_callback([](auto ... x) noexcept { bus.fire<window_framebuffer_size_event>(x...); });
		win.set_iconify_callback([](auto ... x) noexcept { bus.fire<window_iconify_event>(x...); });
		win.set_key_callback([](auto ... x) noexcept { bus.fire<window_key_event>(x...); });
		win.set_maximize_callback([](auto ... x) noexcept { bus.fire<window_maximize_event>(x...);  });
		win.set_mouse_callback([](auto ... x) noexcept { bus.fire<window_mouse_event>(x...); });
		win.set_position_callback([](auto ... x) noexcept { bus.fire<window_pos_event>(x...); });
		win.set_refresh_callback([](auto ... x) noexcept { bus.fire<window_refresh_event>(x...); });
		win.set_scroll_callback([](auto ... x) noexcept { bus.fire<window_scroll_event>(x...); });
		win.set_size_callback([](auto ... x) noexcept { bus.fire<window_size_event>(x...); });
	}

	// configure gui
	win.load_style(io.path2(cfg["gui"]["style"]));
	cfg["gui"]["dockspace"]["visible"].get_to(win.get_dockspace().visible);
	cfg["gui"]["dockspace"]["menubar"].get_to(win.get_dockspace().menubar);

	// load plugins
	for (auto const & path : cfg["plugins"]["files"])
	{
		app.get_mods().install(path);
	}

	// run scripts
	for (auto const & path : cfg["scripts"]["files"])
	{
		scr.do_file(io.path2(path));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// loop
	if (!win.is_open()) { return EXIT_FAILURE; }
	bus.fire<load_event>();
	ML_defer(){ bus.fire<unload_event>(); };
	do
	{
		// timers
		time.begin_step(); ML_defer() { time.end_step(); };

		// update
		ML_benchmark_L("poll")			{ window::poll_events(); };
		ML_benchmark_L("update event")	{ bus.fire<update_event>(); };
		
		// gui
		ML_benchmark_L("begin gui")		{ win.new_frame(&bus); };
		ML_benchmark_L("gui event")		{ bus.fire<gui_event>(); };
		ML_benchmark_L("end gui")		{ win.render_frame(); };
	}
	while (win.is_open());
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}