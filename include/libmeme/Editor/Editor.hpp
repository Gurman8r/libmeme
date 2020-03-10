#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <libmeme/Editor/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Core/NonCopyable.hpp>

namespace ml
{
	class ML_EDITOR_API editor final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		class context;
		struct config;
		struct runtime;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using menu_t		= std::function<void()>;
		using menu_bar_t	= pmr::vector<std::pair<cstring, pmr::vector<menu_t>>>;
		using dock_nodes_t	= pmr::vector<uint32_t>;

		static constexpr auto dockspace_title{ "dockspace##editor##libmeme" };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// startup variables
		struct config final : trackable
		{
			filesystem::path	style_config	{}			; // style config
			pmr::string			api_version		{}			; // imgui shading version
			cstring				ini_filename	{}			; // imgui ini filename
			cstring				log_filename	{}			; // imgui log filename
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// runtime variables
		struct runtime final : trackable
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			bool				show_main_menu	{ true }	; // show main meu bar
			menu_bar_t			main_menus		{}			; // main menu bar menus

			bool				show_dockspace	{ true }	; // show dockspace
			float_t				dock_border		{}			; // dockspace border
			vec2				dock_padding	{}			; // dockspace padding
			float_t				dock_rounding	{}			; // dockspace rounding
			vec2				dock_size		{}			; // dockspace size
			float_t				dock_alpha		{ 1.f }		; // dockspace transparancy
			dock_nodes_t		dock_nodes		{}			; // dockspace dock-nodes

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// editor context
		class context final : trackable, non_copyable
		{
			friend class		editor						;
			editor::config		m_config		{}			; // startup variables
			editor::runtime		m_io			{}			; // runtime variables
			void *				m_imgui_context	{}			; // current imgui context
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool is_initialized() noexcept;

		ML_NODISCARD static bool create_context(json const & j);

		ML_NODISCARD static bool destroy_context();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static editor::config & get_config() noexcept;

		ML_NODISCARD static editor::runtime & get_runtime() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool startup();

		ML_NODISCARD static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void new_frame();

		static void render();

		static void render_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void show_imgui_demo(bool * p_open = {});

		static void show_imgui_metrics(bool * p_open = {});

		static void show_imgui_about(bool * p_open = {});

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void add_menu(cstring label, menu_t && fn);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static uint32_t begin_builder(int32_t flags = 0);
		
		static uint32_t end_builder(uint32_t root);
		
		static uint32_t dock(cstring name, uint32_t id);
		
		static uint32_t split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * other);
		
		static uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * other);
		
		static uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * other);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EDITOR_HPP_