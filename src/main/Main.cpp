#include <libmeme/System/EventBus.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>
#include <libmeme/Window/WindowEvents.hpp>

using namespace ml;
using namespace ml::size_literals;

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

namespace ml
{
	static auto read_config_file(fs::path const & path)
	{
		std::ifstream f{ path };
		ML_defer(&f) { f.close(); };
		return f ? json::parse(f) : default_settings;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ds::array<byte_t, 128_MiB>			data{};
	static pmr::monotonic_buffer_resource		mono{ data.data(), data.size() };
	static pmr::unsynchronized_pool_resource	pool{ &mono };
	static test_resource						test{ &pool, data.data(), data.size() };

	static memory			mem	{ &test };
	static timer_context	time{};
	static json				cfg	{ read_config_file(CONFIG_FILE) };
	static file_context		fsys{ fs::current_path(), cfg["path"]["content"] };
	static event_bus		bus	{};
	static render_window	win	{};
	static gui_manager		gui	{ &bus };
	static engine_context	ctx
	{
		&bus, &cfg, &fsys, &gui, &mem, &time, &win
	};
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
	ML_assert(gui.startup(win));
	{
		gui.dockspace().visible = true;
		gui.main_menu_bar().visible = true;
	}
	if (!win.is_open()) { return EXIT_FAILURE; }
	bus.fire<load_event>();
	ML_defer(&){ bus.fire<unload_event>(); };
	do
	{
		time.loop.restart();
		time.frame_rate = std::invoke([&, dt = (float_t)time.delta_time.count()]() noexcept
		{
			time.fps_accum += dt - time.fps_times[time.fps_index];
			time.fps_times[time.fps_index] = dt;
			time.fps_index = (time.fps_index + 1) % time.fps_times.size();
			return (0.f < time.fps_accum) ? 1.f / (time.fps_accum / (float_t)time.fps_times.size()) : FLT_MAX;
		});
		window::poll_events();
		for (auto const & cmd :
		{
			gfx::render_command::set_clear_color(colors::black),
			gfx::render_command::clear(gfx::clear_color),
			gfx::render_command::set_viewport(win.get_framebuffer_size()),
		})
		gfx::execute(cmd, win.get_render_context());
		gui.begin_frame();
		gui.draw_default();
		gui.end_frame();
		if ML_UNLIKELY(win.get_hints() & window_hints_doublebuffer)
		{
			window::swap_buffers(win.get_handle());
		}
	}
	while (win.is_open());
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}