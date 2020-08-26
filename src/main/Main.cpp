#include <libmeme/Core/BatchVector.hpp>
#include <libmeme/Core/EventBus.hpp>
#include <libmeme/Core/SharedLibrary.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Graphics/RenderCommand.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/ImGuiExt.hpp>

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

namespace ml
{
	static ds::batch_vector<
		hash_t, fs::path, shared_library, scoped<plugin>
	> m_plugins;

	static plugin_id attach_plugin(fs::path const & path, engine_context * ctx)
	{
		if (path.empty()) { return nullptr; }
		if (auto const code{ util::hash(path.filename().string()) }
		; m_plugins.find<hash_t>(code) == m_plugins.end<hash_t>())
		{
			// load library
			if (shared_library lib{ path })
			{
				// load plugin
				if (auto const ptr{ lib.call<plugin *>(ML_PLUGIN_MAIN, ctx) })
				{
					debug::info("loaded library {0}", path);

					m_plugins.push_back(code, path, std::move(lib), *ptr);

					return ML_handle(plugin_id, code);
				}
			}
		}
		return nullptr;
	}

	static bool detach_plugin(plugin_id value)
	{
		if (!value) { return false; }
		if (auto const it{ m_plugins.find<hash_t>(ML_handle(hash_t, value)) }
		; it != m_plugins.end<hash_t>())
		{
			m_plugins.erase(m_plugins.index_of<hash_t>(it));

			return true;
		}
		else
		{
			return false;
		}
	}

	struct plugin_manager final : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct library_funcs final
		{
			std::function<void(engine_context *, plugin *)> attach;
			std::function<void(engine_context *, plugin *)> detach;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		plugin_manager(engine_context * ctx) noexcept
			: m_ctx{ ctx }, m_data{ ctx->mem->allocator() }
		{
		}

		~plugin_manager() noexcept
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		plugin_id install(fs::path path)
		{
			if (auto const id{ std::invoke([&]() -> plugin_id
			{
				if (path.empty()) { return nullptr; }
				if (path.extension().empty() && !fs::exists(path))
				{
					path += shared_library::default_extension;
				}
				if (shared_library lib{ path })
				{
					library_funcs fun
					{
						lib.getproc<void, engine_context *, plugin *>("ml_plugin_attach"),
						lib.getproc<void, engine_context *, plugin *>("ml_plugin_detach")
					};
					return std::get<0>(m_data.push_back(
						ML_handle(plugin_id, util::hash(path.string())),
						path,
						std::move(lib),
						std::move(fun),
						manual<plugin>{ nullptr }
					));
				}
				return nullptr;
			}) })
			{
				plugin * ptr{};
				if (m_data.back<library_funcs>().attach(m_ctx, ptr); ptr)
				{
					m_data.back<manual<plugin>>().reset(ptr);

					return id;
				}
			}
			return nullptr;
		}

		bool uninstall(plugin_id value)
		{
			if (!value) { return false; }
			if (auto const it{ m_data.find<plugin_id>(ML_handle(plugin_id, value)) }
			; it != m_data.end<plugin_id>())
			{
				auto const i{ m_data.index_of<plugin_id>(it) };

				m_data.at<library_funcs>(i).detach(m_ctx, m_data.at<manual<plugin>>(i).release());

				m_data.erase(i);

				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		engine_context * const m_ctx;
		ds::batch_vector<
			plugin_id		,	// guid
			fs::path		,	// path
			shared_library	,	// library instance
			library_funcs	,	// library functions
			manual<plugin>		// plugin instance
		> m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// memory
	static ds::array<byte_t, 128_MiB>			data{};
	static pmr::monotonic_buffer_resource		mono{ data.data(), data.size() };
	static pmr::unsynchronized_pool_resource	pool{ &mono };
	static test_resource						test{ &pool, data.data(), data.size() };
	pmr::set_default_resource(&test);

	// context
	static memory			mem	{ &test };
	static json				cfg	{ read_config_file(CONFIG_FILE) };
	static timer_context	time{};
	static file_context		fsys{ fs::current_path(), cfg["path"]["content"] };
	static event_bus		bus	{};
	static render_window	win	{};
	static gui_manager		gui	{ &bus };
	static engine_context	ctx	{ &bus, &cfg, &fsys, &gui, &mem, &time, &win };
	static plugin_manager	man	{ &ctx };

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
	{
		gui.dockspace.visible = true;
		gui.dockspace.main_menu.visible = true;
	}

	// plugins
	auto demo{ man.install("plugins/demo") };
	ML_defer(demo) { man.uninstall(demo); };

	// loop
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
		bus.fire<update_event>();

		for (auto const & cmd :
		{
			gfx::render_command::set_clear_color(colors::black),
			gfx::render_command::clear(gfx::clear_color),
			gfx::render_command::set_viewport(win.get_framebuffer_size()),
		})
		gfx::execute(cmd, win.get_render_context());
		bus.fire<draw_event>();

		gui.begin_frame();
		gui.draw_default();
		bus.fire<gui_draw_event>();
		gui.end_frame();

		if ML_UNLIKELY(win.get_hints() & window_hints_doublebuffer)
		{
			window::swap_buffers(win.get_handle());
		}
		time.delta_time = time.loop.elapsed();
	}
	while (win.is_open());
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}