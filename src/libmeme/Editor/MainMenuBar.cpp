#include <libmeme/Editor/MainMenuBar.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MainMenuBar::MainMenuBar()
		: m_open	{ true }
		, m_good	{ false }
		, m_menus	{ decltype(m_menus)::allocator_type{} }
	{
	}

	MainMenuBar::~MainMenuBar()
	{
		m_menus.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool MainMenuBar::dispose()
	{
		m_menus.clear();
		return m_menus.empty();
	}

	bool MainMenuBar::render()
	{
		if (([&, this]()
		{
			// Begin Draw
			if (m_good = m_open)
			{
				ImGui::PushID(ML_ADDRESSOF(this));
				ImGui::PushID(mainmenubar_title);
				m_good = ImGui::BeginMainMenuBar();
			}
			return m_good;
		})())
		{
			// Draw
			for (auto & pair : m_menus)
			{
				if (!pair.second.empty() && ImGui::BeginMenu(pair.first.c_str()))
				{
					for (auto & func : pair.second)
					{
						if (func) { func(); }
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
				ImGui::PopID();
			}
			return m_good;
		})();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}