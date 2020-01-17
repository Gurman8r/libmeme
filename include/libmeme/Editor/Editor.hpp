#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <libmeme/Core/ScopeGuard.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Editor/Dockspace.hpp>
#include <libmeme/Editor/MainMenuBar.hpp>

#ifdef IMGUI_VERSION
#	define ML_ImGui_ScopeID(id) ImGui::PushID(id); ML_SCOPE_EXIT{ ImGui::PopID(); };
#	else
#	define ML_ImGui_ScopeID(id)
#endif

namespace ml
{
	struct ML_EDITOR_API Editor final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Editor() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct EditorSettings final
		{
			void *	window_handle;
			bool	install_callbacks;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup(EditorSettings const & es);

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