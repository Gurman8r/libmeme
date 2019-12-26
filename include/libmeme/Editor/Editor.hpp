#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <libmeme/Editor/Export.hpp>
#include <libmeme/Core/Singleton.hpp>

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

	private:
		friend struct Singleton<Editor>;
		
		Editor();
		
		~Editor();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EDITOR_HPP_