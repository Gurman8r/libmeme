/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/ECS.hpp>
#include <libmeme/Core/StreamSniper.hpp>
#include <libmeme/Engine/Application.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/ImGuiExt.hpp>
#include <libmeme/Engine/API_Embed.hpp>
#include <libmeme/Graphics/Camera.hpp>
#include <libmeme/Graphics/Font.hpp>
#include <libmeme/Graphics/Mesh.hpp>
#include <libmeme/Graphics/Shader.hpp>
#include <libmeme/Graphics/Renderer.hpp>
#include <libmeme/Window/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// ECS
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// tags
	struct	t_default {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// components
	struct	c_transform	{ vec3 pos; vec4 rot; vec3 scl; };
	using	c_shader	= shader_asset;
	using	c_uniforms	= shared<uniform_buffer>;
	using	c_mesh		= shared<mesh>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// update uniforms
	ML_decl_ecs_sx(s_update_uniforms, x_update_uniforms, c_uniforms, c_transform)
	{
		void operator()(c_uniforms & ufm, c_transform const & tf)
		{
			ufm->set<vec3>("u_position"	, tf.pos)
				.set<vec4>("u_rotation"	, tf.rot)
				.set<vec3>("u_scale"	, tf.scl);
		}
	};

	// upload uniforms
	ML_decl_ecs_sx(s_upload_uniforms, x_upload_uniforms, c_shader, c_uniforms)
	{
		void operator()(c_shader & shd, c_uniforms const & ufm)
		{
			shd.bind();
			for (uniform const & u : *ufm)
			{
				shd.set_uniform(u);
			}
			shd.unbind();
		}
	};

	// render meshes
	ML_decl_ecs_sx(s_render_meshes, x_render_meshes, c_shader, c_mesh)
	{
		void operator()(c_shader const & shd, c_mesh const & msh, gfx::render_context * ctx)
		{
			shd.bind();
			shd.bind_textures();
			ctx->draw(msh->get_vao());
			shd.unbind();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// manager
	using demo_ecs = ecs::manager<ecs::detail::traits<

		// tags
		ecs::detail::tags<
		t_default
		>,

		// components
		ecs::detail::components<
		c_shader, c_uniforms, c_mesh, c_transform
		>,

		// signatures
		ecs::detail::signatures<
		s_update_uniforms, s_upload_uniforms, s_render_meshes
		>,

		// systems
		ecs::detail::systems<
		x_update_uniforms, x_upload_uniforms, x_render_meshes
		>,

		// options
		ecs::detail::options<>
	>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// DEMO
namespace ml
{
	struct ML_PLUGIN_API demo final : plugin
	{
		// ASSETS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ds::map< pmr::string, shared<gfx::texture2d>> m_textures	{};
		ds::map< pmr::string, shared<font>			> m_fonts		{};
		ds::map< pmr::string, shared<uniform_buffer>> m_uniforms	{};
		ds::map< pmr::string, shader_asset			> m_shaders		{};
		ds::map< pmr::string, shared<image>			> m_images		{};
		ds::map< pmr::string, shared<mesh>			> m_meshes		{};


		// ECS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		demo_ecs m_ecs{};


		// RENDERING
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		perspective_camera m_camera{};
		
		shader_cache m_cache{};

		vec2 m_resolution{ 1280, 720 };

		pmr::vector<shared<gfx::framebuffer>> m_fbo{};


		// GUI
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		gui::widget
			m_imgui_demo	{ "Dear ImGui Demo"		, 0, ImGuiWindowFlags_None },
			m_imgui_metrics	{ "Dear ImGui Metrics"	, 0, ImGuiWindowFlags_None },
			m_imgui_about	{ "About Dear ImGui"	, 0, ImGuiWindowFlags_None },
			m_gui_assets	{ "assets##demo"		, 1, ImGuiWindowFlags_None },
			m_gui_console	{ "console##demo"		, 1, ImGuiWindowFlags_None },
			m_gui_docs		{ "documents##demo"		, 1, ImGuiWindowFlags_MenuBar },
			m_gui_ecs		{ "ecs##demo"			, 1, ImGuiWindowFlags_None },
			m_gui_json		{ "json editor##demo"	, 1, ImGuiWindowFlags_None },
			m_gui_memory	{ "memory##demo"		, 1, ImGuiWindowFlags_MenuBar },
			m_gui_nodes		{ "node editor##demo"	, 1, ImGuiWindowFlags_None },
			m_gui_profiler	{ "profiler##demo"		, 1, ImGuiWindowFlags_None },
			m_gui_renderer	{ "renderer##demo"		, 1, ImGuiWindowFlags_MenuBar },
			m_gui_viewport	{ "viewport##demo"		, 1, ImGuiWindowFlags_NoScrollbar };

		stream_sniper m_cout{ &std::cout };

		gui::console m_console{};

		scoped<ax::NodeEditor::EditorContext> m_node_editor{ ax::NodeEditor::CreateEditor() };

		static constexpr size_t FPS_MAX{ 240 };

		gui::plot_controller m_plots
		{
			gui::plot::create(120
				, gui::plot::histogram
				, "##frame time"
				, []() noexcept { return "%.3f ms/frame"; }
				, [&]() noexcept { return 1000.f / get_io()->frame_rate; }
				, vec2{ 0.f, 64.f }
				, vec2{ FLT_MAX, FLT_MAX }),

			gui::plot::create(120
				, gui::plot::histogram
				, "##frame rate"
				, []() noexcept { return "%.1f fps"; }
				, [&]() noexcept { return get_io()->frame_rate; }
				, vec2{ 0.f, 64.f }
				, vec2{ FLT_MAX, FLT_MAX }),
		};

		MemoryEditor m_mem_editor{};

		void highlight_memory(byte_t * ptr, size_t const size)
		{
			static auto const testres{ get_memory()->resource() };
			auto const addr{ std::distance(testres->begin(), ptr) };
			m_gui_memory.focus();
			m_mem_editor.GotoAddrAndHighlight((size_t)addr, (size_t)addr + size);
		}

		template <class T> void highlight_memory(T const * ptr)
		{
			highlight_memory((byte_t *)ptr, sizeof(T));
		}


		// DEMO
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		demo(application * app, void * user) noexcept : plugin{ app, user }
		{
			subscribe<	load_event				>();
			subscribe<	update_event			>();
			subscribe<	dockspace_event			>();
			subscribe<	main_menu_bar_event		>();
			subscribe<	gui_event				>();
			subscribe<	window_key_event		>();
			subscribe<	window_mouse_event		>();
			subscribe<	window_cursor_pos_event	>();
		}

		~demo() noexcept override {}

		void on_event(event const & value) override
		{
			switch (value)
			{
			case load_event			::ID: return on_load	((load_event const &)value);
			case update_event		::ID: return on_update	((update_event const &)value);
			case dockspace_event	::ID: return on_dock	((dockspace_event const &)value);
			case main_menu_bar_event::ID: return on_menu	((main_menu_bar_event const &)value);
			case gui_event			::ID: return on_gui		((gui_event const &)value);
			
			case window_key_event::ID: {
				switch (auto const & ev{ (window_key_event const &)value }; ev.key)
				{
				default:
					get_io()->keyboard[ev.key] = ev.action;
				case key_code_w: {
					switch (ev.action)
					{
					case 0: { debug::puts("w up"); } break;
					case 1: { debug::puts("w down"); } break;
					case 2: { debug::puts("w hold"); } break;
					}
				} break;
				}
			} break;

			case window_mouse_event::ID: {
				switch (auto const & ev{ (window_mouse_event const &)value }; ev.button)
				{
				case mouse_button_0: {} break;
				case mouse_button_1: {} break;
				case mouse_button_2: {} break;
				}
			} break;

			case window_cursor_pos_event::ID: {
				auto const & ev{ (window_cursor_pos_event const &)value };
				float64_t const x = ev.x, y = ev.y;
			} break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_load(load_event const & ev)
		{
			// load stuff, etc...
			auto const sys	{ ev->get_system() };
			auto const mem	{ sys->mem };
			auto const io	{ sys->io };
			auto const win	{ sys->win };

			// ICON
			if (image const icon{ io->path2("assets/textures/icon.png"), 0, false })
			{
				win->set_icon(icon.width(), icon.height(), icon.data());
			}

			// FRAMEBUFFERS
			{
				m_fbo.push_back(gfx::framebuffer::create({ "FBO_0", m_resolution }));
			}

			// IMAGES
			{
				m_images["default"] = mem->make_ref<image>(image::get_default_rgba());
			}

			// TEXTURES
			{
				m_textures["default"	] = gfx::texture2d::create(*m_images["default"]);
				m_textures["doot"		] = gfx::texture2d::create(io->path2("assets/textures/doot.png"));
				m_textures["navball"	] = gfx::texture2d::create(io->path2("assets/textures/navball.png"));
				m_textures["earth_dm_2k"] = gfx::texture2d::create(io->path2("assets/textures/earth/earth_dm_2k.png"));
				m_textures["earth_sm_2k"] = gfx::texture2d::create(io->path2("assets/textures/earth/earth_sm_2k.png"));
				m_textures["moon_dm_2k"	] = gfx::texture2d::create(io->path2("assets/textures/moon/moon_dm_2k.png"));
			}

			// FONTS
			{
				m_fonts["clacon"		] = mem->make_ref<font>(io->path2("assets/fonts/clacon.ttf"));
				m_fonts["consolas"		] = mem->make_ref<font>(io->path2("assets/fonts/consolas.ttf"));
				m_fonts["lucida_console"] = mem->make_ref<font>(io->path2("assets/fonts/lucida_console.ttf"));
				m_fonts["minecraft"		] = mem->make_ref<font>(io->path2("assets/fonts/minecraft.ttf"));
			}

			// SHADERS
			{
				using namespace gfx;

				m_cache.read_file(gfx::shader_type_vertex, "vs_2D",
					io->path2("assets/shaders/2D.vs.shader"));

				m_cache.read_file(gfx::shader_type_vertex, "vs_3D",
					io->path2("assets/shaders/3D.vs.shader"));

				m_cache.read_file(gfx::shader_type_fragment, "fs_basic",
					io->path2("assets/shaders/basic.fs.shader"));

				m_shaders["basic_2D"].load_from_memory
				(
					m_cache.str(gfx::shader_type_vertex		, "vs_2D"),
					m_cache.str(gfx::shader_type_fragment	, "fs_basic")
				);

				m_shaders["basic_3D"].load_from_memory
				(
					m_cache.str(gfx::shader_type_vertex		, "vs_3D"),
					m_cache.str(gfx::shader_type_fragment	, "fs_basic")
				);
			}

			// UNIFORMS
			{
				// timers
				auto const _timers = uniform_buffer
				{
					make_uniform<float_t>("u_time"	, [io]() { return (float_t)io->main_timer.elapsed().count(); }),
					make_uniform<float_t>("u_delta"	, [io]() { return (float_t)io->loop_timer.elapsed().count(); })
				};

				// camera
				auto const _camera = uniform_buffer
				{
					make_uniform<vec3	>("u_cam.pos"	, vec3{ 0, 0, 0.f }),
					make_uniform<vec3	>("u_cam.dir"	, vec3{ 0, 0, 1.f }),
					make_uniform<float_t>("u_cam.fov"	, 45.f * util::deg2rag_v<float_t>),
					make_uniform<float_t>("u_cam.near"	, 0.0001f),
					make_uniform<float_t>("u_cam.far"	, 1000.0f),
					make_uniform<vec2	>("u_cam.view"	, vec2{ 1280.f, 720.f })
				};

				// transform
				auto const _tf = uniform_buffer
				{
					make_uniform<vec3	>("u_position"	, vec3{}),
					make_uniform<vec4	>("u_rotation"	, vec4{}),
					make_uniform<vec3	>("u_scale"		, vec3{})
				};

				// 3d
				auto const _3d = uniform_buffer
				{
					make_uniform<color>("u_color", colors::white),
					make_uniform<gfx::texture2d>("u_texture0", m_textures["default"])
				}
				+ _timers + _camera + _tf;

				// earth
				auto & earth{ m_uniforms["earth"] = mem->make_ref<uniform_buffer>(_3d) };
				earth->set<gfx::texture2d>("u_texture0", m_textures["earth_dm_2k"]);

				// moon
				auto & moon{ m_uniforms["moon"] = mem->make_ref<uniform_buffer>(_3d) };
				moon->set<gfx::texture2d>("u_texture0", m_textures["moon_dm_2k"]);
			}

			// MESHES
			{
				m_meshes["sphere8x6"	] = mem->make_ref<mesh>(io->path2("assets/models/sphere8x6.obj"));
				m_meshes["sphere32x24"	] = mem->make_ref<mesh>(io->path2("assets/models/sphere32x24.obj"));
				m_meshes["monkey"		] = mem->make_ref<mesh>(io->path2("assets/models/monkey.obj"));

				m_meshes["triangle"] = mem->make_ref<mesh>(mesh
				{
					{
						vertex{ {  0.0f,  0.5f, 0.0f }, vec3::one(), { 0.5f, 1.0f } },
						vertex{ {  0.5f, -0.5f, 0.0f }, vec3::one(), { 1.0f, 0.0f } },
						vertex{ { -0.5f, -0.5f, 0.0f }, vec3::one(), { 0.0f, 0.0f } },
					},
					{
						0, 1, 2,
					}
				});

				m_meshes["quad"] = mem->make_ref<mesh>(mesh
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
				});

				m_meshes["skybox"] = mem->make_ref<mesh>(mesh
				{
					{
						vertex{ { -1.0f,  1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f, -1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f, -1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f, -1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f,  1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f,  1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f, -1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f, -1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f,  1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f,  1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f,  1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f, -1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f, -1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f, -1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f,  1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f,  1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f,  1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f, -1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f, -1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f,  1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f,  1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f,  1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f, -1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f, -1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f,  1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f,  1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f,  1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f,  1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f,  1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f,  1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f, -1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f, -1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f, -1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f, -1.0f, -1.0f }, vec3::one(), vec2::zero() },
						vertex{ { -1.0f, -1.0f,  1.0f }, vec3::one(), vec2::zero() },
						vertex{ {  1.0f, -1.0f,  1.0f }, vec3::one(), vec2::zero() },
					}
				});
			}

			// ENTITIES
			{
				ML_defer(&){ m_ecs.apply_changes(); };

				auto make_renderer = [&](auto shd, auto uni, auto msh, auto tf) noexcept
				{
					auto & h{ m_ecs.create_handle() };
					h.add_tag<t_default>();
					h.add_component<c_shader>	(m_shaders	[shd]);
					h.add_component<c_uniforms>	(m_uniforms	[uni]);
					h.add_component<c_mesh>		(m_meshes	[msh]);
					h.add_component<c_transform>(tf);
					return h;
				};
				
				auto & earth = make_renderer("basic_3D", "earth", "sphere32x24", c_transform{
					vec3{ -.5f, -.2f, 5.f },
					vec4{ 0.0f, 0.1f, 0.0f, -.15f },
					vec3::fill(1.f)
					});

				auto & moon = make_renderer("basic_3D", "moon", "monkey", c_transform{ // sphere8x6
					vec3{ 1.f, .2f, 5.f },
					vec4{ 0.0f, 0.1f, 0.0f, .25f },
					vec3::fill(.27f)
					});
			}
		}

		void on_update(update_event const & ev)
		{
			// update stuff, etc...
			auto const sys	{ ev->get_system() };
			auto const io	{ sys->io };

			// console
			m_console.dump(m_cout.sstr());

			// plots
			m_plots.update(io->main_timer.elapsed().count());
			
			// uniforms
			m_ecs.run_system<x_update_uniforms>();
			m_ecs.run_system<x_upload_uniforms>();

			// resize fb
			for (auto & fbo : m_fbo) { fbo->resize(m_resolution); }

			// render
			for (auto const & cmd : {
				gfx::command::bind_framebuffer(m_fbo[0]),
				gfx::command::set_clear_color(colors::magenta),
				gfx::command::clear(gfx::clear_color | gfx::clear_depth),
				gfx::command([&](gfx::render_context * ctx) noexcept
				{
					m_ecs.run_system<x_render_meshes>(ctx);
				}),
				gfx::command::bind_framebuffer(nullptr),
			})	gfx::execute(cmd, sys->win->get_render_context());
		}

		void on_dock(dockspace_event const & ev)
		{
			enum : int32_t // nodes
			{
				root,
				left, left_up, left_dn,
				right, right_up, right_dn,
				MAX_DOCK_NODE
			};
			
			// setup dockspace
			auto & d{ *ev };
			if (!d.nodes.empty()) { return; } else { d.nodes.resize(MAX_DOCK_NODE); }
			if (!(d[root] = d.begin_builder(ImGuiDockNodeFlags_None))) { return; }
			ML_defer(&d) { d.end_builder(root); };

			constexpr float_t lhs{ 0.465f }, rhs{ 1.f - lhs };

			// split nodes
			d.split(left	, d[root]	, ImGuiDir_Left	, lhs	, &d[root]);	// left
			d.split(left_up	, d[left]	, ImGuiDir_Up	, 0.5f	, &d[left]);	// left-up
			d.split(left_dn	, d[left]	, ImGuiDir_Down	, 0.5f	, &d[left]);	// left-down
			d.split(right	, d[root]	, ImGuiDir_Right, rhs	, &d[root]);	// right
			d.split(right_up, d[right]	, ImGuiDir_Up	, 0.5f	, &d[right]);	// right-up
			d.split(right_dn, d[right]	, ImGuiDir_Down	, 0.5f	, &d[right]);	// right-down

			// dock windows
			d.dock(m_gui_viewport	, d[left_up]);
			d.dock(m_gui_ecs		, d[left_dn]);
			d.dock(m_gui_assets		, d[left_dn]);
			d.dock(m_gui_renderer	, d[left_dn]);
			d.dock(m_gui_profiler	, d[left_dn]);
			d.dock(m_gui_json		, d[left_dn]);
			d.dock(m_gui_memory		, d[right]);
			d.dock(m_gui_docs		, d[right]);
			d.dock(m_gui_nodes		, d[right]);
			d.dock(m_gui_console	, d[right]);
		}

		void on_menu(main_menu_bar_event const & ev)
		{
			ML_ImGui_ScopeID(this);

			// FILE
			if (ImGui::BeginMenu("file"))
			{
				if (ImGui::MenuItem("quit", "alt+f4"))
				{
					get_window()->set_should_close(true);
				}
				ImGui::EndMenu();
			}
			// VIEW
			if (ImGui::BeginMenu("view"))
			{
				m_gui_assets.menu_item();
				m_gui_console.menu_item();
				m_gui_docs.menu_item();
				m_gui_ecs.menu_item();
				m_gui_json.menu_item();
				m_gui_memory.menu_item();
				m_gui_nodes.menu_item();
				m_gui_profiler.menu_item();
				m_gui_renderer.menu_item();
				m_gui_viewport.menu_item();
				ImGui::EndMenu();
			}
			// HELP
			if (ImGui::BeginMenu("help"))
			{
				m_imgui_demo.menu_item();
				m_imgui_metrics.menu_item();
				m_imgui_about.menu_item();
				ImGui::EndMenu();
			}

			ImGui::Separator();
			ImGui::TextDisabled("%.1f fps", get_io()->frame_rate);
			ImGui::Separator();
		}

		void on_gui(gui_event const & ev)
		{
			ML_ImGui_ScopeID(this);

			// IMGUI
			if (m_imgui_demo.open)		{ ev->show_imgui_demo(&m_imgui_demo.open); }
			if (m_imgui_metrics.open)	{ ev->show_imgui_metrics(&m_imgui_metrics.open); }
			if (m_imgui_about.open)		{ ev->show_imgui_about(&m_imgui_about.open); }

			// WIDGETS
			m_gui_viewport	.render(&demo::show_viewport_gui	, this); // VIEWPORT
			m_gui_ecs		.render(&demo::show_ecs_gui			, this); // ECS
			m_gui_assets	.render(&demo::show_assets_gui		, this); // ASSETS
			m_gui_renderer	.render(&demo::show_renderer_gui	, this); // RENDERER
			m_gui_json		.render(&demo::show_json_gui		, this); // JSON
			m_gui_profiler	.render(&demo::show_profiler_gui	, this); // PROFILER
			m_gui_nodes		.render(&demo::show_nodes_gui		, this); // NODES
			m_gui_memory	.render(&demo::show_memory_gui		, this); // MEMORY
			m_gui_docs		.render(&demo::show_documents_gui	, this); // DOCS
			m_gui_console	.render(&demo::show_console_gui		, this); // CONSOLE
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_assets_gui()
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			auto draw_asset = [&](auto const & n, auto const & v)
			{
				ML_ImGui_ScopeID(&v);

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

			m_fonts		.for_each([&](auto && ... x) { draw_asset(ML_forward(x)...); });
			m_images	.for_each([&](auto && ... x) { draw_asset(ML_forward(x)...); });
			m_uniforms	.for_each([&](auto && ... x) { draw_asset(ML_forward(x)...); });
			m_meshes	.for_each([&](auto && ... x) { draw_asset(ML_forward(x)...); });
			m_shaders	.for_each([&](auto && ... x) { draw_asset(ML_forward(x)...); });
			m_textures	.for_each([&](auto && ... x) { draw_asset(ML_forward(x)...); });

			ImGui::Columns(1);

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_console_gui()
		{
			// render console
			ML_defer(&){ m_console.render(); };

			// setup commands
			if ML_LIKELY(!m_console.commands.empty()) { return; }

			m_console.commands.push_back({ "clear", [&](auto && args) noexcept
			{
				m_console.clear();
			},
			{
				"clear the console window",
			} });

			m_console.commands.push_back({ "exit", [&](auto && args) noexcept
			{
				get_window()->set_should_close(true);
			},
			{
				"shutdown the application",
			} });

			m_console.commands.push_back({ "help", [&](auto && args) noexcept
			{
				if (args.empty())
				{
					for (auto const & cmd : m_console.commands)
					{
						std::cout << cmd.name << '\n';
					}
				}
				else
				{
					if (auto const it{ std::find_if(
						m_console.commands.begin(),
						m_console.commands.end(),
						[&](auto & e) noexcept { return (e.name == args.front()); }) }
					; it != m_console.commands.end())
					{
						for (auto const & str : it->info)
						{
							std::cout << str << '\n';
						}
					}
					else
					{
						std::cout << "unknown command: " << args.front() << '\n';
					}
				}
			},
			{
				"display list of commands",
				"help",
				"help [CMD]",
			} });

			m_console.commands.push_back({ "history", [&](auto && args) noexcept
			{
				for (auto const & str : m_console.history)
				{
					std::cout << str << '\n';
				}
			},
			{
				"display command history",
			} });

			m_console.commands.push_back({ "py", [&](auto && args) noexcept
			{
				if (args.empty() && m_console.lock("py"))
				{
					static ML_scope(&){ std::cout << "# type \'\\\' to stop using python\n"; };
				}
				else if (args.front() == "\\" && m_console.unlock("py"))
				{
					m_console.cmd_lock = nullptr;
				}
				else
				{
					get_scripts()->do_string(util::detokenize(args));
				}
			},
			{
				"execute python code",
			} });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_documents_gui()
		{
			static ImGui::TextEditor text{};

			static ML_scope() // setup text editor
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
				ML_ImGui_ScopeID(&value);

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
							get_window()->set_clipboard(buf);
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
				else if constexpr (std::is_same_v<T, demo_ecs::signature>)
				{
					auto const & style			{ ImGui::GetStyle() };
					auto const button_width		{ ImGui::GetFrameHeight() };
					auto const button_count		{ static_cast<int32_t>(value.size()) };
					auto const window_visible	{ ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x };

					int32_t i{};
					meta::for_types<meta::concat<
						demo_ecs::component_list,
						demo_ecs::tag_list>
					>([&](auto type)
					{
						ML_ImGui_ScopeID(i);
						bool temp{ value.read((size_t)i) };
						ImGui::Checkbox("##value", &temp);

						using U = typename demo_ecs::traits;
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
							demo_ecs::traits::component_id<C>(),
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

		void show_json_gui()
		{

		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_memory_gui()
		{
			static passthrough_resource * const testres{ get_memory()->resource() };

			static ML_scope(&) // setup memory editor
			{
				m_mem_editor.Open				= true;
				m_mem_editor.ReadOnly			= true;
				m_mem_editor.Cols				= get_window()->is_maximized() ? 32 : 16;
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
				auto const & records{ get_memory()->records().values() };
				static auto selected_record{ &records.front() };
				char selected_address[20] = "highlight";
				if (selected_record)
				{
					std::sprintf(selected_address, "%p", selected_record->addr);
				}
				if (ImGui::BeginCombo("##records", selected_address, 0))
				{
					static auto const initial_width{ ImGui::GetContentRegionAvailWidth() };
					ImGui::Columns(3);

					static ML_scope(&){ ImGui::SetColumnWidth(-1, initial_width * 0.50f); };
					ImGui::Text("address"); ImGui::NextColumn();

					static ML_scope(&){ ImGui::SetColumnWidth(-1, initial_width * 0.25f); };
					ImGui::Text("index"); ImGui::NextColumn();

					static ML_scope(&){ ImGui::SetColumnWidth(-1, initial_width * 0.25f); };
					ImGui::Text("size"); ImGui::NextColumn();

					ImGui::Separator();
					for (auto const & rec : records)
					{
						ML_ImGui_ScopeID(&rec);
						char addr[20] = ""; std::sprintf(addr, "%p", rec.addr);
						bool const pressed{ ImGui::Selectable(addr) }; ImGui::NextColumn();
						ImGui::TextDisabled("%u", rec.index); ImGui::NextColumn();
						ImGui::TextDisabled("%u", rec.size); ImGui::NextColumn();
						if (pressed)
						{
							selected_record = &rec;
							highlight_memory((byte_t *)rec.addr, rec.size);
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
			m_mem_editor.DrawContents(testres->buffer(), testres->capacity(), testres->base_addr());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_nodes_gui()
		{
			namespace gui = ax::NodeEditor;

			auto & io{ ImGui::GetIO() };
			
			ImGui::Separator();
			
			gui::SetCurrentEditor(m_node_editor.get());
			gui::Begin("My Editor", { 0.f, 0.f });
			
			int32_t uniqueId{ 1 };

			gui::BeginNode(uniqueId++);
			{
				ImGui::Text("Node A");
				gui::BeginPin(uniqueId++, gui::PinKind::Input);
				{
					ImGui::Text("-> In");
				}
				gui::EndPin();
				ImGui::SameLine();
				gui::BeginPin(uniqueId++, gui::PinKind::Output);
				{
					ImGui::Text("Out ->");
				}
				gui::EndPin();
			}
			gui::EndNode();

			gui::End();
			gui::SetCurrentEditor(nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_profiler_gui()
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			// total time
			ImGui::Columns(2);
			ImGui::Selectable("total time"); ImGui::NextColumn();
			ImGui::Text("%.3fs", get_io()->main_timer.elapsed().count()); ImGui::NextColumn();
			ImGui::Columns(1);
			ImGui::Separator();

			// plots
			for (gui::plot & p : m_plots)
			{
				ML_ImGui_ScopeID(&p);
				p.render();
				if (ImGui::BeginPopupContextItem("plot settings"))
				{
					ImGui::Checkbox("animate", &p.animate); ImGui::SameLine();
					ImGui::Combo("##mode", &p.mode, "lines\0histogram\0");
					ImGui::EndPopup();
				}
			};

			// benchmarks
			ImGui::Columns(1);
			
			//if (static auto const & prev{ performance::last_frame() }; !prev.empty())
			//{
			//	ImGui::Separator();
			//	for (auto const & e : prev)
			//	{
			//		char time_str[20] = ""; std::sprintf(time_str, "%.7fs", e.time.count());
			//		ImGui::Selectable(e.name); gui::tooltip(time_str); ImGui::NextColumn();
			//		ImGui::Selectable(time_str); gui::tooltip(e.name); ImGui::NextColumn();
			//	}
			//}
			//ImGui::Separator();
			//ImGui::Columns(1);

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_renderer_gui()
		{
			static auto const & dev	{ get_window()->get_render_device() };
			static auto const & inf	{ dev->get_info() };
			static auto const & ctx	{ dev->get_context() };

			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("%s", inf.vendor.c_str()); gui::tooltip("vendor"); ImGui::Separator();
				ImGui::Text("%s", inf.renderer.c_str()); gui::tooltip("renderer"); ImGui::Separator();
				ImGui::Text("%s", inf.version.c_str()); gui::tooltip("version"); ImGui::Separator();
				ImGui::Text("%s", inf.shading_language_version.c_str()); gui::tooltip("shading language version"); ImGui::Separator();
				ImGui::EndMenuBar();
			}
			ImGui::Separator();

			if (gfx::alpha_state a{}; ImGui::CollapsingHeader("alpha") && ctx->get_alpha_state(&a))
			{
				ImGui::Checkbox("enabled", &a.enabled);
				ImGui::Text("pred: %s (%u)", gfx::predicate_NAMES[a.pred], a.pred);
				ImGui::Text("ref: %f", a.ref);
			}
			ImGui::Separator();

			if (gfx::blend_state b{}; ImGui::CollapsingHeader("blend") && ctx->get_blend_state(&b))
			{
				ImGui::Checkbox("enabled", &b.enabled);
				ImGui::ColorEdit4("color", b.color);
				ImGui::Text("color equation: %s (%u)", gfx::equation_NAMES[b.color_equation], b.color_equation);
				ImGui::Text("color sfactor: %s (%u)", gfx::factor_NAMES[b.color_sfactor], b.color_sfactor);
				ImGui::Text("color dfactor: %s (%u)", gfx::factor_NAMES[b.color_dfactor], b.color_dfactor);
				ImGui::Text("alpha equation: %s (%u)", gfx::equation_NAMES[b.alpha_equation], b.alpha_equation);
				ImGui::Text("alpha sfactor: %s (%u)", gfx::factor_NAMES[b.alpha_sfactor], b.alpha_sfactor);
				ImGui::Text("alpha dfactor: %s (%u)", gfx::factor_NAMES[b.alpha_dfactor], b.alpha_dfactor);
			}
			ImGui::Separator();

			if (gfx::cull_state c{}; ImGui::CollapsingHeader("cull") && ctx->get_cull_state(&c))
			{
				ImGui::Checkbox("enabled", &c.enabled);
				ImGui::Text("facet: %s (%u)", gfx::facet_NAMES[c.facet], c.facet);
				ImGui::Text("order: %s (%u)", gfx::order_NAMES[c.order], c.order);
			}
			ImGui::Separator();

			if (gfx::depth_state d{}; ImGui::CollapsingHeader("depth") && ctx->get_depth_state(&d))
			{
				ImGui::Checkbox("enabled", &d.enabled);
				ImGui::Text("pred: %s (%u) ", gfx::predicate_NAMES[d.pred], d.pred);
				ImGui::Text("range: %f, %f", d.range[0], d.range[1]);
			}
			ImGui::Separator();

			if (gfx::stencil_state s{}; ImGui::CollapsingHeader("stencil") && ctx->get_stencil_state(&s))
			{
				ImGui::Checkbox("enabled", &s.enabled);
				ImGui::Text("front pred: %s (%u)", gfx::predicate_NAMES[s.front.pred], s.front.pred);
				ImGui::Text("front ref: %i", s.front.ref);
				ImGui::Text("front mask: %u", s.front.mask);
				ImGui::Text("back pred: %s (%u)", gfx::predicate_NAMES[s.back.pred], s.back.pred);
				ImGui::Text("back ref: %i", s.back.ref);
				ImGui::Text("back mask: %u", s.back.mask);
			}
			ImGui::Separator();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_viewport_gui()
		{
			static gui::texture_preview pview{};

			auto const & tex{ m_fbo.back()->get_color_attachments().front() };
			
			pview.tex_addr = tex->get_handle();
			
			pview.tex_size = tex->get_data().size;

			m_resolution = util::maintain(m_resolution, ImGui::GetContentRegionAvail());

			pview.render();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API ml::plugin * ml_plugin_attach(ml::application * app, void * user)
	{
		return app->get_memory()->new_object<ml::demo>(app, user);
	}
	
	ML_PLUGIN_API void ml_plugin_detach(ml::application * app, ml::plugin * ptr)
	{
		app->get_memory()->delete_object((ml::demo *)ptr);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */