#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/FileSystem.hpp>
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

		dense::map<std::string, Font>		m_fonts		{};
		dense::map<std::string, Image>		m_images	{};
		dense::map<std::string, Material>	m_materials	{};
		dense::map<std::string, Model>		m_models	{};
		dense::map<std::string, Script>		m_scripts	{};
		dense::map<std::string, Shader>		m_shaders	{};
		dense::map<std::string, Texture>	m_textures	{};
		std::vector<RenderTexture>			m_pipeline	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Demo() : Plugin{}
		{
			ML_EventSystem.addListener<LoadEvent>(this);
			ML_EventSystem.addListener<UpdateEvent>(this);
			ML_EventSystem.addListener<DrawEvent>(this);
			ML_EventSystem.addListener<GuiEvent>(this);
			ML_EventSystem.addListener<UnloadEvent>(this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(Event const & value) override
		{
			switch (value.id())
			{
			case LoadEvent::ID: if (auto ev{ event_cast<LoadEvent>(value) })
			{
				ML_Editor.mainMenuBar().addMenu("Plugins", [&]()
				{
					ImGui::PushID(ML_ADDRESSOF(this));
					if (ImGui::BeginMenu("Demo"))
					{
						ImGui::Text("Sample text.");
						ImGui::EndMenu();
					}
					ImGui::PopID();
				});

				if (auto const & img{ m_images.insert({ "icon", make_image(
					FS::path_to("../../../assets/textures/icon.png")
				) }).first->second }; !img.empty())
				{
					ev->window.setIcon(img.width(), img.height(), img.data());
				}

				m_pipeline.emplace_back(make_rendertexture(
					vec2i{ 1280, 720 }
				)).create();

				m_textures.insert({ "doot", make_texture(
					FS::path_to("../../../assets/textures/doot.png")
				) });

				m_textures.insert({ "navball", make_texture(
					FS::path_to("../../../assets/textures/navball.png")
				) });

				m_fonts.insert({ "consolas", make_font(
					FS::path_to("../../../assets/fonts/consolas.ttf")
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
					make_uniform<float_t>("u_time",			[]() { return (float_t)ImGui::GetTime(); }),
					make_uniform<Texture>("u_texture0",		&m_textures["navball"]),
					make_uniform<Color	>("u_color",		colors::white),
					make_uniform<mat4	>("u_proj",			mat4::identity()),
					make_uniform<mat4	>("u_view",			mat4::identity()),
					make_uniform<mat4	>("u_model",		mat4::identity())
				) });

				m_materials.insert({ "3d", make_material(
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
				) });

				m_models.insert({ "sphere32x24", make_model(
					FS::path_to("../../../assets/meshes/sphere32x24.obj")
				) });

				m_models.insert({ "triangle", make_model(make_mesh(
					{
						make_vertex({  0.0f,  0.5f, 0.0f }, vec3::one(), { 0.5f, 1.0f }),
						make_vertex({  0.5f, -0.5f, 0.0f }, vec3::one(), { 1.0f, 0.0f }),
						make_vertex({ -0.5f, -0.5f, 0.0f }, vec3::one(), { 0.0f, 0.0f }),
					},
					{
						0, 1, 2,
					}
				)) });

				m_models.insert({ "quad", make_model(make_mesh(
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
				)) });

			} break;
			case UpdateEvent::ID: if (auto ev{ event_cast<UpdateEvent>(value) })
			{
				// update stuff
			} break;
			case DrawEvent::ID: if (auto ev{ event_cast<DrawEvent>(value) })
			{
				GL::clearColor(0, 0, 0, 1);
				GL::clear(GL::DepthBufferBit | GL::ColorBufferBit);
				GL::viewport(0, 0, ev->window.getFrameWidth(), ev->window.getFrameHeight());
				RenderStates{}();

				if (ML_BIND_EX(RenderTexture, _r, m_pipeline[0]))
				{
					constexpr auto bg{ colors::magenta };
					GL::clearColor(bg[0], bg[1], bg[2], bg[3]);
					GL::clear(GL::DepthBufferBit | GL::ColorBufferBit);
					GL::viewport(0, 0, _r->size()[0], _r->size()[1]);
					RenderStates{
						{}, {}, CullState{ false }, {}
					}();

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
			case GuiEvent::ID: if (auto ev{ event_cast<GuiEvent>(value) })
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
					if (auto const & prev{ ML_PerformanceTracker.previous() }; !prev.empty())
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
			case UnloadEvent::ID: if (auto ev{ event_cast<UnloadEvent>(value) })
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

		static void draw_texture_preview(Texture const & value, vec2 maxSize = { 512, 512 })
		{
			auto & io{ ImGui::GetIO() };
			void * const tex_id{ value.address() };
			vec2 const tex_sz{ alg::scale_to_fit((vec2)value.size(), maxSize) };
			float_t const tex_w{ tex_sz[0] };
			float_t const tex_h{ tex_sz[1] };
			ImGui::Text("ID: %u", value.handle());
			ImGui::Text("%.0fx%.0f (%ux%u)", tex_w, tex_h, value.width(), value.height());
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
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

extern "C" ML_PLUGIN_API ml::Plugin * ML_Plugin_Main()
{
	return new ml::Demo{};
}