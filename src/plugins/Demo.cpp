#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Engine/Script.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Platform/WindowEvents.hpp>
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
	struct Demo final : public Plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		std::vector<RenderTexture>			m_pipeline	{};
		ds::flat_map<std::string, Font>		m_fonts		{};
		ds::flat_map<std::string, Image>	m_images	{};
		ds::flat_map<std::string, Material>	m_materials	{};
		ds::flat_map<std::string, Model>	m_models	{};
		ds::flat_map<std::string, Script>	m_scripts	{};
		ds::flat_map<std::string, Shader>	m_shaders	{};
		ds::flat_map<std::string, Texture>	m_textures	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Demo() : Plugin{}
		{
			EventSystem::add_listener<LoadEvent>(this);
			EventSystem::add_listener<UpdateEvent>(this);
			EventSystem::add_listener<DrawEvent>(this);
			EventSystem::add_listener<DockspaceEvent>(this);
			EventSystem::add_listener<GuiEvent>(this);
			EventSystem::add_listener<UnloadEvent>(this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(Event const & value) override
		{
			switch (value.id())
			{
			case LoadEvent::ID: if (auto ev{ event_cast<LoadEvent>(&value) })
			{
				Editor::main_menu().add_menu("File", [&]()
				{
					ImGui::PushID(ML_ADDRESSOF(this));
					if (ImGui::MenuItem("Quit", "Alt+F4"))
					{
						Engine::window().close();
					}
					ImGui::PopID();
				});

				Editor::main_menu().add_menu("Options", [&]()
				{
					ImGui::PushID(ML_ADDRESSOF(this));
					if (ImGui::BeginMenu("Window"))
					{
						bool fullscreen{ Engine::window().is_fullscreen() };
						if (ImGui::MenuItem("Fullscreen", "", &fullscreen))
						{
							Engine::window().set_fullscreen(fullscreen);
						}
						ImGui::EndMenu();
					}
					ImGui::PopID();
				});

				m_pipeline.emplace_back(make_render_texture(vec2i{ 1280, 720 })).create();

				if (auto const & img{ m_images["icon"] = make_image(
					FS::path_to("../../../assets/textures/icon.png")
				) }; !img.empty())
				{
					Engine::window().set_icon(img.width(), img.height(), img.data());
				}

				m_textures["doot"] = make_texture(
					FS::path_to("../../../assets/textures/doot.png")
				);

				m_textures["navball"] = make_texture(
					FS::path_to("../../../assets/textures/navball.png")
				);

				m_fonts["consolas"] = make_font(
					FS::path_to("../../../assets/fonts/consolas.ttf")
				);

				m_shaders["2d"] = make_shader(
					FS::path_to("../../../assets/shaders/2D.vs.shader"),
					FS::path_to("../../../assets/shaders/basic.fs.shader")
				);

				m_shaders["3d"] = make_shader(
					FS::path_to("../../../assets/shaders/3D.vs.shader"),
					FS::path_to("../../../assets/shaders/basic.fs.shader")
				);

				m_materials["2d"] = make_material(
					make_uniform<float_t>("u_time",			[]() { return (float_t)ImGui::GetTime(); }),
					make_uniform<Texture>("u_texture0",		&m_textures["navball"]),
					make_uniform<Color	>("u_color",		colors::white),
					make_uniform<mat4	>("u_proj",			mat4::identity()),
					make_uniform<mat4	>("u_view",			mat4::identity()),
					make_uniform<mat4	>("u_model",		mat4::identity())
				);

				m_materials["3d"] = make_material(
					make_uniform<float_t>("u_time",			[]() { return (float_t)ImGui::GetTime(); }),
					make_uniform<vec3	>("u_camera.pos",	vec3{ 0, 0, 3.f }),
					make_uniform<vec3	>("u_camera.dir",	vec3{ 0, 0, -1.f }),
					make_uniform<float_t>("u_camera.fov",	45.0f),
					make_uniform<float_t>("u_camera.near",	0.0001f),
					make_uniform<float_t>("u_camera.far",	1000.0f),
					make_uniform<vec2	>("u_camera.view",	vec2{ 1280.f, 720.f }),
					make_uniform<Texture>("u_texture0",		&m_textures["navball"]),
					make_uniform<Color	>("u_color",		colors::white),
					make_uniform<vec3	>("u_position",		vec3{ 0.f, 0.f, 0.f }),
					make_uniform<vec3	>("u_scale",		vec3{ 1.f, 1.f, 1.f }),
					make_uniform<vec4	>("u_rotation",		vec4{ 0.0f, 0.1f, 0.0f, 0.25f })
				);

				m_models["sphere32x24"] = make_model(
					FS::path_to("../../../assets/meshes/sphere32x24.obj")
				);

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

			} break;
			case UpdateEvent::ID: if (auto ev{ event_cast<UpdateEvent>(&value) })
			{
				// update stuff
			} break;
			case DrawEvent::ID: if (auto ev{ event_cast<DrawEvent>(&value) })
			{
				GL::clearColor(0, 0, 0, 1);
				GL::clear(GL::DepthBufferBit | GL::ColorBufferBit);
				GL::viewport(0, 0, Engine::window().get_frame_width(), Engine::window().get_frame_height());
				RenderStates{}();

				if (ML_BIND_EX(RenderTexture, _r, m_pipeline[0]))
				{
					constexpr auto bg{ colors::magenta };
					GL::clearColor(bg[0], bg[1], bg[2], bg[3]);
					GL::clear(GL::DepthBufferBit | GL::ColorBufferBit);
					GL::viewport(0, 0, _r->size()[0], _r->size()[1]);
					CullState{ false }();

					if (ML_BIND_EX(Shader, _s, m_shaders["3d"], false))
					{
						for (auto const & u : m_materials["3d"])
						{
							_s->set_uniform(u);
						}
						_s->bind(true);
						_r->draw(m_models["sphere32x24"]);
					}
				}

			} break;
			case DockspaceEvent::ID: if (auto ev{ event_cast<DockspaceEvent>(&value) })
			{
				auto & d{ ev->dockspace };
				d.dock_window("libmeme demo", d.get_node(d.Root));
			} break;
			case GuiEvent::ID: if (auto ev{ event_cast<GuiEvent>(&value) })
			{
				ImGui::PushID(ML_ADDRESSOF(this));
				ImGui::SetNextWindowSize({ 640, 640 }, ImGuiCond_Once);
				if (ImGui::Begin("libmeme demo", nullptr, ImGuiWindowFlags_None))
				{
					ImGui::Text("Active Allocations: %u", ML_MemoryTracker.records().size());

					ImGui::Separator();
					ImGui::Columns(2);

					// Total Time
					ImGui::Text("total time"); ImGui::NextColumn();
					ImGui::Text("%.3fs", ImGui::GetTime()); ImGui::NextColumn();

					// Delta Time
					ImGui::Text("delta time"); ImGui::NextColumn();
					ImGui::Text("%.7fs", ImGui::GetIO().DeltaTime); ImGui::NextColumn();

					// Frame Rate
					ImGui::Text("frame rate"); ImGui::NextColumn();
					ImGui::Text("%.4ffps", ImGui::GetIO().Framerate); ImGui::NextColumn();

					// Benchmarks
					if (auto const & prev{ PerformanceTracker::previous() }; !prev.empty())
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

					draw_texture_preview(m_pipeline[0].texture()); ImGui::Separator();

					//draw_texture_preview(m_textures["doot"]); ImGui::Separator();
				}
				ImGui::End();
				ImGui::PopID();
			} break;
			case UnloadEvent::ID: if (auto ev{ event_cast<UnloadEvent>(&value) })
			{
				m_images.clear();
				m_shaders.clear();
				m_materials.clear();
				m_models.clear();
				m_textures.clear();
				m_fonts.clear();
				m_pipeline.clear();
				m_scripts.clear();
			} break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void draw_texture_preview(Texture const & value, vec2 const & maxSize = { 512, 512 })
		{
			auto & io{ ImGui::GetIO() };
			void * const tex_id{ value.address() };
			vec2 const tex_sz{ alg::scale_to_fit((vec2)value.size(), maxSize) };
			float_t const tex_w{ tex_sz[0] };
			float_t const tex_h{ tex_sz[1] };
			ImGui::Text("ID: %u", value.handle());
			ImGui::Text("%.0fx%.0f (%ux%u)", tex_w, tex_h, value.width(), value.height());
			auto const pos{ ImGui::GetCursorScreenPos() };
			ImGui::Image(tex_id,
				{ tex_w, tex_h },
				{ 0, 0 },
				{ 1, 1 },
				{ 1.0f, 1.0f, 1.0f, 1.0f },
				{ 1.0f, 1.0f, 1.0f, 0.5f }
			);
			if (ImGui::IsItemHovered())
			{
				float_t const region_zoom{ 4.0f };
				float_t const region_size{ 32.0f };

				ImGui::BeginTooltip();

				float_t region_x{ io.MousePos.x - pos.x - region_size * 0.5f };
				if (region_x < 0.0f) region_x = 0.0f;
				else if (region_x > (tex_w - region_size)) region_x = (tex_w - region_size);

				float_t region_y{ io.MousePos.y - pos.y - region_size * 0.5f };
				if (region_y < 0.0f) region_y = 0.0f;
				else if (region_y > (tex_h - region_size)) region_y = (tex_h - region_size);

				ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
				ImGui::Text("Max: (%.2f, %.2f)", region_x + region_size, region_y + region_size);
				ImGui::Image(tex_id,
					{ region_size * region_zoom, region_size * region_zoom },
					{ region_x / tex_w, region_y / tex_h },
					{ (region_x + region_size) / tex_w, (region_y + region_size) / tex_h },
					{ 1.0f, 1.0f, 1.0f, 1.0f },
					{ 1.0f, 1.0f, 1.0f, 0.5f }
				);

				ImGui::EndTooltip();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

extern "C" ML_PLUGIN_API ml::Plugin * ML_Plugin_Main()
{
	return new ml::Demo{};
}