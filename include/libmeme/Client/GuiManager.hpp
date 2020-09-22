#ifndef _ML_GUI_MANAGER_HPP_
#define _ML_GUI_MANAGER_HPP_

#include <libmeme/Client/Export.hpp>
#include <libmeme/Client/ImGui.hpp>

namespace ml
{
	struct event_bus;
	struct render_window;
	namespace gui { struct form; }

	struct ML_CLIENT_API gui_manager final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		gui_manager(event_bus * bus, render_window * win, allocator_type alloc);

		~gui_manager();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool startup(bool install_callbacks = true);

		void shutdown();

		void new_frame();

		void render_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void imgui_demo(bool * p_open = {});

		static void imgui_metrics(bool * p_open = {});

		static void imgui_about(bool * p_open = {});

		static void imgui_style_editor(ImGuiStyle * ref = {});

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// main menu bar
		struct main_menu_bar final : trackable, non_copyable
		{
			bool enabled{ true };

			explicit main_menu_bar(allocator_type alloc) noexcept {}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// dockspace
		struct ML_CLIENT_API dockspace final : trackable, non_copyable
		{
			cstring					title		{ "dockspace##libmeme" };
			bool					enabled		{ true };
			float_t					border		{};
			vec2					padding		{};
			float_t					rounding	{};
			vec2					size		{};
			float_t					alpha		{};
			int32_t					flags		{ ImGuiDockNodeFlags_AutoHideTabBar };
			pmr::vector<uint32_t>	nodes		{};

			explicit dockspace(allocator_type alloc) noexcept : nodes{ alloc } {}

			ML_NODISCARD auto & operator[](size_t i) noexcept { return nodes[i]; }

			ML_NODISCARD auto const & operator[](size_t i) const noexcept { return nodes[i]; }

			uint32_t begin_builder();

			uint32_t end_builder(uint32_t root);

			uint32_t dock(cstring name, uint32_t id);

			uint32_t dock(gui::form const & w, uint32_t id);

			uint32_t split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

			uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

			uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * value);
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_style(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_docker() noexcept -> dockspace & { return m_docker; }

		ML_NODISCARD auto get_menubar() noexcept -> main_menu_bar & { return m_menubar; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		render_window * const	m_win		; // 
		event_bus * const		m_bus		; // 
		ImGuiContext *			m_ctx		; // 
		main_menu_bar			m_menubar	; // 
		dockspace				m_docker	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_GUI_MANAGER_HPP_