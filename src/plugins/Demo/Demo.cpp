/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/ECS.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/RAII_Pointer.hpp>
#include <libmeme/Core/StreamSniper.hpp>
#include <libmeme/Core/Wrapper.hpp>
#include <libmeme/Engine/API_Embed.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/ImGuiExt.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Graphics/Binder.hpp>
#include <libmeme/Graphics/Font.hpp>
#include <libmeme/Graphics/Model.hpp>
#include <libmeme/Graphics/RenderTexture.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// ECS CONFIG
namespace ml
{
	// (T) TAGS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct t_renderer {};

	// (C) COMPONENTS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct c_shader		: ds::wrapper<shader>	{};
	struct c_material	: ds::wrapper<material> {};
	struct c_model		: ds::wrapper<model>	{};
	struct c_transform	{ vec3 pos; vec4 rot; vec3 scl; };


	// (S) SIGNATURES
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using s_apply_transforms = meta::list<c_material, c_transform
	>;
	using s_apply_materials = meta::list<c_shader, c_material
	>;
	using s_draw_renderers = meta::list<c_shader, c_model
	>;


	// (X) SYSTEMS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class> struct x_apply_transforms final : ecs::detail::x_base<s_apply_transforms>
	{
		void operator()(c_material & mat, c_transform const & tf)
		{
			mat->set<vec3>("u_position", tf.pos)
				.set<vec4>("u_rotation", tf.rot)
				.set<vec3>("u_scale", tf.scl);
		}
	};

	template <class> struct x_apply_materials final : ecs::detail::x_base<s_apply_materials>
	{
		void operator()(c_shader & shd, c_material const & mat)
		{
			ML_bind_scope(*shd, false);
			for (uniform const & u : *mat)
			{
				shd->set_uniform(u);
			}
		}
	};

	template <class> struct x_draw_renderers final : ecs::detail::x_base<s_draw_renderers>
	{
		void operator()(c_shader const & shd, c_model const & mdl, render_target & target)
		{
			ML_bind_scope(*shd, true);
			target.draw(*mdl);
		}
	};


	// (U) TRAITS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using entity_traits = ecs::traits<

		// tags
		ecs::cfg::tags<
		t_renderer
		>,

		// components
		ecs::cfg::components<
		c_shader, c_material, c_model, c_transform
		>,

		// signatures
		ecs::cfg::signatures<
		s_apply_transforms, s_apply_materials, s_draw_renderers
		>,

		// systems
		ecs::cfg::systems<
		x_apply_transforms, x_apply_materials, x_draw_renderers
		>
	>;


	// (M) MANAGER
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using entity_manager = ecs::manager<entity_traits>;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// DEMO
namespace ml
{
	struct demo final : plugin
	{
		// ASSETS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ds::map<int32_t,		render_texture	> m_pipeline	{};
		ds::map<pmr::string,	font			> m_fonts		{};
		ds::map<pmr::string,	image			> m_images		{};
		ds::map<pmr::string,	material		> m_materials	{};
		ds::map<pmr::string,	model			> m_models		{};
		ds::map<pmr::string,	shader			> m_shaders		{};
		ds::map<pmr::string,	texture			> m_textures	{};


		// ECS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		entity_manager m_ecs{};


		// GUI
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		gui::widget
			m_imgui_demo	{ "Dear ImGui Demo"		, 0, "", ImGuiWindowFlags_None },
			m_imgui_metrics	{ "Dear ImGui Metrics"	, 0, "", ImGuiWindowFlags_None },
			m_imgui_about	{ "About Dear ImGui"	, 0, "", ImGuiWindowFlags_None },
			m_gui_assets	{ "assets##demo"		, 1, "", ImGuiWindowFlags_None },
			m_gui_console	{ "console##demo"		, 1, "", ImGuiWindowFlags_None },
			m_gui_display	{ "display##demo"		, 1, "", ImGuiWindowFlags_NoScrollbar },
			m_gui_docs		{ "documents##demo"		, 1, "", ImGuiWindowFlags_MenuBar },
			m_gui_ecs		{ "ecs##demo"			, 1, "", ImGuiWindowFlags_None },
			m_gui_files		{ "files##demo"			, 0, "", ImGuiWindowFlags_MenuBar },
			m_gui_memory	{ "memory##demo"		, 1, "", ImGuiWindowFlags_MenuBar },
			m_gui_nodes		{ "node editor##demo"	, 1, "", ImGuiWindowFlags_None },
			m_gui_profiler	{ "profiler##demo"		, 1, "", ImGuiWindowFlags_None },
			m_gui_scripting	{ "scripting##demo"		, 0, "", ImGuiWindowFlags_MenuBar };

		vec2 m_display_size{};

		stream_sniper m_cout{ &std::cout };

		gui::console m_console{};

		raii_ptr<ax::NodeEditor::EditorContext> m_node_editor
		{
			[]() { return ax::NodeEditor::CreateEditor(); },
			&ax::NodeEditor::DestroyEditor
		};


		gui::plot_controller m_plots
		{ {
			gui::plot::create(120, gui::plot::histogram
				, "##frame time"
				, "%.3f ms/frame"
				, vec2{ 0.f, 64.f }
				, vec2{ FLT_MAX, FLT_MAX }
				, []() { return engine::time().delta().count<float_t>() * 1000.f; }
			),
			gui::plot::create(120, gui::plot::histogram
				, "##frame rate"
				, "%.3f fps"
				, vec2{ 0.f, 64.f }
				, vec2{ FLT_MAX, FLT_MAX }
				, []() { return (float_t)engine::time().frame_rate(); }
			),
		} };

		MemoryEditor m_mem_editor{};

		void highlight_memory(byte_t * ptr, size_t const size)
		{
			static auto const & testres{ memory_manager::get_testres() };
			auto const addr{ std::distance(testres->begin(), ptr) };
			m_gui_memory.set_focused();
			m_mem_editor.GotoAddrAndHighlight((size_t)addr, (size_t)addr + size);
		}

		template <class T> void highlight_memory(T const * ptr)
		{
			highlight_memory((byte_t *)ptr, sizeof(T));
		}


		// DEMO
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		demo() noexcept
		{
			event_system::add_listener<	load_event		>(this);
			event_system::add_listener<	update_event	>(this);
			event_system::add_listener<	draw_event		>(this);
			event_system::add_listener<	dock_gui_event	>(this);
			event_system::add_listener<	draw_gui_event	>(this);
			event_system::add_listener<	unload_event	>(this);
		}

		void on_event(event const & ev) override
		{
			switch (ev.ID)
			{
			case hashof_v<load_event	>: return on_load		(*ev.cast<load_event	>());
			case hashof_v<update_event	>: return on_update		(*ev.cast<update_event	>());
			case hashof_v<draw_event	>: return on_draw		(*ev.cast<draw_event	>());
			case hashof_v<dock_gui_event>: return on_gui_dock	(*ev.cast<dock_gui_event>());
			case hashof_v<draw_gui_event>: return on_gui_draw	(*ev.cast<draw_gui_event>());
			case hashof_v<unload_event	>: return on_unload		(*ev.cast<unload_event	>());
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_load(load_event const &)
		{
			// load stuff, etc...

			// ICON
			if (auto icon{ make_image(engine::fs().path2("assets/textures/icon.png")) })
			{
				engine::window().set_icon(icon.width(), icon.height(), icon.data());
			}

			// PIPELINE
			{
				(m_pipeline[0] = make_render_texture(vec2i{ 1280, 720 })).generate();
			}

			// IMAGES
			{
				m_images["default"] = make_image(image::default_rgba);
			}

			// TEXTURES
			{
				m_textures["default"] = make_texture(m_images["default"]);

				m_textures["doot"] = make_texture(
					engine::fs().path2("assets/textures/doot.png")
				);

				m_textures["navball"] = make_texture(
					engine::fs().path2("assets/textures/navball.png")
				);

				m_textures["earth_dm_2k"] = make_texture(
					engine::fs().path2("assets/textures/earth/earth_dm_2k.png")
				);

				m_textures["earth_sm_2k"] = make_texture(
					engine::fs().path2("assets/textures/earth/earth_sm_2k.png")
				);

				m_textures["moon_dm_2k"] = make_texture(
					engine::fs().path2("assets/textures/moon/moon_dm_2k.png")
				);
			}

			// FONTS
			{
				m_fonts["clacon"] = make_font(
					engine::fs().path2("assets/fonts/clacon.ttf")
				);

				m_fonts["consolas"] = make_font(
					engine::fs().path2("assets/fonts/consolas.ttf")
				);

				m_fonts["lucida_console"] = make_font(
					engine::fs().path2("assets/fonts/lucida_console.ttf")
				);

				m_fonts["minecraft"] = make_font(
					engine::fs().path2("assets/fonts/minecraft.ttf")
				);
			}

			// SHADERS
			{
				m_shaders["2d"] = make_shader(
					engine::fs().path2("assets/shaders/2D.vs.shader"),
					engine::fs().path2("assets/shaders/basic.fs.shader")
				);

				m_shaders["3d"] = make_shader(
					engine::fs().path2("assets/shaders/3D.vs.shader"),
					engine::fs().path2("assets/shaders/basic.fs.shader")
				);
			}

			// MATERIALS
			{
				// timers
				auto const _timers = make_material(
					make_uniform<float_t>("u_time"	, []() { return engine::time().total().count<float_t>(); }),
					make_uniform<float_t>("u_delta"	, []() { return engine::time().delta().count<float_t>(); })
				);

				// MVP
				auto const _mvp = make_material(
					make_uniform<mat4	>("u_model"		, mat4::identity()),
					make_uniform<mat4	>("u_view"		, mat4::identity()),
					make_uniform<mat4	>("u_proj"		, mat4::identity())
				);

				// camera
				auto const _camera = make_material(
					make_uniform<vec3	>("u_cam.pos"	, vec3{ 0, 0, 3.f }),
					make_uniform<vec3	>("u_cam.dir"	, vec3{ 0, 0, -1.f }),
					make_uniform<float_t>("u_cam.fov"	, 45.0f),
					make_uniform<float_t>("u_cam.near"	, 0.0001f),
					make_uniform<float_t>("u_cam.far"	, 1000.0f),
					make_uniform<vec2	>("u_cam.view"	, vec2{ 1280.f, 720.f })
				);

				// transform
				auto const _tf = make_material(
					make_uniform<vec3	>("u_position"	, vec3{}),
					make_uniform<vec4	>("u_rotation"	, vec4{}),
					make_uniform<vec3	>("u_scale"		, vec3{})
				);

				// 2d
				m_materials["2d"] = make_material(
					make_uniform<color	>("u_color"		, colors::white),
					make_uniform<texture>("u_texture0"	, &m_textures["doot"])
				) + _timers + _mvp;

				// 3d
				m_materials["3d"] = make_material(
					make_uniform<color	>("u_color",	colors::white),
					make_uniform<texture>("u_texture0", &m_textures["earth_dm_2k"])
					//, make_uniform<texture>("u_texture1", &m_textures["earth_sm_2k"])
				) + _timers + _camera + _tf;

				// test
				m_materials["moon"] = (make_material() + m_materials["3d"])
					.set<texture>("u_texture0", &m_textures["moon_dm_2k"]);
			}

			// MODELS
			{
				m_models["sphere8x6"] = make_model(
					engine::fs().path2("assets/models/sphere8x6.obj")
				);

				m_models["sphere32x24"] = make_model(
					engine::fs().path2("assets/models/sphere32x24.obj")
				);

				m_models["monkey"] = make_model(
					engine::fs().path2("assets/models/monkey.obj")
				);

				m_models["triangle"] = make_model(make_mesh(
					{
						vertex{ {  0.0f,  0.5f, 0.0f }, vec3::one(), { 0.5f, 1.0f } },
						vertex{ {  0.5f, -0.5f, 0.0f }, vec3::one(), { 1.0f, 0.0f } },
						vertex{ { -0.5f, -0.5f, 0.0f }, vec3::one(), { 0.0f, 0.0f } },
					},
					{
						0, 1, 2,
					}
				));

				m_models["quad"] = make_model(make_mesh(
					{
						vertex{ { +1.0f, +1.0f, 0.0f }, vec3::one(), { 1.0f, 1.0f } },
						vertex{ { +1.0f, -1.0f, 0.0f }, vec3::one(), { 1.0f, 0.0f } },
						vertex{ { -1.0f, -1.0f, 0.0f }, vec3::one(), { 0.0f, 0.0f } },
						vertex{ { -1.0f, +1.0f, 0.0f }, vec3::one(), { 0.0f, 1.0f } },
					},
					{
						0, 1, 3,
						1, 2, 3,
					}
				));
			}

			// ENTITIES
			{
				ML_defer{ m_ecs.refresh(); };

				auto make_renderer = [&](auto shd, auto mat, auto mdl, auto tf)
				{
					auto & h{ m_ecs.create_handle() };
					h.add_tag<t_renderer>();
					h.add_component<c_shader>	(m_shaders	[shd]);
					h.add_component<c_material>	(m_materials[mat]);
					h.add_component<c_model>	(m_models	[mdl]);
					h.add_component<c_transform>(tf);
					return h;
				};
				
				auto & earth = make_renderer("3d", "3d", "sphere32x24", c_transform{
					vec3{ -.5f, 0.f, 0.f },
					vec4{ 0.0f, 0.1f, 0.0f, .15f },
					vec3::fill(1.f)
					});

				auto & moon = make_renderer("3d", "moon", "monkey", c_transform{
					vec3{ 1.f, 0.f, 0.f },
					vec4{ 0.0f, 0.1f, 0.0f, -.25f },
					vec3::fill(.27f)
					});
			}
		}

		void on_update(update_event const &)
		{
			// update stuff, etc...

			// console
			if (m_cout) { m_console.printss(m_cout); }

			// plots
			m_plots.update(engine::time().total().count<float_t>());
			
			// systems
			m_ecs.update_system<x_apply_transforms>();
			m_ecs.update_system<x_apply_materials>();

			// pipeline
			if (m_display_size[0] > 0 && m_display_size[1] > 0)
			{
				m_pipeline.for_each([&](auto &, auto & e)
				{
					e.resize(m_display_size);
				});
			}
		}

		void on_draw(draw_event const &)
		{
			// draw stuff, etc...

			if (m_pipeline.empty()) { return; }

			if (render_texture & target{ m_pipeline[0] })
			{
				ML_bind_scope(target);
				target.clear_color(colors::magenta);
				target.viewport(target.bounds());
				m_ecs.update_system<x_draw_renderers>(target);
			}
		}

		void on_gui_dock(dock_gui_event const &)
		{
			// gui docking

			enum : int32_t // node ids
			{
				root,
				left, left_up, left_dn, left_dn2,
				right, right_up, right_dn,
				MAX_DOCK_NODE
			};

			auto & d{ engine::gui().dockspace() };
			if (!d.nodes.empty()) { return; }
			d.nodes.resize(MAX_DOCK_NODE);
			
			// begin builder
			if (d[root] = d.begin_builder(ImGuiDockNodeFlags_AutoHideTabBar))
			{
				constexpr float_t lhs{ 0.465f }, rhs{ 1.f - lhs };

				// split nodes
				d.split(left	, d[root]	, ImGuiDir_Left	, lhs	, &d[root]);	// left
				d.split(left_up	, d[left]	, ImGuiDir_Up	, 0.5f	, &d[left]);	// left-up
				d.split(left_dn	, d[left]	, ImGuiDir_Down	, 0.71f	, &d[left]);	// left-down
				d.split(left_dn2, d[left_dn], ImGuiDir_Right, 0.29f	, &d[left_dn]);	// left-down2
				d.split(right	, d[root]	, ImGuiDir_Right, rhs	, &d[root]);	// right
				d.split(right_up, d[right]	, ImGuiDir_Up	, 0.5f	, &d[right]);	// right-up
				d.split(right_dn, d[right]	, ImGuiDir_Down	, 0.5f	, &d[right]);	// right-down

				// dock windows
				d.dock(m_gui_display.title		, d[left_up]);
				d.dock(m_gui_ecs.title			, d[left_dn]);
				d.dock(m_gui_assets.title		, d[left_dn]);
				d.dock(m_gui_files.title		, d[left_dn]);
				d.dock(m_gui_console.title		, d[left_dn]);
				d.dock(m_gui_profiler.title		, d[left_dn2]);
				d.dock(m_gui_memory.title		, d[right]);
				d.dock(m_gui_docs.title			, d[right]);
				d.dock(m_gui_scripting.title	, d[right]);
				d.dock(m_gui_nodes.title		, d[right]);

				// end builder
				d.end_builder(root);
			}
		}

		void on_gui_draw(draw_gui_event const &)
		{
			// gui stuff, etc...

			static ML_scope // main menu bar
			{
				auto & mmb{ engine::gui().main_menu_bar() };
				mmb.visible = true;
				mmb.add("file", [&]()
				{
					ML_imgui_scope_id(ML_addressof(this));
					if (ImGui::MenuItem("quit", "alt+f4"))
					{
						engine::window().set_should_close(true);
					}
				});
				mmb.add("tools", [&]()
				{
					ML_imgui_scope_id(ML_addressof(this));
					m_gui_assets.menu_item();
					m_gui_console.menu_item();
					m_gui_display.menu_item();
					m_gui_docs.menu_item();
					m_gui_ecs.menu_item();
					m_gui_memory.menu_item();
					m_gui_nodes.menu_item();
					m_gui_profiler.menu_item();
					m_gui_scripting.menu_item();
				});
				mmb.add("settings", [&]()
				{
					ML_imgui_scope_id(ML_addressof(this));
					bool fullscreen{ engine::window().is_fullscreen() };
					if (ImGui::MenuItem("fullscreen", "(FIXME)", &fullscreen))
					{
						engine::window().set_fullscreen(fullscreen);
					}
				});
				mmb.add("help", [&]()
				{
					ML_imgui_scope_id(ML_addressof(this));
					m_imgui_demo.menu_item();
					m_imgui_metrics.menu_item();
					m_imgui_about.menu_item();
				});
			};

			// draw
			ML_imgui_scope_id(ML_addressof(this));
			{
				// IMGUI
				if (m_imgui_demo.open)		{ engine::gui().show_imgui_demo(&m_imgui_demo.open); }
				if (m_imgui_metrics.open)	{ engine::gui().show_imgui_metrics(&m_imgui_metrics.open); }
				if (m_imgui_about.open)		{ engine::gui().show_imgui_about(&m_imgui_about.open); }

				// WIDGETS
				m_gui_display	.render(&demo::show_display_gui		, this); // DISPLAY
				m_gui_ecs		.render(&demo::show_ecs_gui			, this); // ECS
				m_gui_assets	.render(&demo::show_assets_gui		, this); // ASSETS
				m_gui_files		.render(&demo::show_files_gui		, this); // FILES
				m_gui_console	.render(&demo::show_console_gui		, this); // CONSOLE
				m_gui_profiler	.render(&demo::show_profiler_gui	, this); // PROFILER
				m_gui_scripting	.render(&demo::show_scripting_gui	, this); // SCRIPTING
				m_gui_nodes		.render(&demo::show_nodes_gui		, this); // NODES
				m_gui_memory	.render(&demo::show_memory_gui		, this); // MEMORY
				m_gui_docs		.render(&demo::show_documents_gui	, this); // DOCS
			}
		}

		void on_unload(unload_event const &)
		{
			// unload stuff, etc...

			m_ecs.clear();
			m_images.clear();
			m_shaders.clear();
			m_materials.clear();
			m_models.clear();
			m_textures.clear();
			m_fonts.clear();
			m_pipeline.clear();

			m_node_editor.destroy();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_assets_gui()
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			auto draw_asset = [&](auto const & n, auto const & v)
			{
				ML_imgui_scope_id(ML_addressof(&v));

				// type
				using T = typename std::decay_t<decltype(v)>;
				static constexpr auto ftype{ nameof<>::filter_all(typeof_v<T>.name()) };
				char type[64] = "";
				std::sprintf(type, "%.*s", (uint32_t)ftype.size(), ftype.data());

				// name
				pmr::string const name = util::to_string(n);

				// size
				char size[20] = "";
				std::sprintf(size, "%u", (uint32_t)sizeof(T));

				// address
				char addr[sizeof(size_t) * 2 + 1] = "";
				std::sprintf(addr, "%p", &v);

				if (ImGui::Selectable(type))		highlight_memory(&v); ImGui::NextColumn();
				if (ImGui::Selectable(name.c_str()))highlight_memory(&v); ImGui::NextColumn();
				if (ImGui::Selectable(size))		highlight_memory(&v); ImGui::NextColumn();
				if (ImGui::Selectable(addr))		highlight_memory(&v); ImGui::NextColumn();
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ImGui::Columns(4);

			ImGui::Text("type"); ImGui::NextColumn();
			ImGui::Text("name"); ImGui::NextColumn();
			ImGui::Text("size"); ImGui::NextColumn();
			ImGui::Text("addr"); ImGui::NextColumn();
			
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Columns(4);

			m_pipeline	.for_each([&](auto && ... args) { draw_asset(ML_forward(args)...); });
			m_fonts		.for_each([&](auto && ... args) { draw_asset(ML_forward(args)...); });
			m_images	.for_each([&](auto && ... args) { draw_asset(ML_forward(args)...); });
			m_materials	.for_each([&](auto && ... args) { draw_asset(ML_forward(args)...); });
			m_models	.for_each([&](auto && ... args) { draw_asset(ML_forward(args)...); });
			m_shaders	.for_each([&](auto && ... args) { draw_asset(ML_forward(args)...); });
			m_textures	.for_each([&](auto && ... args) { draw_asset(ML_forward(args)...); });

			ImGui::Columns(1);

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_console_gui()
		{
			// render console
			ML_defer{ m_console.render(); };

			// setup commands
			if (m_console.commands.empty())
			{
				m_console.commands.push_back({ "clear", [&](auto args)
				{
					m_console.clear();
				} });

				m_console.commands.push_back({ "exit", [&](auto args)
				{
					engine::window().set_should_close(true);
				} });

				m_console.commands.push_back({ "help", [&](auto args)
				{
					for (auto const & e : m_console.commands)
					{
						std::cout << e.first << "\n";
					}
				} });

				m_console.commands.push_back({ "history", [&](auto args)
				{
					for (auto const & e : m_console.history)
					{
						std::cout << e << "\n";
					}
				} });

				m_console.commands.push_back({ "python", [&](auto args)
				{
					if (!m_console.overload && args.empty())
					{
						m_console.overload = "python";
						static ML_scope{ std::cout << "# type \'\\\' to exit\n"; };
					}
					else if (m_console.overload && (args.front() == "\\"))
					{
						m_console.overload = nullptr;
					}
					else
					{
						engine::scripts().do_string(util::detokenize(args));
					}
				} });
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_display_gui()
		{
			if (m_pipeline.empty()) return;

			static gui::texture_preview pview{ nullptr, {}, 4.f, 32.f };
			
			pview.value = &m_pipeline.back().second.get_texture();

			pview.render([&]()
			{
				m_display_size = util::maintain(m_display_size, ImGui::GetContentRegionAvail());
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_documents_gui()
		{
			// menu bar
			if (ImGui::BeginMenuBar())
			{
				gui::help_marker("WIP");
				ImGui::Separator();
				if (ImGui::MenuItem("new")) {}
				if (ImGui::MenuItem("open")) {}
				if (ImGui::MenuItem("save")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("undo")) {}
				if (ImGui::MenuItem("redo")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("cut")) {}
				if (ImGui::MenuItem("copy")) {}
				if (ImGui::MenuItem("paste")) {}
				ImGui::Separator();
				ImGui::EndMenuBar();
			}

			static ImGui::TextEditor text{};
			static ML_scope
			{
				text.SetLanguageDefinition(ImGui::TextEditor::LanguageDefinition::CPlusPlus());
				text.SetText("int main()\n{\n\treturn 0;\n}");
			};

			if (ImGui::BeginTabBar("documents##tabs"))
			{
				if (ImGui::BeginTabItem("text document"))
				{
					text.Render("##document##text_editor", {}, true);

					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_ecs_gui()
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			// SHOW VALUE
			auto show_value = [&](auto const & value)
			{
				ML_imgui_scope_id(ML_addressof(&value));

				using T = typename std::decay_t<decltype(value)>;
				static constexpr auto info{ typeof_v<T> };
				static constexpr auto type{ nameof<>::filter_all(info.name()) };

				// FLOAT
				if constexpr (std::is_floating_point_v<T>)
				{
					auto temp{ static_cast<float_t>(value) };
					if (ImGui::DragFloat("##value", &temp)) {}
				}
				// INTEGRAL
				else if constexpr (std::is_integral_v<T>)
				{
					// BOOL
					if constexpr (std::is_same_v<T, bool>)
					{
						auto temp{ static_cast<bool>(value) };
						ImGui::Checkbox("##value", &temp);
					}
					// INT64
					else if constexpr (std::is_same_v<T, int64_t>)
					{
						auto temp{ static_cast<int64_t>(value) };
						ImGui::DragScalar("##value", ImGuiDataType_S64, &temp, 0.2f);
					}
					// INT32
					else if constexpr (std::is_same_v<T, int32_t>)
					{
						auto temp{ static_cast<int32_t>(value) };
						ImGui::DragScalar("##value", ImGuiDataType_S32, &temp, 0.2f);
					}
					// UINT64
					else if constexpr (std::is_same_v<T, uint64_t>)
					{
						auto temp{ static_cast<uint64_t>(value) };
						ImGui::DragScalar("##value", ImGuiDataType_U64, &temp, 0.2f);
					}
					// UINT32
					else if constexpr (std::is_same_v<T, uint32_t>)
					{
						auto temp{ static_cast<uint32_t>(value) };
						ImGui::DragScalar("##value", ImGuiDataType_U32, &temp, 0.2f);
					}
				}
				// POINTER
				else if constexpr (std::is_pointer_v<T>)
				{
					char buf[sizeof(size_t) * 2 + 1] = "";
					std::sprintf(buf, "%p", value);
					if (ImGui::Selectable(buf))
					{
						highlight_memory(value);
					}
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("copy"))
						{
							engine::window().set_clipboard_string(buf);
						}
						ImGui::EndPopup();
					}
				}
				// STRING
				else if constexpr (util::is_string_v<T>)
				{
					char buf[128] = "";
					if constexpr (std::is_pointer_v<T>)
					{
						std::strcpy(buf, value);
					}
					else
					{
						std::sprintf(buf, "%.*s", (int32_t)value.size(), value.data());
					}
					ImGui::InputText("##value", buf, ML_arraysize(buf),
						ImGuiInputTextFlags_ReadOnly
					);
				}
				// SIGNATURE
				else if constexpr (std::is_same_v<T, entity_traits::signature>)
				{
					auto const & style			{ ImGui::GetStyle() };
					auto const button_width		{ ImGui::GetFrameHeight() };
					auto const button_count		{ static_cast<int32_t>(value.size()) };
					auto const window_visible	{ ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x };

					int32_t i{};
					meta::for_types<meta::concat<entity_traits::component_list, entity_traits::tag_list>
					>([&](auto type)
					{
						ML_imgui_scope_id(i);
						bool temp{ value.read((size_t)i) };
						ImGui::Checkbox("##value", &temp);

						using U = typename entity_traits;
						using T = typename decltype(type)::type;
						using S = typename U::signature;
						static constexpr bool
							is_component{ U::has_component<T>() },
							is_tag{ U::has_tag<T>() };

						std::stringstream ss; ss
							<< i << ": " << nameof_v<T> << "\n"
							<< (is_component ? "component" : (is_tag ? "tag" : "?"))
							<< " [" << (is_component ? i : (is_tag ? i - U::component_count : -1)) << "]"
							<< " 0b" << S{}.set(i)
							;
						gui::tooltip(pmr::string{ ss.str() });

						float_t const last_button{ ImGui::GetItemRectMax().x };
						float_t const next_button{ last_button + style.ItemSpacing.x + button_width };
						if ((i + 1 < button_count) && (next_button < window_visible)) { ImGui::SameLine(); }
						++i;
					});
				}
				else
				{
					ImGui::Text("error type");
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			// SHOW FIELD
			auto show_field = [&](cstring label, auto const & value)
			{
				ImGui::Selectable(label); ImGui::NextColumn();
				show_value(value); ImGui::NextColumn();
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ImGui::BeginChildFrame(ImGui::GetID("ecs"),
				ImGui::GetContentRegionAvail(),
				ImGuiWindowFlags_NoScrollbar
			);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 2, 2 });
			ImGui::Columns(2);
			ImGui::Separator();
			m_ecs.for_entities([&](size_t const e)
			{
				ML_imgui_scope_id(static_cast<int32_t>(e));
				ImGui::AlignTextToFramePadding();
				
				bool const e_open{ ImGui::TreeNode(
					"entity node", "entity_%u", e
				) }; ImGui::NextColumn();

				ImGui::AlignTextToFramePadding();
				show_value(e); gui::tooltip("index");
				ImGui::NextColumn();

				if (e_open)
				{
					m_ecs.for_components(e, [&](auto & c)
					{
						ImGui::Separator();
						ML_imgui_scope_id(&c);
						using C = typename std::decay_t<decltype(c)>;
						static constexpr auto cname{ nameof_v<C> };

						bool const c_open{ ImGui::TreeNode(
							"component node", "[%u] %.*s",
							entity_traits::component_id<C>(),
							cname.size(), cname.data()
						) }; ImGui::NextColumn();

						ImGui::AlignTextToFramePadding();
						show_value(&c); gui::tooltip("address");
						ImGui::NextColumn();

						if (c_open)
						{
							show_field("size", sizeof(C));
							show_field("alignment", alignof(C));

							ImGui::TreePop();
						}
					});

					ImGui::TreePop();
				}
			});
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::PopStyleVar();
			ImGui::EndChildFrame();

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_files_gui()
		{
			// menu bar
			if (ImGui::BeginMenuBar())
			{
				gui::help_marker("WIP");
				ImGui::EndMenuBar();
			}

			// file list
			static gui::file_tree file_tree{ engine::fs().path2() };
			file_tree.render();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_memory_gui()
		{
			static auto const & testres{ memory_manager::get_testres() };

			static ML_scope // setup memory editor
			{
				m_mem_editor.Open				= true;
				m_mem_editor.ReadOnly			= true;
				m_mem_editor.Cols				= engine::window().get_hint(window_hints_maximized) ? 32 : 16;
				m_mem_editor.OptShowOptions		= true;
				m_mem_editor.OptShowDataPreview	= true;
				m_mem_editor.OptShowHexII		= false;
				m_mem_editor.OptShowAscii		= true;
				m_mem_editor.OptGreyOutZeroes	= true;
				m_mem_editor.OptUpperCaseHex	= true;
				m_mem_editor.OptMidColsCount	= 8;
				m_mem_editor.OptAddrDigitsCount	= 0;
				m_mem_editor.HighlightColor		= IM_COL32(0, 255, 255, 50);
				m_mem_editor.ReadFn				= nullptr;
				m_mem_editor.WriteFn			= nullptr;
				m_mem_editor.HighlightFn		= nullptr;
			};

			// menu bar
			if (ImGui::BeginMenuBar())
			{
				// read only
				ImGui::Checkbox("read only", &m_mem_editor.ReadOnly);
				ImGui::Separator();

				// begin / end
				if (ImGui::Button("begin")) highlight_memory(testres->begin());
				if (ImGui::Button("end")) highlight_memory(testres->end() - sizeof(void *));
				ImGui::Separator();

				// highlight
				ImGui::PushItemWidth(256);
				static memory_manager::record const * selected_record{};
				char selected_address[20] = "highlight";
				if (selected_record)
				{
					std::sprintf(selected_address, "%p", selected_record->data);
				}
				if (ImGui::BeginCombo("##records", selected_address, 0))
				{
					static auto const initial_width{ ImGui::GetContentRegionAvailWidth() };
					ImGui::Columns(3);

					static ML_scope{ ImGui::SetColumnWidth(-1, initial_width * 0.50f); };
					ImGui::Text("address"); ImGui::NextColumn();

					static ML_scope{ ImGui::SetColumnWidth(-1, initial_width * 0.25f); };
					ImGui::Text("index"); ImGui::NextColumn();

					static ML_scope{ ImGui::SetColumnWidth(-1, initial_width * 0.25f); };
					ImGui::Text("size"); ImGui::NextColumn();

					ImGui::Separator();
					for (auto const & rec : memory_manager::get_records().values())
					{
						ML_imgui_scope_id(ML_addressof(&rec));
						char addr[20] = ""; std::sprintf(addr, "%p", rec.data);
						bool const pressed{ ImGui::Selectable(addr) }; ImGui::NextColumn();
						ImGui::TextDisabled("%u", rec.index); ImGui::NextColumn();
						ImGui::TextDisabled("%u", rec.size); ImGui::NextColumn();
						if (pressed)
						{
							selected_record = &rec;
							highlight_memory(rec.data, rec.size);
						}
					}
					ImGui::Columns(1);
					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();
				ImGui::Separator();

				// progress
				char progress[32] = "";
				std::sprintf(progress, "%u / %u (%.2f%%)",
					(uint32_t)testres->used_bytes(),
					(uint32_t)testres->capacity(),
					testres->percent_used()
				);
				ImGui::ProgressBar(testres->fraction_used(), { 256.f, 0.f }, progress);
				gui::tooltip_ex([&]() noexcept
				{
					ImGui::Text("allocations: %u", testres->num_allocations());
					ImGui::Text("total:       %u", testres->capacity());
					ImGui::Text("in use:      %u", testres->used_bytes());
					ImGui::Text("available:   %u", testres->free_bytes());
				});
				ImGui::Separator();

				ImGui::EndMenuBar();
			}

			// memory content
			m_mem_editor.DrawContents(testres->buffer(), testres->capacity(), testres->addr());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_nodes_gui()
		{
			namespace ed = ax::NodeEditor;

			auto & io{ ImGui::GetIO() };
			
			ImGui::Separator();
			
			ed::SetCurrentEditor(m_node_editor);
			ed::Begin("My Editor", { 0.f, 0.f });
			
			int32_t uniqueId{ 1 };

			ed::BeginNode(uniqueId++);
			{
				ImGui::Text("Node A");
				ed::BeginPin(uniqueId++, ed::PinKind::Input);
				{
					ImGui::Text("-> In");
				}
				ed::EndPin();
				ImGui::SameLine();
				ed::BeginPin(uniqueId++, ed::PinKind::Output);
				{
					ImGui::Text("Out ->");
				}
				ed::EndPin();
			}
			ed::EndNode();

			ed::End();
			ed::SetCurrentEditor(nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_profiler_gui()
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static auto const & tt{ engine::time().total() };

			// total time
			ImGui::Columns(2);
			ImGui::Selectable("total time"); ImGui::NextColumn();
			ImGui::Text("%.3fs", tt.count()); ImGui::NextColumn();
			ImGui::Columns(1);
			ImGui::Separator();

			// plots
			m_plots.for_each([&](gui::plot & p)
			{
				ML_imgui_scope_id(ML_addressof(&p));
				p.render();
				if (ImGui::BeginPopupContextItem("plot settings"))
				{
					ImGui::Checkbox("animate", &p.animate); ImGui::SameLine();
					ImGui::Combo("##mode", &p.mode, "lines\0histogram\0");
					ImGui::EndPopup();
				}
			});

			// benchmarks
			ImGui::Columns(2);
			if (static auto const & frame{ performance_tracker::previous() }; !frame.empty())
			{
				ImGui::Separator();
				for (auto const & elem : frame)
				{
					char time[20] = "";
					std::sprintf(time, "%.7fs", elem.second.count());
					ImGui::Selectable(elem.first); gui::tooltip(time); ImGui::NextColumn();
					ImGui::Text(time); gui::tooltip(elem.first); ImGui::NextColumn();
				}
			}
			ImGui::Separator();
			ImGui::Columns(1);

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_scripting_gui()
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::EndMenuBar();
			}

			// WIP
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern "C" ML_PLUGIN_API ml::plugin * ml_plugin_main()
{
	static auto p{ new ml::demo{} };
	return p;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */