#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <libmeme/Renderer/Texture.hpp>
#include <libmeme/Editor/Editor_Dockspace.hpp>
#include <libmeme/Editor/Editor_MainMenuBar.hpp>

namespace ml
{
	class ML_EDITOR_API editor final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// startup
		struct config final : trackable, non_copyable
		{
			void *				window_handle	{}		; // 
			pmr::string			api_version		{}		; // 
			pmr::string			style			{}		; // 
			cstring				ini_file		{}		; // 
			cstring				log_file		{}		; // 
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// runtime
		struct io final : trackable, non_copyable
		{
			editor_dockspace	dockspace		{}		; // 
			editor_main_menu	main_menu		{}		; // 
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// context
		class context final : trackable, non_copyable
		{
			friend class		editor					; // 
			editor::config		config			{}		; // 
			editor::io			io				{}		; // 
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool create_context();

		static bool startup(bool install_callbacks);

		static void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void new_frame();

		static void render_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void show_about_window(bool * p_open = nullptr);
		
		static void show_imgui_demo(bool * p_open = nullptr);
		
		static void show_user_guide();
		
		static void show_style_editor(void * ref = nullptr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool initialized() noexcept;

		ML_NODISCARD static editor::context const * const get_context() noexcept;

		ML_NODISCARD static editor::config & get_config() noexcept;

		ML_NODISCARD static editor::io & get_io() noexcept;

		ML_NODISCARD static editor_dockspace & get_dockspace() noexcept;

		ML_NODISCARD static editor_main_menu & get_main_menu() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EDITOR_HPP_