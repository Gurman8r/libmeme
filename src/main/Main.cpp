#include <libmeme/Client/Application.hpp>

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

} const & ML_anon{ memcfg::get_singleton() };

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
		"instclbk"	: true,
		"style"		: "assets/styles/obsidian.style",
		"menubar"	: { "enabled": true },
		"dockspace"	: { "enabled": true }
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
	static memory			mem	{ pmr::get_default_resource() };
	static io_context		io	{ __argc, __argv, mem.get_allocator(), load_settings() };
	static event_bus		bus	{ io.alloc };
	static python_context	scr	{ io.program_name, io.content_path, mem.get_resource() };
	static render_window	win	{ io.alloc };
	static gui_manager		gui	{ &bus, &win, io.alloc };
	static system_context	sys	{ &mem, &io, &bus, &scr, &win, &gui };
	static application		app	{ &sys };

	for (auto const & path : io.prefs["plugins"]["files"]) { app.install_plugin(path); }
	for (auto const & path : io.prefs["scripts"]["files"]) { scr.do_file(io.path2(path)); }
	
	return app();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */