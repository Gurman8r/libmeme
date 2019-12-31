#include <libmeme/Editor/Dockspace.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Editor/Editor.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Dockspace::Dockspace()
		: m_good		{ false }
		, m_open		{ false }
		, m_nodes		{}
		, m_border		{ 0.0f }
		, m_padding		{ 0.f, 0.f }
		, m_rounding	{ 0.0f }
		, m_size		{ 0.f, 0.f }
		, m_transparency{ 1.0f }
	{
		m_nodes.push_back(0);
		m_nodes.push_back(1);
		m_nodes.push_back(2);
		m_nodes.push_back(3);
		m_nodes.push_back(4);
		m_nodes.push_back(5);
		m_nodes.push_back(6);
	}

	Dockspace::~Dockspace() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Dockspace::render()
	{
		if (([&, this]()
		{
			// Begin Draw
			if (m_good = (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable))
			{
				// Bounds
				ImGuiViewport const * viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);

				// Style
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, m_rounding);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, m_border);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { m_padding[0], m_padding[1] });
				ImGui::SetNextWindowBgAlpha(m_transparency);

				// Begin
				ImGui::PushID(ML_ADDRESSOF(this));
				ImGui::PushID(dockspace_title);
				if (m_good = ImGui::Begin(dockspace_title, &m_open,
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoBringToFrontOnFocus |
					ImGuiWindowFlags_NoNavFocus |
					ImGuiWindowFlags_NoDocking |
					ImGuiWindowFlags_NoBackground |
					(ML_Editor.mainMenuBar().open() ? ImGuiWindowFlags_MenuBar : 0)
				))
				{
					ImGui::PopStyleVar(3);
				}
			}
			return m_good;
		})())
		{
			// Draw
			if (m_nodes[Root] = begin_builder(ImGuiDockNodeFlags_AutoHideTabBar))
			{
				m_nodes[Left] = split_node(m_nodes[Root], ImGuiDir_Left, 0.4f, &m_nodes[Root]);
				m_nodes[Right] = split_node(m_nodes[Root], ImGuiDir_Right, 0.6f, &m_nodes[Root]);
				m_nodes[LeftUp] = split_node(m_nodes[Left], ImGuiDir_Up, 0.75f, &m_nodes[Left]);
				m_nodes[RightUp] = split_node(m_nodes[Right], ImGuiDir_Up, 0.75f, &m_nodes[Right]);
				m_nodes[LeftDn] = split_node(m_nodes[Left], ImGuiDir_Down, 0.25f, &m_nodes[Left]);
				m_nodes[RightDn] = split_node(m_nodes[Right], ImGuiDir_Down, 0.25f, &m_nodes[Right]);

				//ML_EventSystem.fireEvent<DockspaceEvent>(*this);

				end_builder(m_nodes[Root]);
			};
		}
		return ([&, this]()
		{
			// End Draw
			if (m_good)
			{
				ImGui::DockSpace(
					ImGui::GetID(dockspace_title),
					{ m_size[0], m_size[1] },
					ImGuiDockNodeFlags_PassthruCentralNode |
					ImGuiDockNodeFlags_AutoHideTabBar
				);
			}
			ImGui::End();
			ImGui::PopID();
			ImGui::PopID();
			return m_good;
		})();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t Dockspace::begin_builder(int32_t flags)
	{
		if (uint32_t root{ ImGui::GetID(dockspace_title) })
		{
			if (!ImGui::DockBuilderGetNode(root))
			{
				ImGui::DockBuilderRemoveNode(root);
				
				ImGui::DockBuilderAddNode(root, flags);
				
				return root;
			}
		}
		return NULL;
	}

	uint32_t Dockspace::end_builder(uint32_t root)
	{
		if (root)
		{
			ImGui::DockBuilderFinish(root);
		}
		return root;
	}

	uint32_t Dockspace::dock_window(C_String name, uint32_t id)
	{
		if (name && id)
		{
			ImGui::DockBuilderDockWindow(name, id);
			return id;
		}
		return NULL;
	}

	uint32_t Dockspace::split_node(uint32_t id, int32_t dir, float_t ratio, uint32_t * other)
	{
		return split_node(id, dir, ratio, nullptr, other);
	}

	uint32_t Dockspace::split_node(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * other)
	{
		return ImGui::DockBuilderSplitNode(id, dir, ratio, out, other);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}