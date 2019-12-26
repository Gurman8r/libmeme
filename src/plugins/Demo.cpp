#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Platform/WindowEvents.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct Demo final : public Plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		std::vector<Image>		m_images	{};
		std::vector<Texture>	m_textures	{};
		std::vector<Shader>		m_shaders	{};
		std::vector<Material>	m_materials	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Demo() : Plugin{}
		{
			ML_EventSystem.addListener<LoadEvent>(this);
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


				m_images.emplace_back(make_image(
					FS::path_to("../../../assets/textures/doot.png")
				));
				
				m_textures.emplace_back(make_texture(
					m_images[0]
				));

				m_shaders.emplace_back(make_shader(
					FS::path_to("../../../assets/shaders/2D.vs.shader"),
					FS::path_to("../../../assets/shaders/basic.fs.shader")
				));

				m_materials.emplace_back(make_material(
					make_uniform<bool>("bool", true),
					make_uniform<int32_t>("int", 123),
					make_uniform<float_t>("float_t", 4.56f),
					make_uniform<vec2>("vec2", vec2{ 1, 2 }),
					make_uniform<vec3>("vec3", vec3{ 3, 4, 5 }),
					make_uniform<vec4>("vec4", vec4{ 6, 7, 8, 9 }),
					make_uniform<Color>("color", []() { return colors::magenta; }),
					make_uniform<mat2>("mat2", []() { return mat2::identity(); }),
					make_uniform<mat3>("mat3", []() { return mat3::identity(); }),
					make_uniform<mat4>("mat4", []() { return mat4::identity(); }),
					make_uniform<Texture const *>("tex0", &m_textures[0])
				));

			} break;
			case DrawEvent::ID: if (auto ev{ event_cast<DrawEvent>(value) })
			{
				GL::clear(GL::DepthBufferBit | GL::ColorBufferBit);
				GL::clearColor(
					colors::magenta[0],
					colors::magenta[1],
					colors::magenta[2],
					colors::magenta[3]
				);
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

					draw_texture_preview(m_textures[0]);
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