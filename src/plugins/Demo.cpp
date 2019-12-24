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

		std::vector<Image>		img{};
		std::vector<Texture>	tex{};
		std::vector<Shader>		shd{};
		std::vector<Material>	mtl{};

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
			switch (*value)
			{
			case LoadEvent::ID: if (auto ev{ value.as<LoadEvent>() })
			{
				img.emplace_back(make_image(FS::path_to("../../../assets/textures/doot.png")));
				
				tex.emplace_back(make_texture(img[0]));

				shd.emplace_back(make_shader(
					FS::path_to("../../../assets/shaders/2D.vs.shader"),
					FS::path_to("../../../assets/shaders/basic.fs.shader")
				));

				mtl.emplace_back(make_material(
					make_uniform<bool>("bool", true),
					make_uniform<int>("int", 123),
					make_uniform<float>("float", 4.56f),
					make_uniform<vec2>("vec2", vec2{ 1, 2 }),
					make_uniform<vec3>("vec3", vec3{ 3, 4, 5 }),
					make_uniform<vec4>("vec4", vec4{ 6, 7, 8, 9 }),
					make_uniform<Color>("color", []() { return colors::magenta; }),
					make_uniform<mat2>("mat2", []() { return mat2::identity(); }),
					make_uniform<mat3>("mat3", []() { return mat3::identity(); }),
					make_uniform<mat4>("mat4", []() { return mat4::identity(); }),
					make_uniform<Texture const *>("tex0", &tex[0])
				));

			} break;
			case DrawEvent::ID: if (auto ev{ value.as<DrawEvent>() })
			{
				GL::clear(GL::DepthBufferBit | GL::ColorBufferBit);
				GL::clearColor(
					colors::magenta[0],
					colors::magenta[1],
					colors::magenta[2],
					colors::magenta[3]
				);
			} break;
			case GuiEvent::ID: if (auto ev{ value.as<GuiEvent>() })
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

					auto & io{ ImGui::GetIO() };
					ImTextureID my_tex_id = tex[0].address();
					float my_tex_w = (float)tex[0].width();
					float my_tex_h = (float)tex[0].height();
					ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
					ImVec2 pos = ImGui::GetCursorScreenPos();
					ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						float region_sz = 32.0f;
						float region_x = io.MousePos.x - pos.x - region_sz * 0.5f; if (region_x < 0.0f) region_x = 0.0f; else if (region_x > my_tex_w - region_sz) region_x = my_tex_w - region_sz;
						float region_y = io.MousePos.y - pos.y - region_sz * 0.5f; if (region_y < 0.0f) region_y = 0.0f; else if (region_y > my_tex_h - region_sz) region_y = my_tex_h - region_sz;
						float zoom = 4.0f;
						ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
						ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
						ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
						ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
						ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
						ImGui::EndTooltip();
					}

				}
				ImGui::End();
				ImGui::PopID();
			} break;
			case UnloadEvent::ID: if (auto ev{ value.as<UnloadEvent>() })
			{
				img.clear();
				tex.clear();
				shd.clear();
				mtl.clear();
			} break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

extern "C" ML_PLUGIN_API ml::Plugin * ML_Plugin_Main() { return new ml::Demo{}; }