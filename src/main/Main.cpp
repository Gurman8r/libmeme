#include <libmeme/Client/PluginManager.hpp>
#include <libmeme/Client/ClientEvents.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Engine/SceneManager.hpp>

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
		"callbacks"		: true,
		"style_path"	: "assets/styles/obsidian.style",
		"menubar"		: { "enabled": true },
		"dockspace"		: { "enabled": true }
	},
	"plugins": {
		"files": [ "plugins/demo" ]
	},
	"scripts": {
		"files": [ "assets/scripts/setup.py" ]
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
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static memory			mem		{ pmr::get_default_resource() };
	static client_io		io		{ __argc, __argv, mem.get_allocator(), load_settings() };
	static data_manager		data	{ io.alloc };
	static event_bus		bus		{ io.alloc };
	static render_window	win		{ io.alloc };
	static gui_manager		gui		{ &bus, &win, io.alloc };
	static py_interpreter	python	{ mem.get_resource(), io.program_name, io.content_path };
	static loop_system		loopsys	{ io.alloc };
	static client_context	context	{ &mem, &io, &data, &bus, &win, &gui, &python, &loopsys };
	static plugin_manager	plugins	{ &context };

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

	ML_assert(gui.startup(io.prefs["gui"]["callbacks"]));
	gui.load_style(io.path2(io.prefs["gui"]["style_path"]));
	gui.get_menubar().configure(io.prefs["gui"]["main_menubar"]);
	gui.get_dockspace().configure(io.prefs["gui"]["main_dockspace"]);

	for (auto const & path : io.prefs["plugins"]["files"]) { plugins.install(path); }
	for (auto const & path : io.prefs["scripts"]["files"]) { python.do_file(io.path2(path)); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	struct client_events final : singleton<client_events>, event_listener
	{
	private:
		friend singleton;

		client_events() noexcept : event_listener{ &bus }
		{
			subscribe<window_key_event>();
			subscribe<window_mouse_event>();
			subscribe<window_cursor_position_event>();

			bus.fire<client_enter_event>(&context);
		}

		~client_events() noexcept override
		{
			bus.fire<client_exit_event>(&context);
		}

		void on_event(event const & value) override
		{
			switch (value)
			{
			case window_key_event::ID: {
				auto const & ev{ (window_key_event const &)value };
				io.input.keys[ev.key] = ev.action;
			} break;

			case window_mouse_event::ID: {
				auto const & ev{ (window_mouse_event const &)value };
				io.input.mouse[ev.button] = ev.action;
			} break;

			case window_cursor_position_event::ID: {
				auto const & ev{ (window_cursor_position_event const &)value };
				io.input.cursor = { ev.x, ev.y };
			} break;
			}
		}
	}
	const & ML_anon{ *client_events::get_singleton() };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	if (!win.is_open()) { return EXIT_FAILURE; }
	do
	{
		io.loop_timer.restart();
		auto const dt{ (float_t)io.delta_time.count() };
		io.fps.accum += dt - io.fps.times[io.fps.index];
		io.fps.times[io.fps.index] = dt;
		io.fps.index = (io.fps.index + 1) % io.fps.times.size();
		io.fps.rate = (0.f < io.fps.accum) ? 1.f / (io.fps.accum / (float_t)io.fps.times.size()) : FLT_MAX;
		ML_defer(&) {
			io.frame_index++;
			io.delta_time = io.loop_timer.elapsed();
		};

		window::poll_events();

		bus.fire<client_update_event>(&context);

		gui.do_frame();

		if (win.has_hints(window_hints_doublebuffer))
		{
			window::swap_buffers(win.get_handle());
		}
	}
	while (win.is_open());
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */