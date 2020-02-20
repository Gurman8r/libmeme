#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <libmeme/Core/Memory.hpp>
#include <libmeme/Editor/Editor_Dockspace.hpp>

namespace ml
{
	class ML_EDITOR_API editor final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using menu_t = std::function<void()>;
		using main_menu_bar_t = pmr::vector<std::pair<cstring, pmr::vector<menu_t>>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// startup
		struct config final : trackable, non_copyable
		{
			pmr::string			api_version		{}			; // 
			pmr::string			style			{}			; // 
			cstring				ini_file		{}			; // 
			cstring				log_file		{}			; // 
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// runtime
		struct io final : trackable, non_copyable
		{
			bool				show_main_menu	{ true }	; // 
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// context
		struct context final : trackable, non_copyable
		{
		private:
			friend class		editor						;
			editor::config		m_config		{}			; // startup settings
			editor::io			m_io			{}			; // runtime variables
			void *				m_imgui_context	{}			; // current imgui context
			editor_dockspace	m_dockspace		{}			; // dockspace
			main_menu_bar_t		m_main_menu		{}			; // main menu bar
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool initialized() noexcept;

		ML_NODISCARD static bool create_context();

		static bool destroy_context();

		ML_NODISCARD static editor::context * const get_context() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool startup(bool install_callbacks);

		static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void new_frame();

		static void render();

		static void render_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void add_menu(cstring label, menu_t && value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void show_imgui_demo(bool * p_open);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline editor::config & get_config() noexcept
		{
			return get_context()->m_config;
		}

		ML_NODISCARD static inline editor::io & get_io() noexcept
		{
			return get_context()->m_io;
		}

		ML_NODISCARD static inline auto * const get_imgui_context() noexcept
		{
			return static_cast<struct ImGuiContext * const>(get_context()->m_imgui_context);
		}

		ML_NODISCARD static inline editor_dockspace & get_dockspace() noexcept
		{
			return get_context()->m_dockspace;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EDITOR_HPP_