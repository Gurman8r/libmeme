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
		// (T) TAGS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct t_renderer {};


		// (C) COMPONENTS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct c_transform	{ vec3 pos; vec4 rot; vec3 scl; };
		struct c_shader		: ds::value_wrapper<shader	> {};
		struct c_material	: ds::value_wrapper<material> {};
		struct c_model		: ds::value_wrapper<model	> {};


		// (S) SIGNATURES
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using s_apply_transform = meta::list<
			t_renderer, c_material, c_transform
		>;

		using s_apply_uniforms = meta::list<
			t_renderer, c_shader, c_material
		>;

		using s_draw_renderer = meta::list<
			t_renderer, c_shader, c_model
		>;


		// (X) SYSTEMS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class> struct x_apply_transform final : ecs::util::x_base<s_apply_transform>
		{
			void update(c_material & mat, c_transform const & tfm)
			{
				mat->set<vec3>("u_position", tfm.pos)
					.set<vec4>("u_rotation", tfm.rot)
					.set<vec3>("u_scale", tfm.scl);
			}
		};

		template <class> struct x_apply_uniforms final : ecs::util::x_base<s_apply_uniforms>
		{
			void update(c_shader & shd, c_material const & mat)
			{
				shd->bind(false);
				for (uniform const & u : *mat)
					shd->set_uniform(u);
				shd->unbind();
			}
		};

		template <class> struct x_draw_renderer final : ecs::util::x_base<s_draw_renderer>
		{
			void update(c_shader const & shd, c_model const & mod, render_target const & tgt)
			{
				shd->bind(true);
				tgt.draw(*mod);
				shd->unbind();
			}
		};


		// (M) MANAGER
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ecs::manager<ecs::traits<

			// tags
			ecs::cfg::tags<
			t_renderer
			>,

			// components
			ecs::cfg::components<
			c_transform, c_shader, c_material, c_model
			>,

			// signatures
			ecs::cfg::signatures<
			s_apply_transform, s_apply_uniforms, s_draw_renderer
			>,

			// systems
			ecs::cfg::systems<
			x_apply_transform, x_apply_uniforms, x_draw_renderer
			>
		>
		> m_ecs{};


		// GUI
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool m_show_libmeme_demo{ true };
		bool m_show_imgui_demo	{ false };


		// CONTENT
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ds::flat_map<pmr::string, render_texture	> m_pipeline	{};
		ds::flat_map<pmr::string, font				> m_fonts		{};
		ds::flat_map<pmr::string, image				> m_images		{};
		ds::flat_map<pmr::string, material			> m_materials	{};
		ds::flat_map<pmr::string, model				> m_models		{};
		ds::flat_map<pmr::string, script			> m_scripts		{};
		ds::flat_map<pmr::string, shader			> m_shaders		{};
		ds::flat_map<pmr::string, texture			> m_textures	{};


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
			case hashof_v<enter_event	> : on_enter	(*value.as<enter_event>());		break;
			case hashof_v<update_event	> : on_update	(*value.as<update_event>());	break;
			case hashof_v<draw_event	> : on_draw		(*value.as<draw_event>());		break;
			case hashof_v<gui_dock_event> : on_gui_dock	(*value.as<gui_dock_event>());	break;
			case hashof_v<gui_draw_event> : on_gui_draw	(*value.as<gui_draw_event>());	break;
			case hashof_v<exit_event	> : on_exit		(*value.as<exit_event>());		break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_enter(enter_event const & ev)
		{
			// load stuff, etc...

			// MENUS
			{
				// File Menu
				editor::get_main_menu().add_menu("File", [&, this]()
				{
					ML_ImGui_ScopeID(ML_ADDRESSOF(this));
					if (ImGui::MenuItem("Quit", "Alt+F4"))
					{
						engine::get_window().close();
					}
				});

				// View Menu
				editor::get_main_menu().add_menu("View", [&, this]()
				{
					ML_ImGui_ScopeID(ML_ADDRESSOF(this));
					ImGui::MenuItem("libmeme demo", "", &m_show_libmeme_demo);
					ImGui::MenuItem("imgui demo", "", &m_show_imgui_demo);
				});

				// Option Menu
				editor::get_main_menu().add_menu("Option", [&, this]()
				{
					ML_ImGui_ScopeID(ML_ADDRESSOF(this));
					bool fullscreen{ engine::get_window().is_fullscreen() };
					if (ImGui::MenuItem("Fullscreen", "", &fullscreen))
					{
						engine::get_window().set_fullscreen(fullscreen);
					}
				});
			}

			// PIPELINE
			{
				(m_pipeline["1"] = make_render_texture(vec2i{ 1280, 720 })).create();
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
				m_materials["2d"] = make_material(
					make_uniform<float_t>("u_time",		[]() { return engine::get_time().count<float_t>(); }),
					make_uniform<float_t>("u_delta",	[]() { return engine::get_runtime().delta_time; }),
					make_uniform<texture>("u_texture0",	&m_textures["navball"]),
					make_uniform<color	>("u_color",	colors::white),
					make_uniform<mat4	>("u_proj",		mat4::identity()),
					make_uniform<mat4	>("u_view",		mat4::identity()),
					make_uniform<mat4	>("u_model",	mat4::identity())
				);

				m_materials["3d"] = make_material(
					make_uniform<float_t>("u_time",		[]() { return engine::get_time().count<float_t>(); }),
					make_uniform<float_t>("u_delta",	[]() { return engine::get_runtime().delta_time; }),
					make_uniform<vec3	>("u_cam.pos",	vec3{ 0, 0, 3.f }),
					make_uniform<vec3	>("u_cam.dir",	vec3{ 0, 0, -1.f }),
					make_uniform<float_t>("u_cam.fov",	45.0f),
					make_uniform<float_t>("u_cam.near", 0.0001f),
					make_uniform<float_t>("u_cam.far",	1000.0f),
					make_uniform<vec2	>("u_cam.view", vec2{ 1280.f, 720.f }),
					make_uniform<texture>("u_texture0",	&m_textures["navball"]),
					make_uniform<color	>("u_color",	colors::white),
					make_uniform<vec3	>("u_position", vec3{}),
					make_uniform<vec3	>("u_scale",	vec3{}),
					make_uniform<vec4	>("u_rotation", vec4{})
				);
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

				if (auto e{ m_ecs.create_handle() })
				{
					e.add_tag<t_renderer>();
					e.add_component<c_shader>	(m_shaders	["3d"]);
					e.add_component<c_material>	(m_materials["3d"]);
					e.add_component<c_model>	(m_models	["sphere32x24"]);
					e.add_component<c_transform>(
						vec3{ 0.f, 0.f, 0.f },
						vec4{ 0.0f, 0.1f, 0.0f, 0.25f },
						vec3{ 1.f, 1.f, 1.f }
					);
				}
			}
		}

		void on_update(update_event const & ev)
		{
			// update stuff, etc...

			m_ecs.update_system<x_apply_transform>();
			m_ecs.update_system<x_apply_uniforms>();
		}

		void on_draw(draw_event const & ev)
		{
			// draw stuff, etc...

			if (m_pipeline.empty()) return;

			if (render_texture const & target{ m_pipeline["1"] })
			{
				target.bind();
				target.clear_color(colors::magenta);
				target.viewport(target.bounds());
				constexpr render_states states{
					{}, {}, cull_state{ false }, {}
				}; states();
				m_ecs.update_system<x_draw_renderer>(target);
				target.unbind();
			}
		}

		void on_gui_dock(gui_dock_event const & ev)
		{
			// dock gui windows
			ev.d.dock_window("libmeme demo", ev.d.get_node(ev.d.Root));
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

			// libmeme demo
			if (m_show_libmeme_demo)
			{
				ImGui::SetNextWindowSize({ 640, 640 }, ImGuiCond_Once);
				if (ImGui::Begin("libmeme demo", &m_show_libmeme_demo, ImGuiWindowFlags_None))
				{
					// Memory
					ImGui::Text("manual allocations: %u", memory_tracker::get_records().size());
					ImGui::Separator();

					ImGui::Columns(2);

					// Total Time
					ImGui::Text("total time"); ImGui::NextColumn();
					ImGui::Text("%.3fs", engine::get_time().count()); ImGui::NextColumn();

					// Delta Time
					ImGui::Text("delta time"); ImGui::NextColumn();
					ImGui::Text("%.7fs", engine::get_runtime().delta_time); ImGui::NextColumn();

					// Frame Rate
					ImGui::Text("frame rate"); ImGui::NextColumn();
					ImGui::Text("%.4ffps", ImGui::GetIO().Framerate); ImGui::NextColumn();

					// Benchmarks
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

					editor::draw_texture_preview(m_pipeline["1"].get_texture(),
						(vec2)engine::get_window().get_frame_size() / 2.f
					);
					ImGui::Separator();
				}
				ImGui::End();
			}
		}

		void on_exit(exit_event const & ev)
		{
			// cleanup stuff, etc...

			m_images.clear();
			m_shaders.clear();
			m_materials.clear();
			m_models.clear();
			m_textures.clear();
			m_fonts.clear();
			m_pipeline.clear();
			m_scripts.clear();
			m_ecs.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

extern "C" ML_PLUGIN_API ml::plugin * ml_plugin_main()
{
	static ml::plugin * temp{};
	if (!temp) { temp = new ml::demo{}; }
	return temp;
}