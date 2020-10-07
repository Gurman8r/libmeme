#include <libmeme/Core/Blackboard.hpp>
#include <libmeme/Client/ClientEvents.hpp>
#include <libmeme/Client/ImGuiContext.hpp>
#include <libmeme/Client/PluginManager.hpp>
#include <libmeme/Embed/Python.hpp>
#include <libmeme/Scene/SceneManager.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>

using namespace ml;
using namespace ml::byte_literals;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define RESERVE_MEMORY 128_MiB

static class memcfg final : public singleton<memcfg>
{
	friend singleton;

	ds::array<byte_t, RESERVE_MEMORY>	data{};
	pmr::monotonic_buffer_resource		mono{ data.data(), data.size() };
	pmr::unsynchronized_pool_resource	pool{ &mono };
	passthrough_resource				view{ &pool, data.data(), data.size() };

	memcfg() noexcept { pmr::set_default_resource(&view); }

	~memcfg() noexcept { pmr::set_default_resource(nullptr); }

} const & ML_anon{ *memcfg::get_singleton() };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define SETTINGS_PATH "../../../../libmeme.json"

static auto const default_settings{ R"(
{
	"path": "../../../../",
	"window": {
		"title": "libmeme <3",
		"video": {
			"resolution"	: [ 1280, 720 ],
			"bits_per_pixel": [ 8, 8, 8, 8 ],
			"refresh_rate"	: -1
		},
		"context": {
			"api"			: "opengl",
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
		"callbacks": true,
		"style_path": "assets/styles/obsidian.style",
		"main_menubar": {
			"enabled": true
		},
		"main_dockspace": {
			"enabled"	: true,
			"title"		: "dockspace##libmeme",
			"border"	: 0,
			"rounding"	: 0,
			"alpha"		: 0,
			"padding"	: [ 0, 0 ],
			"size"		: [ 0, 0 ],
			"nodes"		: []
		}
	},
	"plugins": {
		"files": [
			"plugins/demo",
			"plugins/studio"
		]
	},
	"scripts": {
		"files": [
			"assets/scripts/setup.py"
		]
	}
}
)"_json };

static auto load_settings(fs::path const & path = SETTINGS_PATH)
{
	std::ifstream f{ path }; ML_defer(&f) { f.close(); };

	return f ? json::parse(f) : default_settings;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ml::int32_t main()
{
	// context
	static memory			mem		{ pmr::get_default_resource() };
	static client_io		io		{ __argc, __argv, load_settings(), mem.get_allocator() };
	static blackboard		vars	{ io.alloc };
	static event_bus		bus		{ io.alloc };
	static render_window	win		{ io.alloc };
	static imgui_context	imgui	{ &bus, &win, io.alloc };
	static client_context	context	{ &mem, &io, &vars, &bus, &win, &imgui };
	static plugin_manager	mods	{ &context };
	static python_context	python	{ &context };

	// window
	ML_assert(win.open(io.prefs["window"].get<window_settings>()));
	win.set_char_callback([](auto ... x) { bus.fire<window_char_event>(x...); });
	win.set_char_mods_callback([](auto ... x) { bus.fire<window_char_mods_event>(x...); });
	win.set_close_callback([](auto ... x) { bus.fire<window_close_event>(x...); });
	win.set_cursor_enter_callback([](auto ... x) { bus.fire<window_cursor_enter_event>(x...); });
	win.set_cursor_position_callback([](auto ... x) { bus.fire<window_cursor_position_event>(x...); });
	win.set_content_scale_callback([](auto ... x) { bus.fire<window_content_scale_event>(x...); });
	win.set_drop_callback([](auto ... x) { bus.fire<window_drop_event>(x...); });
	win.set_error_callback([](auto ... x) { bus.fire<window_error_event>(x...); });
	win.set_focus_callback([](auto ... x) { bus.fire<window_focus_event>(x...); });
	win.set_framebuffer_resize_callback([](auto ... x) { bus.fire<window_framebuffer_resize_event>(x...); });
	win.set_iconify_callback([](auto ... x) { bus.fire<window_iconify_event>(x...); });
	win.set_key_callback([](auto ... x) { bus.fire<window_key_event>(x...); });
	win.set_maximize_callback([](auto ... x) { bus.fire<window_maximize_event>(x...); });
	win.set_mouse_callback([](auto ... x) { bus.fire<window_mouse_event>(x...); });
	win.set_position_callback([](auto ... x) { bus.fire<window_pos_event>(x...); });
	win.set_refresh_callback([](auto ... x) { bus.fire<window_refresh_event>(x...); });
	win.set_resize_callback([](auto ... x) { bus.fire<window_resize_event>(x...); });
	win.set_scroll_callback([](auto ... x) { bus.fire<window_scroll_event>(x...); });

	// imgui
	ML_assert(imgui.startup(io.prefs["gui"]["callbacks"]));
	imgui.load_style(io.path2(io.prefs["gui"]["style_path"]));
	imgui.get_menubar().configure(io.prefs["gui"]["main_menubar"]);
	imgui.get_dockspace().configure(io.prefs["gui"]["main_dockspace"]);

	// plugins
	for (auto const & path : io.prefs["plugins"]["files"])
	{
		mods.install(path);
	}

	// scripts
	for (auto const & path : io.prefs["scripts"]["files"])
	{
		py::eval_file(io.path2(path).string());
	}

	// main loop
	bus.fire<client_enter_event>(&context);
	ML_defer() { bus.fire<client_exit_event>(&context); };
	if (!win.is_open()) { return EXIT_FAILURE; }
	do
	{
		io.loop_timer.restart();
		auto const dt{ (float_t)io.frame_time.count() };
		io.fps_accum += dt - io.fps_times[io.fps_index];
		io.fps_times[io.fps_index] = dt;
		io.fps_index = (io.fps_index + 1) % io.fps_times.size();
		io.fps_rate = (0.f < io.fps_accum) ? 1.f / (io.fps_accum / (float_t)io.fps_times.size()) : FLT_MAX;
		ML_defer(&) {
			io.frame_count++;
			io.frame_time = io.loop_timer.elapsed();
		};

		window::poll_events();

		bus.fire<client_update_event>(&context);

		imgui.do_frame();

		if (win.has_hints(window_hints_doublebuffer))
		{
			window::swap_buffers(win.get_handle());
		}
	}
	while (win.is_open());
	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */