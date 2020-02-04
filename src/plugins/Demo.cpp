#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/ECS.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Renderer/RenderStates.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>
#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Renderer/Model.hpp>
#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/RenderTexture.hpp>
#include <libmeme/Renderer/Font.hpp>

namespace ml
{
	struct demo final : plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool m_show_imgui_demo{ false };

		pmr::vector<render_texture>			m_pipeline	{};
		ds::flat_map<pmr::string, font>		m_fonts		{};
		ds::flat_map<pmr::string, image>	m_images	{};
		ds::flat_map<pmr::string, material>	m_materials	{};
		ds::flat_map<pmr::string, model>	m_models	{};
		ds::flat_map<pmr::string, script>	m_scripts	{};
		ds::flat_map<pmr::string, shader>	m_shaders	{};
		ds::flat_map<pmr::string, texture>	m_textures	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// components
		struct c_position	{ vec3		value; };
		struct c_rotation	{ vec4		value; };
		struct c_scale		{ vec3		value; };
		struct c_shader		{ shader	value; };
		struct c_material	{ material	value; };
		struct c_model		{ model		value; };

		// tags
		struct t_renderer {};

		// signatures
		using s_apply_position	= meta::list<c_material, c_position>;
		using s_apply_rotation	= meta::list<c_material, c_rotation>;
		using s_apply_scale		= meta::list<c_material, c_scale>;
		using s_renderer		= meta::list<t_renderer, c_shader, c_material, c_model>;

		// entity settings
		using entity_settings = ecs::settings<
			// COMPONENTS
			ecs::component_config
			<
			c_position, c_rotation, c_scale,
			c_shader, c_material, c_model
			>,
			// TAGS
			ecs::tag_config
			<
			t_renderer
			>,
			// SIGNATURES
			ecs::signature_config
			<
			s_apply_position, s_apply_rotation, s_apply_scale,
			s_renderer
			>
		>;

		// entity manager
		using entity_manager = ecs::manager<entity_settings>;

		entity_manager m_entities;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		demo()
		{
			event_system::add_listener<enter_event>(this);
			event_system::add_listener<update_event>(this);
			event_system::add_listener<draw_event>(this);
			event_system::add_listener<dockspace_event>(this);
			event_system::add_listener<gui_event>(this);
			event_system::add_listener<exit_event>(this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_event(event const & value) override
		{
			switch (value.id())
			{
			case hashof_v<enter_event>:
			{
				// load stuff, etc...

				// SETUP MENUS
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
						ImGui::MenuItem("ImGui demo", "", &m_show_imgui_demo);
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

				// SETUP PIPELINE
				{
					m_pipeline.emplace_back(make_render_texture(vec2i{ 1280, 720 })).create();
				}

				// LOAD IMAGES
				{
					if (auto const & img{ m_images["icon"] = make_image(
						fs::path{ "../../../assets/textures/icon.png" }
					) }; !img.empty())
					{
						engine::get_window().set_icon(img.width(), img.height(), img.data());
					}
				}

				// LOAD SHADERS
				{
					m_textures["doot"] = make_texture(
						fs::path{ "../../../assets/textures/doot.png" }
					);

					m_textures["navball"] = make_texture(
						fs::path{ "../../../assets/textures/navball.png" }
					);
				}

				// LOAD FONTS
				{
					m_fonts["consolas"] = make_font(
						fs::path{ "../../../assets/fonts/consolas.ttf" }
					);
				}

				// LOAD SHADERS
				{
					m_shaders["2d"] = make_shader(
						fs::path{ "../../../assets/shaders/2D.vs.shader" },
						fs::path{ "../../../assets/shaders/basic.fs.shader" }
					);

					m_shaders["3d"] = make_shader(
						fs::path{ "../../../assets/shaders/3D.vs.shader" },
						fs::path{ "../../../assets/shaders/basic.fs.shader" }
					);
				}

				// LOAD MATERIALS
				{
					m_materials["2d"] = make_material(
						make_uniform<float_t>("u_time",		[]() { return (float_t)engine::get_time().count(); }),
						make_uniform<float_t>("u_delta",	[]() { return (float_t)engine::get_io().delta_time; }),
						make_uniform<texture>("u_texture0",	&m_textures["navball"]),
						make_uniform<color	>("u_color",	colors::white),
						make_uniform<mat4	>("u_proj",		mat4::identity()),
						make_uniform<mat4	>("u_view",		mat4::identity()),
						make_uniform<mat4	>("u_model",	mat4::identity())
					);

					m_materials["3d"] = make_material(
						make_uniform<float_t>("u_time",		[]() { return (float_t)engine::get_time().count(); }),
						make_uniform<float_t>("u_delta",	[]() { return (float_t)engine::get_io().delta_time; }),
						make_uniform<vec3	>("u_cam.pos",	vec3{ 0, 0, 3.f }),
						make_uniform<vec3	>("u_cam.dir",	vec3{ 0, 0, -1.f }),
						make_uniform<float_t>("u_cam.fov",	45.0f),
						make_uniform<float_t>("u_cam.near",	0.0001f),
						make_uniform<float_t>("u_cam.far",	1000.0f),
						make_uniform<vec2	>("u_cam.view",	vec2{ 1280.f, 720.f }),
						make_uniform<texture>("u_texture0",	&m_textures["navball"]),
						make_uniform<color	>("u_color",	colors::white),
						make_uniform<vec3	>("u_position", vec3{}),
						make_uniform<vec3	>("u_scale",	vec3{}),
						make_uniform<vec4	>("u_rotation", vec4{})
					);
				}

				// LOAD MODELS
				{
					m_models["sphere8x6"] = make_model(
						fs::path{ "../../../assets/models/sphere8x6.obj" }
					);

					m_models["sphere32x24"] = make_model(
						fs::path{ "../../../assets/models/sphere32x24.obj" }
					);

					// FIXME: upside down?
					m_models["monkey"] = make_model(
						fs::path{ "../../../assets/models/monkey.obj" }
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

				// LOAD ENTITIES
				{
					if (auto h{ m_entities.create_handle() })
					{
						h.add_tag<t_renderer>();
						h.add_component<c_position	>(vec3{ 0.f, 0.f, 0.f });
						h.add_component<c_scale		>(vec3{ 1.f, 1.f, 1.f });
						h.add_component<c_rotation	>(vec4{ 0.0f, 0.1f, 0.0f, 0.25f });
						h.add_component<c_shader	>(m_shaders["3d"]);
						h.add_component<c_material	>(m_materials["3d"]);
						h.add_component<c_model		>(m_models["sphere32x24"]);
					}
					m_entities.refresh();
				}

			} break;
			case hashof_v<update_event>:
			{
				// update stuff, etc...

				m_entities.for_matching<s_apply_position
				>([&](auto, c_material & mt, c_position & p)
				{
					if (auto u{ mt.value.find("u_position") }
					; u && u->holds<vec3>()) u->set<vec3>(p.value);
				});

				m_entities.for_matching<s_apply_rotation
				>([&](auto, c_material & mt, c_rotation & r)
				{
					if (auto u{ mt.value.find("u_rotation") }
					; u && u->holds<vec4>()) u->set<vec4>(r.value);
				});

				m_entities.for_matching<s_apply_scale
				>([&](auto, c_material & mt, c_scale & s)
				{
					if (auto u{ mt.value.find("u_scale") }
					; u && u->holds<vec3>()) u->set<vec3>(s.value);
				});
				
			} break;
			case hashof_v<draw_event>:
			{
				// draw stuff, etc...

				if (m_pipeline.empty()) return;

				if (render_texture const & target{ m_pipeline.at(0) })
				{
					ML_BIND_SCOPE(target);
					target.clear_color(colors::magenta);
					target.viewport(target.bounds());
					cull_state{ false }();
					
					m_entities.for_matching<s_renderer
					>([&](auto, c_shader & sh, c_material & mt, c_model & md)
					{
						sh.value.bind(false);
						
						for (uniform const & u : mt.value)
							sh.value.set_uniform(u);
						
						sh.value.bind(true);
						
						target.draw(md.value);
						
						sh.value.unbind();
					});
				}

			} break;
			case hashof_v<dockspace_event>:
			{
				auto ev{ value.as<dockspace_event>() };
				ev->d.dock_window("libmeme demo", ev->d.get_node(ev->d.Root));
			} break;
			case hashof_v<gui_event>:
			{
				ML_ImGui_ScopeID(ML_ADDRESSOF(this));

				// ImGui demo
				if (m_show_imgui_demo)
				{
					editor::show_imgui_demo(&m_show_imgui_demo);
				}

				ImGui::SetNextWindowSize({ 640, 640 }, ImGuiCond_Once);
				if (ImGui::Begin("libmeme demo", nullptr, ImGuiWindowFlags_None))
				{
					// Memory
					ImGui::Text("Manual Allocations: %u", ML_Memory.get_records().size());

					ImGui::Separator();
					ImGui::Columns(2);

					// Total Time
					ImGui::Text("total time"); ImGui::NextColumn();
					ImGui::Text("%.3fs", engine::get_time().count()); ImGui::NextColumn();

					// Delta Time
					ImGui::Text("delta time"); ImGui::NextColumn();
					ImGui::Text("%.7fs", engine::get_io().delta_time); ImGui::NextColumn();

					// Frame Rate
					ImGui::Text("frame rate"); ImGui::NextColumn();
					ImGui::Text("%.4ffps", ImGui::GetIO().Framerate); ImGui::NextColumn();

					// Benchmarks
					if (auto const & prev{ performance_tracker::previous() }; !prev.empty())
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

					editor::draw_texture_preview(m_pipeline[0].get_texture(),
						(vec2)engine::get_window().get_frame_size() / 2.f
					);
					ImGui::Separator();
				}
				ImGui::End();
			} break;
			case hashof_v<exit_event>:
			{
				m_images.clear();
				m_shaders.clear();
				m_materials.clear();
				m_models.clear();
				m_textures.clear();
				m_fonts.clear();
				m_pipeline.clear();
				m_scripts.clear();
				m_entities.clear();
			} break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

extern "C" ML_PLUGIN_API ml::plugin * ml_plugin_main()
{
	static ml::plugin * temp{ nullptr };
	if (!temp) { temp = new ml::demo{}; }
	return temp;
}