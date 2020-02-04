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

		struct config final : trackable
		{
			void *		window_handle;	// 
			cstring	api_version;	// 
			cstring	style;	// 
			cstring	ini_file;		// 
			cstring	log_file;		// 
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct io final : trackable
		{
			// ...
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct context final : trackable
		{
			config				config		{};
			io					io			{};
			editor_dockspace	dockspace	{};
			editor_main_menu	main_menu	{};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static editor::context const * create_context();

		static bool startup(bool install_callbacks);

		static void new_frame();

		static void render_frame();

		static void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void show_about_window(bool * p_open = nullptr);
		
		static void show_imgui_demo(bool * p_open = nullptr);
		
		static void show_user_guide();
		
		static void show_style_editor(void * ref = nullptr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static editor::context const * get_context() noexcept;

		ML_NODISCARD static editor::config & get_config() noexcept;

		ML_NODISCARD static editor::io & get_io() noexcept;

		ML_NODISCARD static editor_dockspace & get_dockspace() noexcept;

		ML_NODISCARD static editor_main_menu & get_main_menu() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void draw_texture_preview(texture const & value, vec2 const & maxSize = { 0 });

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EDITOR_HPP_