/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/StreamSniper.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/Wrapper.hpp>
#include <libmeme/Core/ECS.hpp>
#include <libmeme/Engine/Embed.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/ImGuiExt.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Renderer/RenderStates.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>
#include <libmeme/Renderer/Model.hpp>
#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/RenderTexture.hpp>
#include <libmeme/Renderer/Font.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// ECS CONFIG
namespace ml
{
	// (T) TAGS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct t_renderer {};

	// (C) COMPONENTS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct c_shader		: ds::value_wrapper<shader	> {};
	struct c_material	: ds::value_wrapper<material> {};
	struct c_model		: ds::value_wrapper<model	> {};
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

	template <class> struct x_apply_transforms final : ecs::util::x_base<s_apply_transforms>
	{
		void update(c_material & mat, c_transform const & tf)
		{
			mat->set<vec3>("u_position", tf.pos)
				.set<vec4>("u_rotation", tf.rot)
				.set<vec3>("u_scale", tf.scl);
		}
	};

	template <class> struct x_apply_materials final : ecs::util::x_base<s_apply_materials>
	{
		void update(c_shader & shd, c_material const & mat)
		{
			shd->bind(false);
			for (uniform const & u : *mat)
				shd->set_uniform(u);
			shd->unbind();
		}
	};

	template <class> struct x_draw_renderers final : ecs::util::x_base<s_draw_renderers>
	{
		void update(render_target const & target, c_shader const & shd, c_model const & mdl)
		{
			shd->bind(true);
			target.draw(*mdl);
			shd->unbind();
		}
	};


	// (M) MANAGER
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

		ds::flat_map<int32_t	,	render_texture	> m_pipeline	{};
		ds::flat_map<pmr::string,	font			> m_fonts		{};
		ds::flat_map<pmr::string,	image			> m_images		{};
		ds::flat_map<pmr::string,	material		> m_materials	{};
		ds::flat_map<pmr::string,	model			> m_models		{};
		ds::flat_map<pmr::string,	shader			> m_shaders		{};
		ds::flat_map<pmr::string,	texture			> m_textures	{};


		// ECS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		entity_manager m_ecs{};

		pmr::vector<entity_manager::handle> m_handles;


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
			m_gui_memory	{ "memory##demo"		, 1, "", ImGuiWindowFlags_MenuBar },
			m_gui_profiler	{ "profiler##demo"		, 1, "", ImGuiWindowFlags_None };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		stream_sniper m_cout{ &std::cout };

		vec2 m_display_size{};

		gui::console m_console{};

		MemoryEditor m_memory{};

		inline auto highlight_memory(byte_t * ptr, size_t const size)
		{
			static auto * const testres{ memory_manager::get_testres() };
			auto const addr{ std::distance(testres->begin(), ptr) };
			m_gui_memory.set_focused();
			m_memory.GotoAddrAndHighlight((size_t)addr, (size_t)addr + size);
		}

		template <class T> inline auto highlight_memory(T const * ptr)
		{
			highlight_memory((byte_t *)ptr, sizeof(T));
		}

		gui::plot_controller m_plots{
		{
			gui::make_plot(120, 1, "##frame time", "%.3f ms/frame",
			vec2{ 0.f, 64.f }, vec2{ FLT_MAX, FLT_MAX },
			[]() { static auto const & dt{ engine::get_io().delta_time }; return dt * 1000.f; }
			),

			gui::make_plot(120, 1, "##frame rate", "%.3f fps",
			vec2{ 0.f, 64.f }, vec2{ FLT_MAX, FLT_MAX },
			[]() { static auto const & fps{ engine::get_io().frame_rate }; return fps; }
			),
		} };


		// DEMO
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		demo()
		{
			event_system::add_listener<enter_event		>(this);
			event_system::add_listener<update_event		>(this);
			event_system::add_listener<draw_event		>(this);
			event_system::add_listener<gui_dock_event	>(this);
			event_system::add_listener<gui_draw_event	>(this);
			event_system::add_listener<exit_event		>(this);
		}

		void on_event(event const & value) override
		{
			switch (value.id())
			{
			case hashof_v<enter_event	> : on_enter	(*value.as<	enter_event		>()); break;
			case hashof_v<update_event	> : on_update	(*value.as<	update_event	>()); break;
			case hashof_v<draw_event	> : on_draw		(*value.as<	draw_event		>()); break;
			case hashof_v<gui_dock_event> : on_gui_dock	(*value.as<	gui_dock_event	>()); break;
			case hashof_v<gui_draw_event> : on_gui_draw	(*value.as<	gui_draw_event	>()); break;
			case hashof_v<exit_event	> : on_exit		(*value.as<	exit_event		>()); break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_enter(enter_event const &)
		{
			// load stuff, etc...

			// GUI
			setup_menus();

			// ICON
			if (auto icon{ make_image(engine::path_to("assets/textures/icon.png")) }
			; !icon.empty())
			{
				engine::get_window().set_icon(icon.width(), icon.height(), icon.data());
			}

			// PIPELINE
			{
				(m_pipeline[0] = make_render_texture(vec2i{ 1280, 720 })).create();
			}

			// TEXTURES
			{
				m_textures["doot"] = make_texture(
					engine::path_to("assets/textures/doot.png")
				);

				m_textures["navball"] = make_texture(
					engine::path_to("assets/textures/navball.png")
				);

				m_textures["earth_dm_2k"] = make_texture(
					engine::path_to("assets/textures/earth/earth_dm_2k.png")
				);

				m_textures["moon_dm_2k"] = make_texture(
					engine::path_to("assets/textures/moon/moon_dm_2k.png")
				);
			}

			// FONTS
			{
				m_fonts["clacon"] = make_font(
					engine::path_to("assets/fonts/clacon.ttf")
				);

				m_fonts["consolas"] = make_font(
					engine::path_to("assets/fonts/consolas.ttf")
				);

				m_fonts["lucida_console"] = make_font(
					engine::path_to("assets/fonts/lucida_console.ttf")
				);

				m_fonts["minecraft"] = make_font(
					engine::path_to("assets/fonts/minecraft.ttf")
				);
			}

			// SHADERS
			{
				m_shaders["2d"] = make_shader(
					engine::path_to("assets/shaders/2D.vs.shader"),
					engine::path_to("assets/shaders/basic.fs.shader")
				);

				m_shaders["3d"] = make_shader(
					engine::path_to("assets/shaders/3D.vs.shader"),
					engine::path_to("assets/shaders/basic.fs.shader")
				);
			}

			// MATERIALS
			{
				// timers
				auto const _timers = make_material(
					make_uniform<float_t>("u_time",		[]() { return engine::get_time().count<float_t>(); }),
					make_uniform<float_t>("u_delta",	[]() { return engine::get_io().delta_time; })
				);

				// MVP
				auto const _mvp = make_material(
					make_uniform<mat4	>("u_model",	mat4::identity()),
					make_uniform<mat4	>("u_view",		mat4::identity()),
					make_uniform<mat4	>("u_proj",		mat4::identity())
				);

				// camera
				auto const _camera = make_material(
					make_uniform<vec3	>("u_cam.pos",	vec3{ 0, 0, 3.f }),
					make_uniform<vec3	>("u_cam.dir",	vec3{ 0, 0, -1.f }),
					make_uniform<float_t>("u_cam.fov",	45.0f),
					make_uniform<float_t>("u_cam.near", 0.0001f),
					make_uniform<float_t>("u_cam.far",	1000.0f),
					make_uniform<vec2	>("u_cam.view", vec2{ 1280.f, 720.f })
				);

				// transform
				auto const _tf = make_material(
					make_uniform<vec3	>("u_position", vec3{}),
					make_uniform<vec4	>("u_rotation", vec4{}),
					make_uniform<vec3	>("u_scale",	vec3{})
				);

				// 2d
				m_materials["2d"] = make_material(
					make_uniform<color	>("u_color",	colors::white),
					make_uniform<texture>("u_texture0", &m_textures["doot"])
				) + _timers + _mvp;

				// 3d
				m_materials["3d"] = make_material(
					make_uniform<color	>("u_color",	colors::white),
					make_uniform<texture>("u_texture0", &m_textures["earth_dm_2k"])
					//, make_uniform<texture>("u_texture1", &m_textures["moon_dm_2k"])
				) + _timers + _camera + _tf;

				// test
				m_materials["test"] = (make_material() + m_materials["3d"])
					.set<texture const *>("u_texture0", &m_textures["moon_dm_2k"]);

			}

			// MODELS
			{
				m_models["sphere8x6"] = make_model(
					engine::path_to("assets/models/sphere8x6.obj")
				);

				m_models["sphere32x24"] = make_model(
					engine::path_to("assets/models/sphere32x24.obj")
				);

				// FIXME: upside down?
				m_models["monkey"] = make_model(
					engine::path_to("assets/models/monkey.obj")
				);

				// FIXME: ibo broken
				m_models["triangle"] = make_model(make_mesh(
					{
						make_vertex({  0.0f,  0.5f, 0.0f }, vec3::one(), { 0.5f, 1.0f }),
						make_vertex({  0.5f, -0.5f, 0.0f }, vec3::one(), { 1.0f, 0.0f }),
						make_vertex({ -0.5f, -0.5f, 0.0f }, vec3::one(), { 0.0f, 0.0f }),
					},
					{
						0, 1, 2,
					}
				));

				// FIXME: ibo broken
				m_models["quad"] = make_model(make_mesh(
					{
						make_vertex({ +1.0f, +1.0f, 0.0f }, vec3::one(), { 1.0f, 1.0f }),
						make_vertex({ +1.0f, -1.0f, 0.0f }, vec3::one(), { 1.0f, 0.0f }),
						make_vertex({ -1.0f, -1.0f, 0.0f }, vec3::one(), { 0.0f, 0.0f }),
						make_vertex({ -1.0f, +1.0f, 0.0f }, vec3::one(), { 0.0f, 1.0f }),
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
					auto & h{ m_handles.emplace_back(m_ecs.create_handle()) };
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
					vec3{ 1.f, 1.f, 1.f }
					});

				auto & moon = make_renderer("3d", "test", "sphere8x6", c_transform{
					vec3{ 1.f, 0.f, 0.f },
					vec4{ 0.0f, 0.1f, 0.0f, -.25f },
					vec3::fill(.27f)
					});
			}
		}

		void on_update(update_event const &)
		{
			// update stuff, etc...

			// timers
			static auto const & dt	{ engine::get_io().delta_time };
			static auto const & fps	{ engine::get_io().frame_rate };
			static auto const & fc	{ engine::get_io().frame_count };
			static auto const & tt	{ engine::get_time() };

			// console
			if (m_cout) { m_console.printss(m_cout); }

			// plots
			m_plots.update(tt.count());
			
			// systems
			m_ecs.update_system<x_apply_transforms>();
			m_ecs.update_system<x_apply_materials>();

			// pipeline
			if (m_display_size[0] > 0 && m_display_size[1] > 0)
			{
				m_pipeline.for_each([&](auto &, auto & value)
				{
					value.resize(m_display_size);
				});
			}
		}

		void on_draw(draw_event const &)
		{
			// draw stuff, etc...

			if (m_pipeline.empty()) return;

			if (render_texture const & target{ m_pipeline[0] })
			{
				target.bind();
				target.clear_color(colors::magenta);
				target.viewport(target.bounds());
				constexpr render_states states{
					{}, {}, cull_state{ false }, {}
				}; states();
				m_ecs.update_system<x_draw_renderers>(target);
				target.unbind();
			}
		}

		void on_gui_dock(gui_dock_event const &)
		{
			// gui docking

			enum : int32_t // node ids
			{
				root,
				left, left_up, left_dn, left_dn2,
				right, right_up, right_dn,
				MAX_DOCK_NODE
			};

			auto & d{ editor::get_io().dockspace };
			d.nodes.resize(MAX_DOCK_NODE);
			if (d[root] = d.begin_builder(ImGuiDockNodeFlags_AutoHideTabBar))
			{
				constexpr float_t lhs = 0.465f, rhs = 1.f - lhs;

				d.split(left		, d[root]		, ImGuiDir_Left	, lhs	, &d[root]);	// left
				d.split(left_up		, d[left]		, ImGuiDir_Up	, 0.5f	, &d[left]);	// left-up
				d.split(left_dn		, d[left]		, ImGuiDir_Down	, 0.71f	, &d[left]);	// left-down
				d.split(left_dn2	, d[left_dn]	, ImGuiDir_Right, 0.29f	, &d[left_dn]);	// left-down2

				d.split(right		, d[root]		, ImGuiDir_Right, rhs	, &d[root]);	// right
				d.split(right_up	, d[right]		, ImGuiDir_Up	, 0.5f	, &d[right]);	// right-up
				d.split(right_dn	, d[right]		, ImGuiDir_Down	, 0.5f	, &d[right]);	// right-down

				d.dock(m_gui_display.title	, d[left_up]);
				d.dock(m_gui_assets.title	, d[left_dn]);
				d.dock(m_gui_ecs.title		, d[left_dn]);
				d.dock(m_gui_console.title	, d[left_dn]);
				d.dock(m_gui_profiler.title	, d[left_dn2]);
				d.dock(m_gui_memory.title	, d[right]);
				d.dock(m_gui_docs.title		, d[right]);

				d.end_builder(root);
			}
		}

		void on_gui_draw(gui_draw_event const &)
		{
			// gui stuff, etc...

			ML_ImGui_ScopeID(ML_addressof(this));

			// IMGUI
			if (m_imgui_demo.open) { editor::show_imgui_demo(&m_imgui_demo.open); }
			if (m_imgui_metrics.open) { editor::show_imgui_metrics(&m_imgui_metrics.open); }
			if (m_imgui_about.open) { editor::show_imgui_about(&m_imgui_about.open); }

			// ASSETS
			m_gui_assets.render([&](auto &) { show_assets_gui(); });

			// DISPLAY
			m_gui_display.render([&](auto &) { show_display_gui(); });

			// DOCUMENTS
			m_gui_docs.render([&](auto &) { show_documents_gui(); });

			// ECS
			m_gui_ecs.render([&](auto &) { show_ecs_gui(); });

			// CONSOLE
			m_gui_console.render([&](auto &) { show_console_gui(); });

			// PROFILER
			m_gui_profiler.render([&](auto &) { show_profiler_gui(); });

			// MEMORY
			m_gui_memory.render([&](auto &) { show_memory_gui(); });
		}

		void on_exit(exit_event const &)
		{
			// cleanup stuff, etc...

			m_ecs.clear();
			m_images.clear();
			m_shaders.clear();
			m_materials.clear();
			m_models.clear();
			m_textures.clear();
			m_fonts.clear();
			m_pipeline.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void setup_menus()
		{
			auto & m{ editor::get_io().main_menu };

			m.add_menu("file", [&]()
			{
				ML_ImGui_ScopeID(ML_addressof(this));
				if (ImGui::MenuItem("quit", "Alt+F4"))
				{
					engine::close();
				}
			});
			m.add_menu("tools", [&]()
			{
				ML_ImGui_ScopeID(ML_addressof(this));
				m_gui_assets.menu_item();
				m_gui_console.menu_item();
				m_gui_display.menu_item();
				m_gui_docs.menu_item();
				m_gui_ecs.menu_item();
				m_gui_memory.menu_item();
				m_gui_profiler.menu_item();
			});
			m.add_menu("options", [&]()
			{
				ML_ImGui_ScopeID(ML_addressof(this));
				bool fullscreen{ engine::get_window().is_fullscreen() };
				if (ImGui::MenuItem("fullscreen", "", &fullscreen))
				{
					engine::get_window().set_fullscreen(fullscreen);
				}
			});
			m.add_menu("help", [&]()
			{
				ML_ImGui_ScopeID(ML_addressof(this));
				m_imgui_demo.menu_item();
				m_imgui_metrics.menu_item();
				m_imgui_about.menu_item();
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_assets_gui()
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			auto draw_item = [&](auto const & n, auto const & v)
			{
				ML_ImGui_ScopeID(ML_addressof(&v));
				using T = typename std::decay_t<decltype(v)>;
				static constexpr auto tname{ nameof<>::filter_all(typeof_v<T>.name()) };

				char name[64] = "";
				std::sprintf(name, "%.*s", (uint32_t)tname.size(), tname.data());

				char size[20] = "";
				std::sprintf(size, "%u", (uint32_t)sizeof(T));

				char addr[sizeof(size_t) * 2 + 1] = "";
				std::sprintf(addr, "%p", &v);

				pmr::string const s = util::to_string(n);

				if (ImGui::Selectable(name))		highlight_memory(&v); ImGui::NextColumn();
				if (ImGui::Selectable(s.c_str()))	highlight_memory(&v); ImGui::NextColumn();
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

			m_pipeline.for_each([&](auto const & n, auto const & v) { draw_item(n, v); });
			m_fonts.for_each([&](auto const & n, auto const & v) { draw_item(n, v); });
			m_images.for_each([&](auto const & n, auto const & v) { draw_item(n, v); });
			m_materials.for_each([&](auto const & n, auto const & v) { draw_item(n, v); });
			m_models.for_each([&](auto const & n, auto const & v) { draw_item(n, v); });
			m_shaders.for_each([&](auto const & n, auto const & v) { draw_item(n, v); });
			m_textures.for_each([&](auto const & n, auto const & v) { draw_item(n, v); });

			ImGui::Columns(1);

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_console_gui()
		{
			// render console
			ML_defer{ m_console.render(); };

			// setup commands
			if (auto & cmd{ m_console.commands }; cmd.empty())
			{
				cmd.push_back({ "clear", [&](auto args)
				{
					m_console.clear();
				} });

				cmd.push_back({ "exit", [&](auto args)
				{
					engine::close();
				} });

				cmd.push_back({ "help", [&](auto args)
				{
					for (auto const & e : m_console.commands)
					{
						std::cout << e.first << "\n";
					}
				} });

				cmd.push_back({ "history", [&](auto args)
				{
					for (auto const & e : m_console.history)
					{
						std::cout << e << "\n";
					}
				} });

				cmd.push_back({ "lua", [&](auto args)
				{
					if (!m_console.overload && args.empty())
					{
						m_console.overload = "lua";
						ML_ONCE_CALL{ std::cout << "# type \'\\\' to exit\n"; };
					}
					else if (m_console.overload && (args.front() == "\\"))
					{
						m_console.overload = nullptr;
					}
					else
					{
						engine::do_script<embed::api::lua>(util::detokenize(args));
					}
				} });

				cmd.push_back({ "python", [&](auto args)
				{
					if (!m_console.overload && args.empty())
					{
						m_console.overload = "python";
						ML_ONCE_CALL{ std::cout << "# type \'\\\' to exit\n"; };
					}
					else if (m_console.overload && (args.front() == "\\"))
					{
						m_console.overload = nullptr;
					}
					else
					{
						engine::do_script<embed::api::python>(util::detokenize(args));
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
				if (ImGui::MenuItem("new")) {}
				if (ImGui::MenuItem("open")) {}
				if (ImGui::MenuItem("save")) {}
				if (ImGui::MenuItem("save all")) {}
				ImGui::EndMenuBar();
			}

			// tab bar
			if (ImGui::BeginTabBar("##documents##tabs"))
			{
				gui::help_marker("WIP");
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
				ML_ImGui_ScopeID(ML_addressof(&value));

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
							engine::get_window().set_clipboard(buf);
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
						ML_ImGui_ScopeID(i);
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
				ML_ImGui_ScopeID(static_cast<int32_t>(e));
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
						ML_ImGui_ScopeID(&c);
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

		void show_memory_gui()
		{
			static auto * const testres{ memory_manager::get_testres() };

			// setup memory editor
			ML_ONCE_CALL{
				m_memory.Open				= true;
				m_memory.ReadOnly			= true;
				m_memory.Cols				= engine::get_window().get_hints() & WindowHints_Maximized ? 32 : 16;
				m_memory.OptShowOptions		= true;
				m_memory.OptShowDataPreview	= true;
				m_memory.OptShowHexII		= false;
				m_memory.OptShowAscii		= true;
				m_memory.OptGreyOutZeroes	= true;
				m_memory.OptUpperCaseHex	= true;
				m_memory.OptMidColsCount	= 8;
				m_memory.OptAddrDigitsCount	= 0;
				m_memory.HighlightColor		= IM_COL32(0, 255, 255, 50);
				m_memory.ReadFn				= nullptr;
				m_memory.WriteFn			= nullptr;
				m_memory.HighlightFn		= nullptr;
			};

			// menu bar
			if (ImGui::BeginMenuBar())
			{
				// read only
				ImGui::Checkbox("read only", &m_memory.ReadOnly);
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
					auto const width{ ImGui::GetContentRegionAvailWidth() };
					ImGui::Columns(3);
					ML_ONCE_CALL ImGui::SetColumnWidth(-1, width * 0.50f);
					ImGui::Text("address"); ImGui::NextColumn();
					ML_ONCE_CALL ImGui::SetColumnWidth(-1, width * 0.25f);
					ImGui::Text("index"); ImGui::NextColumn();
					ML_ONCE_CALL ImGui::SetColumnWidth(-1, width * 0.25f);
					ImGui::Text("size"); ImGui::NextColumn();
					ImGui::Separator();
					for (auto const & rec : memory_manager::get_records().values())
					{
						ML_ImGui_ScopeID(ML_addressof(&rec));
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
					(uint32_t)testres->total_bytes(),
					testres->percent()
				);
				ImGui::ProgressBar(testres->fraction(), { 256.f, 0.f }, progress);
				gui::tooltip_ex([&]() noexcept
				{
					ImGui::Text("allocations: %u", testres->num_allocations());
					ImGui::Text("total:       %u", testres->total_bytes());
					ImGui::Text("in use:      %u", testres->used_bytes());
					ImGui::Text("available:   %u", testres->free_bytes());
				});
				ImGui::Separator();

				ImGui::EndMenuBar();
			}

			// memory content
			m_memory.DrawContents(testres->data(), testres->total_bytes(), testres->addr());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_profiler_gui()
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static auto const & tt{ engine::get_time() };

			// total time
			ImGui::Columns(2);
			ImGui::Selectable("total time"); ImGui::NextColumn();
			ImGui::Text("%.3fs", tt.count()); ImGui::NextColumn();
			ImGui::Columns(1);
			ImGui::Separator();

			// plots
			m_plots.for_each([&](gui::plot & p)
			{
				ML_ImGui_ScopeID(ML_addressof(&p));
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
			if (static auto const & bench{ performance_tracker::last_frame() }; !bench.empty())
			{
				ImGui::Separator();
				for (auto const & elem : bench)
				{
					char time[20] = "";
					std::sprintf(time, "%.7fs", elem.second.count());
					ImGui::Selectable(elem.first);
					gui::tooltip(time); ImGui::NextColumn();
					ImGui::Text(time); ImGui::NextColumn();
				}
			}
			ImGui::Columns(1);

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern "C" ML_PLUGIN_API ml::plugin * ml_plugin_main()
{
	static ml::plugin * p{};
	if (!p) p = new ml::demo{};
	return p;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */