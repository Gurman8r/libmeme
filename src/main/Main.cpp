#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/ScopeGuard.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>

#include <libmeme/Engine/ECS.hpp>

namespace ml
{
	// components
	struct C0 { static constexpr auto name = "C0"sv; };
	struct C1 { static constexpr auto name = "C1"sv; };
	struct C2 { static constexpr auto name = "C2"sv; };
	struct C3 { static constexpr auto name = "C3"sv; };
	struct C4 { static constexpr auto name = "C4"sv; };

	// tags
	struct T0 {};
	struct T1 {};
	struct T2 {};

	// signatures
	using S0 = meta::type_list<>;				// 00000000
	using S1 = meta::type_list<C0, C1>;			// 11000000
	using S2 = meta::type_list<C0, C4, T0>;		// 10001100
	using S3 = meta::type_list<C1, T0, C3, T2>;	// 01010101

	int32_t ecs_test()
	{
		using ES = ecs::settings<
			ecs::component_config	<C0, C1, C2, C3, C4>,
			ecs::tag_config			<T0, T1, T2>,
			ecs::signature_config	<S0, S1, S2, S3>
		>;

		static_assert(ES::components::count() == 5);
		static_assert(ES::signatures::count() == 4);
		static_assert(ES::tags		::count() == 3);

		static_assert(ES::components::index<C0>() == 0);
		static_assert(ES::components::index<C1>() == 1);
		static_assert(ES::components::index<C2>() == 2);
		static_assert(ES::components::index<C3>() == 3);
		static_assert(ES::components::index<C4>() == 4);
		static_assert(ES::tags		::index<T0>() == 0);
		static_assert(ES::tags		::index<T1>() == 1);
		static_assert(ES::tags		::index<T2>() == 2);
		static_assert(ES::signatures::index<S0>() == 0);
		static_assert(ES::signatures::index<S1>() == 1);
		static_assert(ES::signatures::index<S2>() == 2);
		static_assert(ES::signatures::index<S3>() == 3);

		static_assert(ES::component_bit<C0>() == 0);
		static_assert(ES::component_bit<C1>() == 1);
		static_assert(ES::component_bit<C2>() == 2);
		static_assert(ES::component_bit<C3>() == 3);
		static_assert(ES::component_bit<C4>() == 4);
		static_assert(ES::		tag_bit<T0>() == 5);
		static_assert(ES::		tag_bit<T1>() == 6);
		static_assert(ES::		tag_bit<T2>() == 7);

		static_assert(ES::get_bitmask<S0>() == "00000000");
		static_assert(ES::get_bitmask<S1>() == "11000000");
		static_assert(ES::get_bitmask<S2>() == "10001100");
		static_assert(ES::get_bitmask<S3>() == "01010101");

		static_assert(std::is_same_v<ES::component_signatures<S0>,
			meta::type_list<>
		>);
		static_assert(std::is_same_v<ES::component_signatures<S3>,
			meta::type_list<C1, C3>
		>);
		static_assert(std::is_same_v<ES::tag_signatures<S3>,
			meta::type_list<T0, T2>
		>);

		ecs::manager<ES> man{};
		man.resize(100);

		if (auto e = man.create_handle())
		{
			e.add_component<C0>();
			e.add_component<C1>();
		}

		if (auto e = man.create_handle())
		{
			e.add_component<C0>();
			e.add_component<C1>();
			e.add_component<C2>();
			e.add_component<C4>();
		}

		if (auto e = man.create_handle())
		{
			e.add_component<C0>();
			e.add_component<C4>();
			e.add_tag<T0>();
		}
		
		man.refresh();

		std::cout << "S1:\n";
		man.for_matching<S1>([&](size_t i, C0 & c0, C1 & c1)
		{
			std::cout << " " << c0.name << ' ' << c1.name << '\n';
		});

		std::cout << "S2:\n";
		man.for_matching<S2>([&](size_t i, C0 & c0, C4 & c4)
		{
			std::cout << " " << c0.name << ' ' << c4.name << '\n';
		});

		return debug::pause(0);
	}
}

ml::int32_t main()
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using namespace ml;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// setup memory
	static auto g_raw = ds::array<byte_t, 64._MiB>{};
	static auto g_buf = pmr::monotonic_buffer_resource{ g_raw.data(), g_raw.size() };
	static auto g_pool = pmr::unsynchronized_pool_resource{ &g_buf };
	pmr::set_default_resource(&g_pool);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// engine configuration
	if (!engine::create_context())
	{
		return debug::log_error("failed creating engine context") | debug::pause(1);
	}
	else
	{
		engine::config & config	= engine::get_config();
		config.program_name		= ML_ARGV[0];
		config.library_path		= "../../../"s;
		config.script_list		= { "../../../libmeme.py"s };
		config.plugin_list		= {};
		config.window_title		= "libmeme";
		config.window_flags		= WindowFlags_Default;
		config.window_video		= make_video_mode(
			vec2u{ 1280, 720 },	// resolution
			32u					// color depth
		);
		config.window_context	= make_context_settings(
			client_api::OpenGL,	// api
			4,					// major version
			6,					// minor version
			client_api::Compat,	// profile
			24,					// depth bits
			8,					// stencil bits
			false,				// multisample
			false				// sRGB capable
		);
	}

	// start engine
	if (!engine::startup(true))
	{
		return debug::log_error("failed starting engine") | debug::pause(1);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// editor configuration
	if (!editor::create_context())
	{
		return debug::log_error("failed creating editor context") | debug::pause(1);
	}
	else
	{
		editor::config & config	= editor::get_config();
		config.window_handle	= engine::get_window().get_handle();
		config.api_version		= "#version 130";
		config.style_config		= "dark";
		config.ini_file			= "";
		config.log_file			= "";
	}

	// start editor
	if (!editor::startup(true))
	{
		return debug::log_error("failed starting editor") | debug::pause(1);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// enter event
	event_system::fire_event<enter_event>(ML_ARGC, ML_ARGV);

	// main loop
	while (engine::running())
	{
		ML_DEFER{ performance_tracker::refresh(); };

		// begin loop
		{
			ML_BENCHMARK("LOOP_BEGIN");
			engine::begin_loop();
			event_system::fire_event<begin_loop_event>();
		}
		// update
		{
			ML_BENCHMARK("\tUPDATE");
			event_system::fire_event<update_event>();
		}
		// draw
		{
			ML_BENCHMARK("\tDRAW");
			engine::begin_draw();
			event_system::fire_event<draw_event>();
		}
		// begin gui
		{
			ML_BENCHMARK("\tGUI_BEGIN");
			editor::new_frame();
			editor::get_main_menu().render();
			editor::get_dockspace().render();
			event_system::fire_event<begin_gui_event>();
		}
		// gui
		{
			ML_BENCHMARK("\t\tGUI");
			event_system::fire_event<gui_event>();
		}
		// end gui
		{
			ML_BENCHMARK("\tGUI_END");
			editor::render_frame();
			event_system::fire_event<end_gui_event>();
		}
		// end loop
		{
			ML_BENCHMARK("LOOP_END");
			engine::end_loop();
			event_system::fire_event<end_loop_event>();
		}
	}

	// exit event
	event_system::fire_event<exit_event>();

	// shutdown
	editor::shutdown();
	engine::shutdown();

	// goodbye!
	return EXIT_SUCCESS;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}