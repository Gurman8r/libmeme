#include <libmeme/Core/EventBus.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/PluginManager.hpp>

using namespace ml;
using namespace ml::size_literals;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// memory
static class memcfg final : public singleton<memcfg>
{
	friend singleton;

	ds::array<byte_t, 128_MiB>			data{};
	pmr::monotonic_buffer_resource		mono{ data.data(), data.size() };
	pmr::unsynchronized_pool_resource	pool{ &mono };
	arena_test_resource					test{ &pool, data.data(), data.size() };

	memcfg() noexcept { pmr::set_default_resource(&test); }

	~memcfg() noexcept { pmr::set_default_resource(nullptr); }

} const & ML_anon{ memcfg::get_singleton() };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SETTINGS_PATH
#define SETTINGS_PATH "../../../../libmeme.json"
#endif

static auto const default_settings{ R"(
{
	"path": {
		"content"	: "../../../../",
		"library"	: "../../../../",
		"guistyle"	: "assets/styles/obsidian.style"
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

static auto load_settings(fs::path const & path)
{
	std::ifstream f{ path }; ML_defer(&f) { f.close(); };

	return f ? json::parse(f) : default_settings;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// gui window
namespace ml
{
	struct gui_window : render_window
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		gui_window() noexcept : render_window{}
		{
		}

		explicit gui_window(window_settings const & ws) noexcept : gui_window{}
		{
			ML_assert(open(ws));
		}

		virtual ~gui_window() noexcept override
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool open(window_settings const & ws) override
		{
			if (is_open()) { return debug::error("gui_window is already open"); }

			if (!render_window::open(ws)) { return debug::error("failed opening gui_window"); }

			return true;
		}

		virtual void close() override
		{
			render_window::close();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// main
ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// context
	static memory			mem	{ pmr::get_default_resource() };
	static json				cfg	{ load_settings(SETTINGS_PATH) };
	static timer_context	time{};
	static file_context		fs	{ fs::current_path(), cfg["path"]["content"] };
	static event_bus		bus	{};
	static render_window	win	{};
	static gui_manager		gui	{ &bus };
	static system_context	sys	{ &bus, &cfg, &fs, &gui, &mem, &time, &win };
	static plugin_manager	mods{ &sys };
	
	// window
	ML_assert(win.open(cfg["window"].get<window_settings>()));
	{
		win.set_char_callback([](auto ... x) noexcept { bus.fire<char_event>(ML_forward(x)...); });
		win.set_char_mods_callback([](auto ... x) noexcept { bus.fire<char_mods_event>(ML_forward(x)...); });
		win.set_close_callback([](auto ... x) noexcept { bus.fire<close_event>(ML_forward(x)...); });
		win.set_cursor_enter_callback([](auto ... x) noexcept { bus.fire<cursor_enter_event>(ML_forward(x)...); });
		win.set_cursor_position_callback([](auto ... x) noexcept { bus.fire<cursor_position_event>(ML_forward(x)...); });
		win.set_content_scale_callback([](auto ... x) noexcept { bus.fire<content_scale_event>(ML_forward(x)...); });
		win.set_drop_callback([](auto ... x) noexcept { bus.fire<drop_event>(ML_forward(x)...); });
		win.set_error_callback([](auto ... x) noexcept { bus.fire<error_event>(ML_forward(x)...); });
		win.set_focus_callback([](auto ... x) noexcept { bus.fire<focus_event>(ML_forward(x)...); });
		win.set_framebuffer_size_callback([](auto ... x) noexcept { bus.fire<framebuffer_size_event>(ML_forward(x)...); });
		win.set_iconify_callback([](auto ... x) noexcept { bus.fire<iconify_event>(ML_forward(x)...); });
		win.set_key_callback([](auto ... x) noexcept { bus.fire<key_event>(ML_forward(x)...); });
		win.set_maximize_callback([](auto ... x) noexcept { bus.fire<maximize_event>(ML_forward(x)...);  });
		win.set_mouse_callback([](auto ... x) noexcept { bus.fire<mouse_event>(ML_forward(x)...); });
		win.set_position_callback([](auto ... x) noexcept { bus.fire<position_event>(ML_forward(x)...); });
		win.set_refresh_callback([](auto ... x) noexcept { bus.fire<refresh_event>(ML_forward(x)...); });
		win.set_scroll_callback([](auto ... x) noexcept { bus.fire<scroll_event>(ML_forward(x)...); });
		win.set_size_callback([](auto ... x) noexcept { bus.fire<size_event>(ML_forward(x)...); });
	}

	// gui
	ML_assert(gui.startup(win));
	gui.dockspace.visible = true;
	gui.dockspace.menubar = true;
	gui.load_style(fs.path2(cfg["path"]["guistyle"]));

	// plugins
	for (auto const & path : cfg["plugins"]["files"])
	{
		mods.install(path);
	}

	// loop
	if (!win.is_open()) { return EXIT_FAILURE; }
	bus.fire<load_event>();
	ML_defer(){ bus.fire<unload_event>(); };
	do
	{
		// begin
		time.loop_timer.restart();
		time.frame_rate = std::invoke([&, dt = (float_t)time.delta_time.count()]() noexcept
		{
			time.fps_accum += dt - time.fps_times[time.fps_index];
			time.fps_times[time.fps_index] = dt;
			time.fps_index = (time.fps_index + 1) % time.fps_times.size();
			return (0.f < time.fps_accum) ? 1.f / (time.fps_accum / (float_t)time.fps_times.size()) : FLT_MAX;
		});
		
		// update
		window::poll_events();
		for (auto const & cmd : {
			gfx::render_command::set_viewport(win.get_framebuffer_size()),
			gfx::render_command::set_clear_color(colors::black),
			gfx::render_command::clear(gfx::clear_color),
		}) gfx::execute(cmd, win.get_render_context());
		bus.fire<update_event>();
		
		// gui
		gui.begin_frame();
		gui.draw_default();
		bus.fire<gui_event>();
		gui.end_frame();

		// end
		window::swap_buffers(win);
		time.delta_time = time.loop_timer.elapsed();
	}
	while (win.is_open());
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}