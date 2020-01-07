#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Editor/Dockspace.hpp>
#include <libmeme/Editor/MainMenuBar.hpp>

#define ML_Editor ::ml::Editor::getInstance()

namespace ml
{
	struct ML_EDITOR_API Editor final : public Singleton<Editor>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup(void * window);

		static void new_frame();

		static void render_frame();

		static void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void show_about_window(bool * p_open = nullptr);
		
		static void show_imgui_demo(bool * p_open = nullptr);
		
		static void show_user_guide();
		
		static void show_style_editor(void * ref = nullptr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto & dockspace() noexcept { return m_dockspace; }

		inline auto & mainMenuBar() noexcept { return m_mainMenuBar; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<Editor>;
		
		Editor();
		~Editor();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Dockspace m_dockspace;
		MainMenuBar m_mainMenuBar;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EDITOR_HPP_