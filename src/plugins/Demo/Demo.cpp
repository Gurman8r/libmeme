/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/ECS.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/StreamSniper.hpp>
#include <libmeme/Core/Wrapper.hpp>
#include <libmeme/Engine/API_Embed.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/ImGuiExt.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Platform/WindowEvents.hpp>
#include <libmeme/Graphics/Font.hpp>
#include <libmeme/Graphics/Mesh.hpp>
#include <libmeme/Graphics/Material.hpp>
#include <libmeme/Graphics/Renderer.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// ECS
namespace ml
{
	// (T) TAGS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct t_renderer {};

	// (C) COMPONENTS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct c_shader		: ds::wrapper<shader_asset>	{};
	struct c_material	: ds::wrapper<material>		{};
	struct c_mesh		: ds::wrapper<mesh>			{};
	struct c_transform	{ vec3 pos; vec4 rot; vec3 scl; };


	// (S) SIGNATURES
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using s_update_materials = meta::list<c_material, c_transform
	>;
	using s_upload_uniforms = meta::list<c_shader, c_material
	>;
	using s_draw_meshes = meta::list<c_shader, c_mesh
	>;


	// (X) SYSTEMS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class> struct x_update_materials final : ecs::detail::x_base<s_update_materials>
	{
		void operator()(c_material & mat, c_transform const & tf)
		{
			mat->set<vec3>("u_position"	, tf.pos)
				.set<vec4>("u_rotation"	, tf.rot)
				.set<vec3>("u_scale"	, tf.scl);
		}
	};

	template <class> struct x_upload_uniforms final : ecs::detail::x_base<s_upload_uniforms>
	{
		void operator()(c_shader & shd, c_material const & mat)
		{
			ML_bind_scope(*shd, false);
			for (uniform const & u : *mat) { shd->set_uniform(u); }
		}
	};

	template <class> struct x_draw_meshes final : ecs::detail::x_base<s_draw_meshes>
	{
		void operator()(c_shader const & shd, c_mesh const & msh)
		{
			ML_bind_scope(*shd, true);
			gl::render_command::draw(msh->vao())();
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
		c_shader, c_material, c_mesh, c_transform
		>,

		// signatures
		ecs::cfg::signatures<
		s_update_materials, s_upload_uniforms, s_draw_meshes
		>,

		// systems
		ecs::cfg::systems<
		x_update_materials, x_upload_uniforms, x_draw_meshes
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

		ds::map< pmr::string, font				> m_fonts		{};
		ds::map< pmr::string, image				> m_images		{};
		ds::map< pmr::string, material			> m_materials	{};
		ds::map< pmr::string, mesh				> m_meshes		{};
		ds::map< pmr::string, shader_asset		> m_shaders		{};
		ds::map< pmr::string, shared<gl::texture2d>	> m_textures	{};


		// ECS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		entity_manager m_ecs{};


		// RENDERING
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shader_cache m_cache{};

		vec2 m_resolution{ 1280, 720 };

		ds::array<shared<gl::framebuffer>, 1> m_pipeline{};


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
			m_gui_renderer	{ "renderer##demo"		, 1, "", ImGuiWindowFlags_MenuBar };

		stream_sniper m_cout{ &std::cout };

		gui::console m_console{};

		ax::NodeEditor::EditorContext * m_node_editor{ ax::NodeEditor::CreateEditor() };

		gui::plot_controller m_plots
		{
			gui::plot::create(120
				, gui::plot::histogram
				, "##frame time"
				, "%.3f ms/frame"
				, vec2{ 0.f, 64.f }
				, vec2{ FLT_MAX, FLT_MAX }
				, []() { return 1000.f / engine::time().frame_rate(); }),

			gui::plot::create(120
				, gui::plot::histogram
				, "##frame rate"
				, "%.1f fps"
				, vec2{ 0.f, 64.f }
				, vec2{ FLT_MAX, FLT_MAX }
				, []() { return engine::time().frame_rate(); }),
		};

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

			// RENDERING
			{
				m_pipeline[0] = gl::framebuffer::create(gl::format_rgba, m_resolution);
			}

			// ICON
			if (image const icon{ engine::fs().path2("assets/textures/icon.png") })
			{
				engine::window().set_icon(icon.width(), icon.height(), icon.data());
			}

			// IMAGES
			{
				m_images["default"] = image::get_default_rgba();
			}

			// TEXTURES
			{
				m_textures["default"]		= gl::texture2d::create(m_images["default"]);
				m_textures["doot"]			= gl::texture2d::create(engine::fs().path2("assets/textures/doot.png"));
				m_textures["navball"]		= gl::texture2d::create(engine::fs().path2("assets/textures/navball.png"));
				m_textures["earth_dm_2k"]	= gl::texture2d::create(engine::fs().path2("assets/textures/earth/earth_dm_2k.png"));
				m_textures["earth_sm_2k"]	= gl::texture2d::create(engine::fs().path2("assets/textures/earth/earth_sm_2k.png"));
				m_textures["moon_dm_2k"]	= gl::texture2d::create(engine::fs().path2("assets/textures/moon/moon_dm_2k.png"));
			}

			// FONTS
			{
				m_fonts["clacon"]			= engine::fs().path2("assets/fonts/clacon.ttf");
				m_fonts["consolas"]			= engine::fs().path2("assets/fonts/consolas.ttf");
				m_fonts["lucida_console"]	= engine::fs().path2("assets/fonts/lucida_console.ttf");
				m_fonts["minecraft"]		= engine::fs().path2("assets/fonts/minecraft.ttf");
			}

			// SHADERS
			{
				m_cache.load_src(gl::vertex_shader, "2D",
					engine::fs().path2("assets/shaders/2D.vs.shader"));

				m_cache.load_src(gl::vertex_shader, "3D",
					engine::fs().path2("assets/shaders/3D.vs.shader"));

				m_cache.load_src(gl::fragment_shader, "basic",
					engine::fs().path2("assets/shaders/basic.fs.shader"));

				m_shaders["2D"] = { {
					m_cache.src(gl::vertex_shader, "2D"),
					m_cache.src(gl::fragment_shader, "basic"),
					{}
				} };

				m_shaders["3D"] = { {
					m_cache.src(gl::vertex_shader, "3D"),
					m_cache.src(gl::fragment_shader, "basic"),
					{}
				} };
			}

			// MATERIALS
			{
				// timers
				auto const _timers = material
				{
					make_uniform<float_t>("u_time"	, []() { return engine::time().total_time().count<float_t>(); }),
					make_uniform<float_t>("u_delta"	, []() { return engine::time().delta_time().count<float_t>(); })
				};

				// camera
				auto const _camera = material
				{
					make_uniform<vec3	>("u_cam.pos"	, vec3{ 0, 0, 3.f }),
					make_uniform<vec3	>("u_cam.dir"	, vec3{ 0, 0, -1.f }),
					make_uniform<float_t>("u_cam.fov"	, 45.0f),
					make_uniform<float_t>("u_cam.near"	, 0.0001f),
					make_uniform<float_t>("u_cam.far"	, 1000.0f),
					make_uniform<vec2	>("u_cam.view"	, vec2{ 1280.f, 720.f })
				};

				// transform
				auto const _tf = material
				{
					make_uniform<vec3	>("u_position"	, vec3{}),
					make_uniform<vec4	>("u_rotation"	, vec4{}),
					make_uniform<vec3	>("u_scale"		, vec3{})
				};

				// 3d
				auto const _3d = material
				{
					make_uniform<color>("u_color", colors::white),
					make_uniform<gl::texture2d>("u_texture0", m_textures["default"])
				}
				+ _timers + _camera + _tf;

				// earth
				m_materials["earth"] = material{ _3d }
					.set<gl::texture2d>("u_texture0", m_textures["earth_dm_2k"])
					;

				// moon
				m_materials["moon"] = material{ _3d }
					.set<gl::texture2d>("u_texture0", m_textures["moon_dm_2k"])
					;
			}

			// MODELS
			{
				m_meshes["sphere8x6"]	= engine::fs().path2("assets/models/sphere8x6.obj");
				m_meshes["sphere32x24"] = engine::fs().path2("assets/models/sphere32x24.obj");
				m_meshes["monkey"]		= engine::fs().path2("assets/models/monkey.obj");

				m_meshes["triangle"] =
				{
					{
						vertex{ {  0.0f,  0.5f, 0.0f }, vec3::one(), { 0.5f, 1.0f } },
						vertex{ {  0.5f, -0.5f, 0.0f }, vec3::one(), { 1.0f, 0.0f } },
						vertex{ { -0.5f, -0.5f, 0.0f }, vec3::one(), { 0.0f, 0.0f } },
					},
					{
						0, 1, 2,
					}
				};

				m_meshes["quad"] =
				{
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
				};
			}

			// ENTITIES
			{
				ML_defer{ m_ecs.refresh(); };

				auto make_renderer = [&](auto shd, auto mat, auto msh, auto tf)
				{
					auto & h{ m_ecs.create_handle() };
					h.add_tag<t_renderer>();
					h.add_component<c_shader>	(m_shaders	[shd]);
					h.add_component<c_material>	(m_materials[mat]);
					h.add_component<c_mesh>		(m_meshes	[msh]);
					h.add_component<c_transform>(tf);
					return h;
				};
				
				auto & earth = make_renderer("3D", "earth", "sphere32x24", c_transform{
					vec3{ -.5f, 0.f, 0.f },
					vec4{ 0.0f, 0.1f, 0.0f, .15f },
					vec3::fill(1.f)
					});

				auto & moon = make_renderer("3D", "moon", "sphere8x6", c_transform{
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
			if (pmr::stringstream ss{ m_cout.sstr().str() })
			{
				pmr::string line{};
				while (std::getline(ss, line))
				{
					m_console.printl(line);
				}
				m_cout.sstr().str({});
			}

			// plots
			m_plots.update(engine::time().total_time().count());
			
			// systems
			m_ecs.update_system<x_update_materials>();
			m_ecs.update_system<x_upload_uniforms>();

			// pipeline
			for (auto & fbo : m_pipeline)
			{
				fbo->resize(m_resolution);
			}
		}

		void on_draw(draw_event const &)
		{
			// draw stuff, etc...

			if (m_pipeline[0])
			{
				ML_bind_scope(m_pipeline[0].get());

				for (auto const & cmd :
				{
					gl::render_command::set_cull_enabled(false),
					gl::render_command::set_clear_color(colors::magenta),
					gl::render_command::clear(gl::color_bit | gl::depth_bit | gl::stencil_bit),
					gl::render_command::custom([&]()
					{
						m_ecs.update_system<x_draw_meshes>();
					}),
				})
				{
					std::invoke(cmd);
				}
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
				d.dock(m_gui_nodes.title		, d[right]);
				d.dock(m_gui_renderer.title		, d[right]);

				// end builder
				d.end_builder(root);
			}
		}

		void on_gui_draw(draw_gui_event const &)
		{
			// gui stuff, etc...

			static ML_scope // setup main menu bar
			{
				auto & mmb{ engine::gui().main_menu_bar() };
				mmb.visible = true;
				mmb.add("file", [&]()
				{
					ML_scoped_imgui_id(ML_addressof(this));
					if (ImGui::MenuItem("quit", "alt+f4"))
					{
						engine::window().set_should_close(true);
					}
				});
				mmb.add("tools", [&]()
				{
					ML_scoped_imgui_id(ML_addressof(this));
					m_gui_assets.menu_item();
					m_gui_console.menu_item();
					m_gui_display.menu_item();
					m_gui_docs.menu_item();
					m_gui_ecs.menu_item();
					m_gui_memory.menu_item();
					m_gui_nodes.menu_item();
					m_gui_profiler.menu_item();
					m_gui_renderer.menu_item();
				});
				mmb.add("settings", [&]()
				{
					ML_scoped_imgui_id(ML_addressof(this));
					bool fullscreen{ engine::window().is_fullscreen() };
					if (ImGui::MenuItem("fullscreen", "(FIXME)", &fullscreen))
					{
						engine::window().set_fullscreen(fullscreen);
					}
				});
				mmb.add("help", [&]()
				{
					ML_scoped_imgui_id(ML_addressof(this));
					m_imgui_demo.menu_item();
					m_imgui_metrics.menu_item();
					m_imgui_about.menu_item();
				});
			};

			// draw
			ML_scoped_imgui_id(ML_addressof(this));
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
				m_gui_nodes		.render(&demo::show_nodes_gui		, this); // NODES
				m_gui_memory	.render(&demo::show_memory_gui		, this); // MEMORY
				m_gui_docs		.render(&demo::show_documents_gui	, this); // DOCS
				m_gui_renderer	.render(&demo::show_renderer_gui	, this); // RENDERER
			}
		}

		void on_unload(unload_event const &)
		{
			// unload stuff, etc...

			m_ecs.clear();
			m_images.clear();
			m_shaders.clear();
			m_materials.clear();
			m_meshes.clear();
			m_textures.clear();
			m_fonts.clear();

			ax::NodeEditor::DestroyEditor(m_node_editor);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_assets_gui()
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			auto draw_asset = [&](auto const & n, auto const & v)
			{
				ML_scoped_imgui_id(ML_addressof(&v));

				// type
				using T = typename std::decay_t<decltype(v)>;
				static constexpr auto ftype{ nameof<>::filter_all(typeof_v<T>.name()) };
				char type[64] = ""; std::sprintf(type, "%.*s", (uint32_t)ftype.size(), ftype.data());

				// name
				pmr::string const name = util::to_string(n);

				// size
				char size[20] = ""; std::sprintf(size, "%u", (uint32_t)sizeof(T));

				// address
				char addr[sizeof(size_t) * 2 + 1] = ""; std::sprintf(addr, "%p", &v);

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

			m_fonts		.for_each([&](auto && ... args) { draw_asset(ML_forward(args)...); });
			m_images	.for_each([&](auto && ... args) { draw_asset(ML_forward(args)...); });
			m_materials	.for_each([&](auto && ... args) { draw_asset(ML_forward(args)...); });
			m_meshes	.for_each([&](auto && ... args) { draw_asset(ML_forward(args)...); });
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
			static gui::texture_preview preview{};

			shared<gl::texture2d> const & tex{ m_pipeline.back()->get_color_attachment() };
			preview.tex_addr = tex->get_handle();
			preview.tex_size = tex->get_size();
			
			preview.render([&]() noexcept
			{
				m_resolution = util::maintain(m_resolution, ImGui::GetContentRegionAvail());
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_documents_gui()
		{
			static ImGui::TextEditor text{};

			static ML_scope // once
			{
				text.SetLanguageDefinition(ImGui::TextEditor::LanguageDefinition::CPlusPlus());
				text.SetText("// work in progress\n\nint main()\n{\n\treturn 0;\n}");
			};

			// menu bar
			if (ImGui::BeginMenuBar())
			{
				gui::help_marker("WIP");
				ImGui::Separator();
				if (ImGui::MenuItem("new")) {}
				if (ImGui::MenuItem("open")) {}
				if (ImGui::MenuItem("save")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("undo")) { text.Undo(); }
				if (ImGui::MenuItem("redo")) { text.Redo(); }
				ImGui::Separator();
				if (ImGui::MenuItem("cut")) { text.Cut(); }
				if (ImGui::MenuItem("copy")) { text.Copy(); }
				if (ImGui::MenuItem("paste")) { text.Paste(); }
				ImGui::Separator();
				ImGui::EndMenuBar();
			}

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
				ML_scoped_imgui_id(ML_addressof(&value));

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
						ImGui::DragScalar("##value", ImGuiDataType_U32 , &temp, 0.2f);
					}
				}
				// POINTER
				else if constexpr (std::is_pointer_v<T>)
				{
					char buf[sizeof(size_t) * 2 + 1] = ""; std::sprintf(buf, "%p", value);
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
						ML_scoped_imgui_id(i);
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
				ML_scoped_imgui_id(static_cast<int32_t>(e));
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
						ML_scoped_imgui_id(&c);
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
						ML_scoped_imgui_id(ML_addressof(&rec));
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
				char progress[32] = ""; std::sprintf(progress,
					"%u / %u (%.2f%%)",
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

			// total time
			ImGui::Columns(2);
			ImGui::Selectable("total time"); ImGui::NextColumn();
			ImGui::Text("%.3fs", engine::time().total_time().count()); ImGui::NextColumn();
			ImGui::Columns(1);
			ImGui::Separator();

			// plots
			m_plots.for_each([&](gui::plot & p)
			{
				ML_scoped_imgui_id(ML_addressof(&p));
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
			if (static auto const & frame{ performance_tracker::prev() }; !frame.empty())
			{
				ImGui::Separator();
				for (auto const & elem : frame)
				{
					char time[20] = ""; std::sprintf(time, "%.7fs", elem.second.count());
					ImGui::Selectable(elem.first); gui::tooltip(time); ImGui::NextColumn();
					ImGui::Selectable(time); gui::tooltip(elem.first); ImGui::NextColumn();
				}
			}
			ImGui::Separator();
			ImGui::Columns(1);

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_renderer_gui()
		{
			static auto api{ gl::render_api::get() };
			static auto const & info{ api->get_info() };

			if (ImGui::BeginMenuBar())
			{
				ImGui::EndMenuBar();
			}

			ImGui::Text("renderer: %s", info.renderer.c_str());
			ImGui::Text("vendor:   %s", info.vendor.c_str());
			ImGui::Text("version:  %s", info.version.c_str());
			ImGui::Separator();

			if (ImGui::CollapsingHeader("alpha"))
			{
				bool a_enabled{ api->get_alpha_enabled() };
				auto a_fn{ api->get_alpha_fn() };
				ImGui::Checkbox("enabled", &a_enabled);
				ImGui::Text("predicate: %s (%u)", gl::predicate_names[a_fn.pred], a_fn.pred);
				ImGui::Text("reference: %f", a_fn.ref);
			}
			ImGui::Separator();

			if (ImGui::CollapsingHeader("blend"))
			{
				bool b_enabled{ api->get_blend_enabled() };
				auto b_color{ api->get_blend_color() };
				auto b_eq{ api->get_blend_eq() };
				auto b_fn{ api->get_blend_fn() };
				ImGui::Checkbox("enabled", &b_enabled);
				ImGui::ColorEdit4("color", b_color);
				ImGui::Text("mode rgb: %s (%u)", gl::function_names[b_eq.modeRGB], b_eq.modeRGB);
				ImGui::Text("mode alpha: %s (%u)", gl::function_names[b_eq.modeAlpha], b_eq.modeAlpha);
				ImGui::Text("src factor rgb: %s (%u)", gl::factor_names[b_fn.sfactorRGB], b_fn.sfactorRGB);
				ImGui::Text("src factor alpha: %s (%u)", gl::factor_names[b_fn.sfactorAlpha], b_fn.sfactorAlpha);
				ImGui::Text("dst factor rgb: %s (%u)", gl::factor_names[b_fn.dfactorRGB], b_fn.dfactorRGB);
				ImGui::Text("dst factor alpha: %s (%u)", gl::factor_names[b_fn.dfactorAlpha], b_fn.dfactorAlpha);
			}
			ImGui::Separator();

			if (ImGui::CollapsingHeader("cull"))
			{
				bool c_enabled{ api->get_cull_enabled() };
				auto c_facet{ api->get_cull_facet() };
				auto c_order{ api->get_cull_order() };
				ImGui::Checkbox("enabled", &c_enabled);
				ImGui::Text("facet: %s (%u)", gl::facet_names[c_facet], c_facet);
				ImGui::Text("order: %s (%u)", gl::order_names[c_order], c_order);
			}
			ImGui::Separator();

			if (ImGui::CollapsingHeader("depth"))
			{
				bool d_enabled{ api->get_depth_enabled() };
				auto d_pred{ api->get_depth_pr() };
				bool d_mask{ api->get_depth_mask() };
				auto d_range{ api->get_depth_range() };
				ImGui::Checkbox("enabled", &d_enabled);
				ImGui::Text("predicate: %s (%u) ", gl::predicate_names[d_pred], d_pred);
				ImGui::Checkbox("mask", &d_mask);
				ImGui::Text("range: %f, %f", d_range.nearVal, d_range.farVal);
			}
			ImGui::Separator();

			if (ImGui::CollapsingHeader("stencil"))
			{
				bool s_enabled{ api->get_stencil_enabled() };
				auto s_fn{ api->get_stencil_fn() };
				ImGui::Checkbox("enabled", &s_enabled);
				ImGui::Text("predicate: %s (%u)", gl::predicate_names[s_fn.pred], s_fn.pred);
				ImGui::Text("reference: %i", s_fn.ref);
				ImGui::Text("mask: %u", s_fn.mask);
			}
			ImGui::Separator();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern "C" ML_PLUGIN_API ml::plugin * ml_plugin_main(void *)
{
	static ml::plugin * temp{};
	return temp ? temp : temp = new ml::demo{};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */