#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Editor/Dockspace.hpp>
#include <libmeme/Editor/MainMenuBar.hpp>

namespace ml
{
	struct ML_EDITOR_API Editor final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Editor() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct StartupSettings
		{
			void *	window;
			bool	install_callbacks;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup(StartupSettings const & settings);

		static void new_frame();

		static void render_frame();

		static void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void show_about_window(bool * p_open = nullptr);
		
		static void show_imgui_demo(bool * p_open = nullptr);
		
		static void show_user_guide();
		
		static void show_style_editor(void * ref = nullptr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline auto & dockspace() noexcept { return s_dockspace; }

		static inline auto & main_menu() noexcept { return s_mainMenuBar; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static Dockspace s_dockspace;
		static MainMenuBar s_mainMenuBar;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EDITOR_HPP_