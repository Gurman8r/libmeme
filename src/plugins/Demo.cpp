#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/Wrapper.hpp>
#include <libmeme/Core/ECS.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/ImGui.hpp>
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

namespace ml
{
	struct demo final : plugin
	{
		// (C) COMPONENTS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct c_transform	{ vec3 pos; vec4 rot; vec3 scl; };
		struct c_shader		: ds::value_wrapper<shader	> {};
		struct c_material	: ds::value_wrapper<material> {};
		struct c_model		: ds::value_wrapper<model	> {};


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
			ecs::cfg::tags<>,

			// components
			ecs::cfg::components<
			c_transform, c_shader, c_material, c_model
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

		ecs::manager<entity_traits> m_ecs{};

		pmr::vector<decltype(m_ecs)::handle> m_handles;


		// GUI
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool m_show_imgui_demo	{ false };
		bool m_show_debug{ true };
		bool m_show_display		{ true };


		// CONTENT
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pmr::vector<				render_texture	> m_pipeline	{};
		ds::flat_map<pmr::string,	font			> m_fonts		{};
		ds::flat_map<pmr::string,	image			> m_images		{};
		ds::flat_map<pmr::string,	material		> m_materials	{};
		ds::flat_map<pmr::string,	model			> m_models		{};
		ds::flat_map<pmr::string,	script			> m_scripts		{};
		ds::flat_map<pmr::string,	shader			> m_shaders		{};
		ds::flat_map<pmr::string,	texture			> m_textures	{};


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
			case hashof_v<enter_event	> : on_enter	(*value.as<enter_event>()	); break;
			case hashof_v<update_event	> : on_update	(*value.as<update_event>()	); break;
			case hashof_v<draw_event	> : on_draw		(*value.as<draw_event>()	); break;
			case hashof_v<gui_dock_event> : on_gui_dock	(*value.as<gui_dock_event>()); break;
			case hashof_v<gui_draw_event> : on_gui_draw	(*value.as<gui_draw_event>()); break;
			case hashof_v<exit_event	> : on_exit		(*value.as<exit_event>()	); break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_enter(enter_event const & ev)
		{
			// load stuff, etc...

			// MENUS
			{
				// File Menu
				editor::get_main_menu().add_menu("File", [&]()
				{
					ML_ImGui_ScopeID(ML_ADDRESSOF(this));
					if (ImGui::MenuItem("Quit", "Alt+F4"))
					{
						engine::get_window().close();
					}
				});

				// View Menu
				editor::get_main_menu().add_menu("View", [&]()
				{
					ML_ImGui_ScopeID(ML_ADDRESSOF(this));
					ImGui::MenuItem("libmeme demo", "", &m_show_debug);
				});

				// Option Menu
				editor::get_main_menu().add_menu("Option", [&]()
				{
					ML_ImGui_ScopeID(ML_ADDRESSOF(this));
					bool fullscreen{ engine::get_window().is_fullscreen() };
					if (ImGui::MenuItem("Fullscreen", "", &fullscreen))
					{
						engine::get_window().set_fullscreen(fullscreen);
					}
				});

				// Help Menu
				editor::get_main_menu().add_menu("Help", [&]()
				{
					ML_ImGui_ScopeID(ML_ADDRESSOF(this));
					ImGui::MenuItem("imgui demo", "", &m_show_imgui_demo);
				});
			}

			// PIPELINE
			{
				m_pipeline.emplace_back(
					make_render_texture(vec2i{ 1280, 720 })
				).create();
			}

			// IMAGES
			{
				if (auto const & img{ m_images["icon"] = make_image(
					fs::path{ "../../../../assets/textures/icon.png" }
				) }; !img.empty())
				{
					engine::get_window().set_icon(img.width(), img.height(), img.data());
				}
			}

			// TEXTURES
			{
				m_textures["doot"] = make_texture(
					fs::path{ "../../../../assets/textures/doot.png" }
				);

				m_textures["navball"] = make_texture(
					fs::path{ "../../../../assets/textures/navball.png" }
				);
			}

			// FONTS
			{
				m_fonts["clacon"] = make_font(
					fs::path{ "../../../../assets/fonts/clacon.ttf" }
				);

				m_fonts["consolas"] = make_font(
					fs::path{ "../../../../assets/fonts/consolas.ttf" }
				);

				m_fonts["lucida_console"] = make_font(
					fs::path{ "../../../../assets/fonts/lucida_console.ttf" }
				);

				m_fonts["minecraft"] = make_font(
					fs::path{ "../../../../assets/fonts/minecraft.ttf" }
				);
			}

			// SHADERS
			{
				m_shaders["2d"] = make_shader(
					fs::path{ "../../../../assets/shaders/2D.vs.shader" },
					fs::path{ "../../../../assets/shaders/basic.fs.shader" }
				);

				m_shaders["3d"] = make_shader(
					fs::path{ "../../../../assets/shaders/3D.vs.shader" },
					fs::path{ "../../../../assets/shaders/basic.fs.shader" }
				);
			}

			// MATERIALS
			{
				// timers
				auto const _timers = make_material(
					make_uniform<float_t>("u_time",		[]() { return engine::get_time().count<float_t>(); }),
					make_uniform<float_t>("u_delta",	[]() { return engine::get_runtime().delta_time; })
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
				auto const _transform = make_material(
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
					make_uniform<texture>("u_texture0", &m_textures["navball"])
				) + _timers + _camera + _transform;
			}

			// MODELS
			{
				m_models["sphere8x6"] = make_model(
					fs::path{ "../../../../assets/models/sphere8x6.obj" }
				);

				m_models["sphere32x24"] = make_model(
					fs::path{ "../../../../assets/models/sphere32x24.obj" }
				);

				// FIXME: upside down?
				m_models["monkey"] = make_model(
					fs::path{ "../../../../assets/models/monkey.obj" }
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
				ML_DEFER{ m_ecs.refresh(); };

				if (auto & h{ m_handles.emplace_back(m_ecs.create_handle()) })
				{
					h.add_component<c_shader>	(m_shaders	["3d"]);
					h.add_component<c_material>	(m_materials["3d"]);
					h.add_component<c_model>	(m_models	["sphere32x24"]);
					h.add_component<c_transform>(
						vec3{ 0.f, 0.f, 0.f },
						vec4{ 0.0f, 0.1f, 0.0f, 0.25f },
						vec3{ 1.f, 1.f, 1.f }
					);
				}
			}
			auto here = 0;
		}

		void on_update(update_event const & ev)
		{
			// update stuff, etc...

			m_ecs.update_system<x_apply_transforms>();
			m_ecs.update_system<x_apply_materials>();
		}

		void on_draw(draw_event const & ev)
		{
			// draw stuff, etc...

			if (m_pipeline.empty()) return;

			using M = typename decltype(m_ecs);
			using U = typename M::traits_type;

			static_assert(U::component_info()[0] == typeof_v<c_transform>);

			static_assert(U::system_info()[2] == typeof_v<
				ecs::util::x_wrapper<x_draw_renderers>>);

			if (render_texture const & rt{ m_pipeline[0] })
			{
				rt.bind();
				rt.clear_color(colors::magenta);
				rt.viewport(rt.bounds());
				constexpr render_states states{
					{}, {}, cull_state{ false }, {}
				}; states();
				m_ecs.update_system<x_draw_renderers>(rt);
				rt.unbind();
			}
		}

		void on_gui_dock(gui_dock_event const & ev)
		{
			// dock gui windows
			ev.d.dock_window("ml::debug",	ev.d.get_node(ev.d.Left));
			ev.d.dock_window("ml::display", ev.d.get_node(ev.d.Right));
		}

		void on_gui_draw(gui_draw_event const & ev)
		{
			// gui stuff, etc...

			ML_ImGui_ScopeID(ML_ADDRESSOF(this));

			// imgui demo
			if (m_show_imgui_demo)
			{
				editor::show_imgui_demo(&m_show_imgui_demo);
			}

			// debug
			if (m_show_debug)
			{
				if (ImGui::Begin("ml::debug", &m_show_debug, ImGuiWindowFlags_None))
				{
					if (ImGui::BeginTabBar("ml::debug##tabs",
						ImGuiTabBarFlags_Reorderable
					))
					{
						// performance
						if (ImGui::BeginTabItem("performance"))
						{
							// memory
							ImGui::Text("manual allocations: %u", memory_tracker::get_records().size());
							ImGui::Separator();

							ImGui::Columns(2);

							// total time
							ImGui::Text("total time"); ImGui::NextColumn();
							ImGui::Text("%.3fs", engine::get_time().count()); ImGui::NextColumn();

							// delta time
							ImGui::Text("delta time"); ImGui::NextColumn();
							ImGui::Text("%.7fs", engine::get_runtime().delta_time); ImGui::NextColumn();

							// frame rate
							ImGui::Text("frame rate"); ImGui::NextColumn();
							ImGui::Text("%.4ffps", ImGui::GetIO().Framerate); ImGui::NextColumn();

							// benchmarks
							if (auto const & prev{ performance_tracker::last_frame() }; !prev.empty())
							{
								ImGui::Separator();
								for (auto const & elem : prev)
								{
									ImGui::Text("%s", elem.first); ImGui::NextColumn();
									ImGui::Text("%.7fs", elem.second.count()); ImGui::NextColumn();
								}
							}

							ImGui::Separator();
							ImGui::Columns(1);
							ImGui::EndTabItem();
						}

						// ecs
						if (ImGui::BeginTabItem("ecs"))
						{
							m_ecs.for_entities([&](size_t const i)
							{
								m_ecs.for_components(i, [&](auto t, auto & c)
								{
									ImGui::Columns(2);
									ImGui::Text("T: %.*s", t.name().size(), t.name().data()); ImGui::NextColumn();
									ImGui::Text("ID: %u", t.hash()); ImGui::NextColumn();
									ImGui::Columns(1);
								});
							});

							ImGui::EndTabItem();
						}
						
						ImGui::EndTabBar();
					}
				}
				ImGui::End();
			}

			// display
			if (m_show_display)
			{
				if (ImGui::Begin("ml::display", &m_show_display))
				{
					editor::draw_texture_preview(
						m_pipeline[0].get_texture(),
						ImGui::GetContentRegionAvail()
					);
				}
				ImGui::End();
			}
		}

		void on_exit(exit_event const & ev)
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
			m_scripts.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U = ecs::traits<>
		> static auto edit_ecs(ecs::manager<U> & m)
		{

		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

extern "C" ML_PLUGIN_API ml::plugin * ml_plugin_main()
{
	static ml::plugin * p{};
	if (!p) p = new ml::demo{};
	return p;
}