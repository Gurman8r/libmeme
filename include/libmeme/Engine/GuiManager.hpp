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

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_NODISCARD gui_dockspace final : non_copyable, trackable
		{
			using nodes_t = typename pmr::vector<uint32_t>;

			static constexpr auto title{ "dockspace##libmeme##builtin" };

			bool		visible	{ true };
			float_t		border	{};
			vec2		padding	{};
			float_t		rounding{};
			vec2		size	{};
			float_t		alpha	{};
			nodes_t		nodes	;

		private:
			friend gui_manager;
			
			gui_dockspace(allocator_type const & alloc = {}) noexcept : nodes{ alloc } {}
		
		} dockspace;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_NODISCARD gui_main_menu_bar final : non_copyable, trackable
		{
			using callback_t	= typename std::function<void()>;
			using menu_t		= typename std::pair<cstring, pmr::vector<callback_t>>;
			using menus_t		= typename pmr::vector<menu_t>;
			
			bool		visible	{ true };
			menus_t		menus	;

		private:
			friend gui_manager;

			gui_main_menu_bar(allocator_type const & alloc = {}) noexcept : menus{ alloc } {}

			~gui_main_menu_bar() noexcept { this->menus.clear(); }
		
		} main_menu;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		gui_manager(json const & j, allocator_type const & alloc = {}) noexcept;

		~gui_manager() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool startup(struct window const & win, cstring ver);

		ML_NODISCARD bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void new_frame();

		void draw();

		void end_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_style(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_imgui_demo(bool * p_open = {});

		void show_imgui_metrics(bool * p_open = {});

		void show_imgui_about(bool * p_open = {});

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void add_menu(cstring label, std::function<void()> && fn);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t begin_dockspace_builder(int32_t flags = 0);

		uint32_t end_dockspace_builder(uint32_t root);

		uint32_t dock(cstring name, uint32_t id);

		uint32_t split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

		uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

		uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void * m_imgui;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_GUI_MANAGER_HPP_