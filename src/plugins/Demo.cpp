#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Platform/WindowEvents.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/Color.hpp>

namespace ml
{
	struct Demo : public Plugin
	{
		Demo() : Plugin{}
		{
			ML_EventSystem.addListener<DrawEvent>(this);
			ML_EventSystem.addListener<GuiEvent>(this);
		}

		void onEvent(Event const & value) override
		{
			switch (*value)
			{
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
				// Profiler
				ImGui::SetNextWindowSize({ 256, 256 }, ImGuiCond_Once);
				if (ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_None))
				{
					if (auto const & prev{ ML_PerformanceTracker.previous() }; !prev.empty())
					{
						ImGui::Separator();
						ImGui::Columns(2);

						// Total Time
						ImGui::Text("total time"); ImGui::NextColumn();
						ImGui::Text("%.3f", ImGui::GetTime()); ImGui::NextColumn();

						// Delta Time
						ImGui::Text("delta time"); ImGui::NextColumn();
						ImGui::Text("%.7f", ImGui::GetIO().DeltaTime); ImGui::NextColumn();

						// Frame Rate
						ImGui::Text("fps"); ImGui::NextColumn();
						ImGui::Text("%.4f", ImGui::GetIO().Framerate); ImGui::NextColumn();

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
				}
				ImGui::End();
				ImGui::PopID();
			} break;
			}
		}
	};
}

extern "C" ML_PLUGIN_API ml::Plugin * ML_Plugin_Main()
{
	return new ml::Demo{};
}