#include <libmeme/Editor/Editor_MainMenuBar.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	editor_main_menu::editor_main_menu()
		: m_open	{ true }
		, m_good	{ false }
		, m_menus	{}
	{
	}

	editor_main_menu::~editor_main_menu()
	{
		m_menus.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool editor_main_menu::render()
	{
		if (([&, this]()
		{
			// Begin Draw
			if (m_good = m_open)
			{
				ImGui::PushID(ML_ADDRESSOF(this));
				m_good = ImGui::BeginMainMenuBar();
			}
			return m_good;
		})())
		{
			// Draw
			for (auto & pair : m_menus)
			{
				if (!pair.second.empty() && ImGui::BeginMenu(pair.first))
				{
					for (auto const & fn : pair.second)
					{
						std::invoke(fn);
					}
					ImGui::EndMenu();
				}
			}
		}
		return ([&, this]()
		{
			// End Draw
			if (m_good)
			{
				ImGui::EndMainMenuBar();
				ImGui::PopID();
			}
			return m_good;
		})();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}