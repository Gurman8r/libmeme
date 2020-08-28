#include <libmeme/Core/EventBus.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/PluginManager.hpp>
#include <libmeme/Engine/API_Embed.hpp>

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
	static file_context		fs	{ __argv[0], fs::current_path(), cfg["content"]["path"] };
	static event_bus		bus	{};
	static render_window	win	{};
	static gui_manager		gui	{ &bus };
	static script_context	scr	{ fs.program_name, fs.content_path };
	static system_context	sys	{ &bus, &cfg, &fs, &gui, &mem, &scr, &time, &win };
	static plugin_manager	mods{ &sys };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// window
	ML_assert(win.open(cfg["window"].get<window_settings>()));
	{
		win.set_char_callback([](auto ... x) noexcept { bus.fire<window_char_event>(ML_forward(x)...); });
		win.set_char_mods_callback([](auto ... x) noexcept { bus.fire<window_char_mods_event>(ML_forward(x)...); });
		win.set_close_callback([](auto ... x) noexcept { bus.fire<window_close_event>(ML_forward(x)...); });
		win.set_cursor_enter_callback([](auto ... x) noexcept { bus.fire<window_cursor_enter_event>(ML_forward(x)...); });
		win.set_cursor_position_callback([](auto ... x) noexcept { bus.fire<window_cursor_pos_event>(ML_forward(x)...); });
		win.set_content_scale_callback([](auto ... x) noexcept { bus.fire<window_content_scale_event>(ML_forward(x)...); });
		win.set_drop_callback([](auto ... x) noexcept { bus.fire<window_drop_event>(ML_forward(x)...); });
		win.set_error_callback([](auto ... x) noexcept { bus.fire<window_error_event>(ML_forward(x)...); });
		win.set_focus_callback([](auto ... x) noexcept { bus.fire<window_focus_event>(ML_forward(x)...); });
		win.set_framebuffer_size_callback([](auto ... x) noexcept { bus.fire<window_framebuffer_size_event>(ML_forward(x)...); });
		win.set_iconify_callback([](auto ... x) noexcept { bus.fire<window_iconify_event>(ML_forward(x)...); });
		win.set_key_callback([](auto ... x) noexcept { bus.fire<window_key_event>(ML_forward(x)...); });
		win.set_maximize_callback([](auto ... x) noexcept { bus.fire<window_maximize_event>(ML_forward(x)...);  });
		win.set_mouse_callback([](auto ... x) noexcept { bus.fire<window_mouse_event>(ML_forward(x)...); });
		win.set_position_callback([](auto ... x) noexcept { bus.fire<window_pos_event>(ML_forward(x)...); });
		win.set_refresh_callback([](auto ... x) noexcept { bus.fire<window_refresh_event>(ML_forward(x)...); });
		win.set_scroll_callback([](auto ... x) noexcept { bus.fire<window_scroll_event>(ML_forward(x)...); });
		win.set_size_callback([](auto ... x) noexcept { bus.fire<window_size_event>(ML_forward(x)...); });
	}

	// gui
	ML_assert(gui.startup(win));
	gui.load_style(fs.path2(cfg["gui"]["style"]));
	cfg["gui"]["dockspace"]["visible"].get_to(gui.dockspace.visible);
	cfg["gui"]["dockspace"]["menubar"].get_to(gui.dockspace.menubar);

	// plugins
	for (auto const & path : cfg["plugins"]["files"])
	{
		mods.install(path);
	}

	// scripts
	for (auto const & path : cfg["scripts"]["files"])
	{
		scr.do_file(fs.path2(path));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// loop
	if (!win.is_open()) { return EXIT_FAILURE; }
	bus.fire<load_event>();
	ML_defer(){ bus.fire<unload_event>(); };
	do
	{
		// begin
		time.begin_step();
		ML_benchmark_L("poll_events") { window::poll_events(); };
		
		// update
		ML_benchmark_L("update event") { bus.fire<update_event>(); };
		
		// gui
		ML_benchmark_L("begin gui") { gui.begin_frame(); };
		ML_benchmark_L("gui_event") { bus.fire<gui_event>(); };
		ML_benchmark_L("end gui") { gui.end_frame(); };

		// end
		ML_benchmark_L("swap_buffers") { window::swap_buffers(win); };
		performance::refresh_samples();
		time.end_step();
	}
	while (win.is_open());
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}