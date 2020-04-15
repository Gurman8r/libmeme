#ifndef _ML_GUI_MANAGER_HPP_
#define _ML_GUI_MANAGER_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	struct ML_ENGINE_API gui_manager final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using menu_t			= typename std::function<void()>;
		using menu_bar_t		= typename pmr::vector<std::pair<cstring, pmr::vector<menu_t>>>;
		using dock_nodes_t		= typename pmr::vector<uint32_t>;

		static constexpr auto dockspace_title{ "dockspace##libmeme" };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_NODISCARD runtime final : trackable
		{
			// menu bar
			bool			show_main_menu	{ true };
			menu_bar_t		main_menus;

			// dockspace
			bool			show_dockspace	{ true };
			float_t			dockspace_border;
			vec2			dockspace_padding;
			float_t			dockspace_rounding;
			vec2			dockspace_size;
			float_t			dockspace_alpha;
			dock_nodes_t	dockspace_nodes;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		gui_manager(json const & j, allocator_type const & alloc = {}) noexcept;

		~gui_manager() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool startup(struct window const & win, cstring ver);

		ML_NODISCARD bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void new_frame();

		void render();

		void end_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_style(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_imgui_demo(bool * p_open = {});

		void show_imgui_metrics(bool * p_open = {});

		void show_imgui_about(bool * p_open = {});

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void add_menu(cstring label, menu_t && fn);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t begin_dockspace_builder(int32_t flags = 0);

		uint32_t end_dockspace_builder(uint32_t root);

		uint32_t dock(cstring name, uint32_t id);

		uint32_t split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

		uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

		uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool running() const noexcept { return m_gui; }

		runtime & io() & noexcept { return m_io; }

		runtime const & io() const & noexcept { return m_io; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void *	m_gui;
		runtime	m_io;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_GUI_MANAGER_HPP_