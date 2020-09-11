#include <libmeme/Core/Events.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/API_Embed.hpp>
#include <libmeme/Engine/Application.hpp>

using namespace ml;
using namespace ml::byte_literals;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static class memcfg final : public singleton<memcfg>
{
	friend singleton;

	ds::array<byte_t, 128_MiB>			data{};
	pmr::monotonic_buffer_resource		mono{ data.data(), data.size() };
	pmr::unsynchronized_pool_resource	pool{ &mono };
	passthrough_resource				view{ &pool, data.data(), data.size() };

	memcfg() noexcept { pmr::set_default_resource(&view); }

	~memcfg() noexcept { pmr::set_default_resource(nullptr); }

} const & ML_anon{ memcfg::get_singleton() };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
	"editor": {
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
	static memory			mem	{ pmr::get_default_resource() };
	static io_context		io	{ __argc, __argv, load_settings() };
	static event_bus		bus	{ mem.allocator() };
	static render_window	win	{ mem.allocator() };
	static editor_context	ed	{ &bus, &win, mem.allocator() };
	static script_context	scr	{ io.program_name, io.content_path };
	static system_context	sys	{ &bus, &ed, &io, &mem, &scr, &win };
	static application		app	{ &sys };
	
	return app();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */