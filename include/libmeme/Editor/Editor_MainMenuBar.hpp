#ifndef _ML_MAIN_MENU_BAR_HPP_
#define _ML_MAIN_MENU_BAR_HPP_

#include <libmeme/Editor/Export.hpp>
#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Core/NonCopyable.hpp>

namespace ml
{
	// built-in main menu bar
	struct ML_EDITOR_API editor_main_menu final : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using menu_t = typename std::function<void()>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool dispose();

		bool render();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline editor_main_menu & add_menu(pmr::string const & name, menu_t const & value)
		{
			auto it{ std::find_if(m_menus.begin(), m_menus.end(), [&](auto elem)
			{
				return (elem.first == name);
			}) };
			if (it == m_menus.end())
			{
				m_menus.push_back({ name, {} });
				it = (m_menus.end() - 1);
			}
			if (value)
			{
				it->second.push_back(value);
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool open() const noexcept { return m_open; }

		inline auto & menus() noexcept { return m_menus; }

		inline auto const & menus() const noexcept { return m_menus; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend class editor;

		editor_main_menu();
		
		~editor_main_menu();

		bool m_good;
		bool m_open;

		pmr::vector<std::pair<
			pmr::string,
			pmr::vector<menu_t>
		>> m_menus;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_MAIN_MENU_BAR_HPP_