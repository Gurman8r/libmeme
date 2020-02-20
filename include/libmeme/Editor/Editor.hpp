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
			void *				imgui_context	{}		; // current imgui context
			editor_dockspace	dockspace		{}		; // main dockspace
			editor_main_menu	main_menu		{}		; // main menu bar
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// context
		struct context final : trackable, non_copyable
		{
			editor::config		config			{}		; // startup settings
			editor::io			io				{}		; // runtime variables

		private:
			friend class		editor					; // private data
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool create_context();

		ML_NODISCARD static bool initialized() noexcept;

		ML_NODISCARD static editor::context * const get_context() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool startup(bool install_callbacks);

		static void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void new_frame();

		static void render();

		static void render_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void show_about_window(bool * p_open = nullptr);
		
		static void show_imgui_demo(bool * p_open = nullptr);
		
		static void show_user_guide();
		
		static void show_style_editor(void * ref = nullptr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline editor::context & ref() noexcept
		{
			ML_BREAK_IF(!initialized());
			ML_ASSERT("editor is not initialized" && initialized());
			return (*get_context());
		}

		ML_NODISCARD static inline editor::context const & cref() noexcept
		{
			return static_cast<editor::context const &>(ref());
		}

		ML_NODISCARD static inline editor::config & get_config() noexcept
		{
			return ref().config;
		}

		ML_NODISCARD static inline editor::io & get_io() noexcept
		{
			return ref().io;
		}

		ML_NODISCARD static inline editor_dockspace & get_dockspace() noexcept
		{
			return get_io().dockspace;
		}

		ML_NODISCARD static inline editor_main_menu & get_main_menu() noexcept
		{
			return get_io().main_menu;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EDITOR_HPP_