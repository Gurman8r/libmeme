#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
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
#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct Demo final : public Plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		dense_map<std::string, Image>		m_images	{};
		dense_map<std::string, Texture>		m_textures	{};
		dense_map<std::string, Shader>		m_shaders	{};
		dense_map<std::string, Material>	m_materials	{};
		dense_map<std::string, Model>		m_models	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Demo() : Plugin{}
		{
			ML_EventSystem.addListener<LoadEvent>(this);
			ML_EventSystem.addListener<DrawEvent>(this);
			ML_EventSystem.addListener<GuiEvent>(this);
			ML_EventSystem.addListener<UnloadEvent>(this);
		}

		~Demo()
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(Event const & value) override
		{
			switch (value.id())
			{
			case LoadEvent::ID: if (auto ev{ event_cast<LoadEvent>(value) })
			{
				m_images.insert({ "icon", make_image(
					FS::path_to("../../../assets/textures/icon.png")
				) });

				if (auto const & img{ m_images["icon"] }; !img.empty())
				{
					ev->window.setIcon(img.width(), img.height(), img.data());
				}
				
				m_textures.insert({ "navball", make_texture(
					make_image(FS::path_to("../../../assets/textures/navball.png"))
				) });

				m_shaders.insert({ "2d", make_shader(
					FS::path_to("../../../assets/shaders/2D.vs.shader"),
					FS::path_to("../../../assets/shaders/basic.fs.shader")
				) });

				m_shaders.insert({ "3d", make_shader(
					FS::path_to("../../../assets/shaders/3D.vs.shader"),
					FS::path_to("../../../assets/shaders/basic.fs.shader")
				) });

				m_materials.insert({ "2d", make_material(
					make_uniform<float_t	>("u_time",			[]() { return (float_t)ImGui::GetTime(); }),
					make_uniform<Texture>("u_texture0",		&m_textures["navball"]),
					make_uniform<Color	>("u_color",		colors::white),
					make_uniform<mat4	>("u_proj",			mat4::identity()),
					make_uniform<mat4	>("u_view",			mat4::identity()),
					make_uniform<mat4	>("u_model",		mat4::identity())
				) });

				m_materials.insert({ "3d", make_material({
					make_uniform<float_t>("u_time",			[]() { return (float_t)ImGui::GetTime(); }),
					make_uniform<vec3	>("u_camera.pos",	vec3{ 0, 0, 3.f }),
					make_uniform<vec3	>("u_camera.dir",	vec3{ 0, 0, -1.f }),
					make_uniform<float_t>("u_camera.fov",	45.0f),
					make_uniform<float_t>("u_camera.near",	0.0001f),
					make_uniform<float_t>("u_camera.far",	1000.0f),
					make_uniform<vec2	>("u_camera.view",	vec2{ 1280.f, 720.f }),
					make_uniform<Texture>("u_texture0",		&m_textures["navball"]),
					make_uniform<Color	>("u_color",		colors::white),
					make_uniform<vec3	>("u_position",		vec3{ 0.f, 0.f, -5.f }),
					make_uniform<vec3	>("u_scale",		vec3{ 0.5f, 0.5f, 0.5f }),
					make_uniform<vec4	>("u_rotation",		vec4{ 0.0f, 0.1f, 0.0f, 0.25f })
				}) });

				m_models.insert({ "sphere32x24", make_model(
					FS::path_to("../../../assets/meshes/sphere32x24.obj")
				) });

				m_models.insert({ "tri", make_model({ make_mesh(
					{
						make_vertex({  0.0f,  0.5f, 0.0f }, vec3::one(), { 0.5f, 1.0f }),
						make_vertex({  0.5f, -0.5f, 0.0f }, vec3::one(), { 1.0f, 0.0f }),
						make_vertex({ -0.5f, -0.5f, 0.0f }, vec3::one(), { 0.0f, 0.0f }),
					},
					{
						0, 1, 2,
					}
				) }) });

			} break;
			case DrawEvent::ID: if (auto ev{ event_cast<DrawEvent>(value) })
			{
				static constexpr auto bg{ make_color(colors::magenta) };
				GL::clear(GL::DepthBufferBit | GL::ColorBufferBit);
				GL::clearColor(bg[0], bg[1], bg[2], bg[3]);

				RenderStates{}();

				if (ML_BIND_EX(Shader, _s, m_shaders["3d"], false))
				{
					for (auto const & u : m_materials["3d"])
					{
						_s->set_uniform(u);
					}
					_s->bind(true);
					ev->window.draw(m_models["sphere32x24"]);
				}

			} break;
			case GuiEvent::ID: if (auto ev{ event_cast<GuiEvent>(value) })
			{
				ImGui::PushID(ML_ADDRESSOF(this));
				ImGui::SetNextWindowSize({ 512, 512 }, ImGuiCond_Once);
				if (ImGui::Begin("libmeme demo", nullptr, ImGuiWindowFlags_None))
				{
					if (auto const & prev{ ML_PerformanceTracker.previous() }; !prev.empty())
					{
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
						ImGui::Separator();
						for (auto const & elem : prev)
						{
							ImGui::Text("%s", elem.first); ImGui::NextColumn();
							ImGui::Text("%.7fs", elem.second.count()); ImGui::NextColumn();
						}

						ImGui::Separator();
						ImGui::Columns(1);
					}

					auto draw_texture_preview = [](auto & value)
					{
						auto & io{ ImGui::GetIO() };
						void * const tex_id{ value.address() };
						float_t const tex_w{ (float_t)value.width() };
						float_t const tex_h{ (float_t)value.height() };
						ImGui::Text("%.0fx%.0f", tex_w, tex_h);
						auto const pos{ ImGui::GetCursorScreenPos() };
						ImGui::Image(
							tex_id,
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
							ImGui::Image(
								tex_id, 
								{ region_size * region_zoom, region_size * region_zoom }, 
								{ region_x / tex_w, region_y / tex_h },
								{ (region_x + region_size) / tex_w, (region_y + region_size) / tex_h },
								{ 1.0f, 1.0f, 1.0f, 1.0f },
								{ 1.0f, 1.0f, 1.0f, 0.5f }
							);

							ImGui::EndTooltip();
						}
					};

					draw_texture_preview(m_textures["navball"]);
				}
				ImGui::End();
				ImGui::PopID();
			} break;
			case UnloadEvent::ID: if (auto ev{ event_cast<UnloadEvent>(value) })
			{
				m_images.clear();
				m_textures.clear();
				m_shaders.clear();
				m_materials.clear();
				m_models.clear();
			} break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

extern "C" ML_PLUGIN_API ml::Plugin * ML_Plugin_Main()
{
	return new ml::Demo{};
}