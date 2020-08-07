#ifndef _ML_GUI_MANAGER_HPP_
#define _ML_GUI_MANAGER_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/System/Memory.hpp>
#include <libmeme/Core/Matrix.hpp>

struct ImGuiContext;

namespace ml
{
	struct window;

	struct ML_ENGINE_API gui_manager final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit gui_manager(allocator_type alloc) noexcept;

		gui_manager(window const & wnd, allocator_type alloc = {}) noexcept;

		~gui_manager() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool startup(window const & wnd);

		void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void begin_frame();

		void draw_default();

		void end_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_style(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_imgui_demo(bool * p_open = nullptr);

		void show_imgui_metrics(bool * p_open = nullptr);

		void show_imgui_about(bool * p_open = nullptr);

		void show_imgui_style_editor(void * ref = nullptr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// DOCKSPACE
		struct ML_ENGINE_API dockspace_data final : non_copyable
		{
			using nodes_t = typename pmr::vector<uint32_t>;

			static constexpr auto title{ "dockspace##libmeme" };

			bool		visible	{ true };
			float_t		border	{};
			vec2		padding	{};
			float_t		rounding{};
			vec2		size	{};
			float_t		alpha	{};
			nodes_t		nodes;

			uint32_t begin_builder(int32_t flags = {});

			uint32_t end_builder(uint32_t root);

			uint32_t dock(cstring name, uint32_t id);

			uint32_t split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

			uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

			uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * value);

			auto & operator[](size_t const i) & noexcept { return nodes[i]; }

			auto const & operator[](size_t const i) const & noexcept { return nodes[i]; }

		private:
			friend gui_manager;
			
			explicit dockspace_data(allocator_type alloc) noexcept : nodes{ alloc }
			{
			}

			~dockspace_data() noexcept {}
		};

		auto & dockspace() & noexcept { return m_dockspace; }

		auto const & dockspace() const & noexcept { return m_dockspace; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// MAIN MENU BAR
		struct ML_ENGINE_API main_menu_bar_data final : non_copyable
		{
			using menus_t = pmr::vector<std::pair<
				cstring,
				pmr::vector<std::function<void()>
			>>>;
			
			bool		visible;
			menus_t		menus;

			template <class Fn> auto & add(cstring label, Fn && fn) & noexcept
			{
				auto it{ std::find_if(menus.begin(), menus.end(), [&
				](auto const & e) { return (0 == std::strcmp(e.first, label)); }) };
				if (it == menus.end())
				{
					menus.push_back({ label, {} });
					it = (menus.end() - 1);
				}
				return it->second.emplace_back(ML_forward(fn));
			}

		private:
			friend gui_manager;

			explicit main_menu_bar_data(allocator_type alloc) noexcept : menus{ alloc }
			{
			}

			~main_menu_bar_data() noexcept { this->menus.clear(); }
		};

		auto & main_menu_bar() & noexcept { return m_main_menu; }

		auto const & main_menu_bar() const & noexcept { return m_main_menu; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto get_context() const noexcept -> ImGuiContext * { return m_imgui; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ImGuiContext * m_imgui;
		dockspace_data m_dockspace;
		main_menu_bar_data m_main_menu;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_GUI_MANAGER_HPP_