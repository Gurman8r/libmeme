#ifndef _ML_MAIN_MENU_BAR_HPP_
#define _ML_MAIN_MENU_BAR_HPP_

#include <libmeme/Editor/Export.hpp>
#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Core/NonCopyable.hpp>

namespace ml
{
	struct ML_EDITOR_API MainMenuBar final : public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using menu_t = typename std::function<void()>;

		static constexpr auto mainmenubar_title{ "libmeme##editor##mainmenubar" };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool render();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline decltype(auto) addMenu(std::string const & name, menu_t const & func)
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
			if (func)
			{
				it->second.push_back(func);
			}
			return it;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool good() const noexcept { return m_good; }

		inline bool open() const noexcept { return m_open; }

		inline auto & menus() noexcept { return m_menus; }

		inline auto const & menus() const noexcept { return m_menus; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Editor;

		MainMenuBar();

		~MainMenuBar();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool m_good;
		bool m_open;

		std::vector<std::pair<
			std::string,
			std::vector<menu_t>
		>> m_menus;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_MAIN_MENU_BAR_HPP_