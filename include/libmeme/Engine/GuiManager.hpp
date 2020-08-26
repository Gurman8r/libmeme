#ifndef _ML_GUI_MANAGER_HPP_
#define _ML_GUI_MANAGER_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Matrix.hpp>

struct ImGuiContext;

namespace ml
{
	struct window;
	struct event_bus;

	struct ML_ENGINE_API gui_manager final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		gui_manager(event_bus * bus, allocator_type alloc = {}) noexcept;

		gui_manager(window const & wnd, event_bus * bus, allocator_type alloc = {}) noexcept;

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
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using nodes_t = typename pmr::vector<uint32_t>;

			static constexpr auto title{ "dockspace##libmeme" };

			bool		visible	{ true };
			bool		menubar	{ true };
			float_t		border	{};
			vec2		padding	{};
			float_t		rounding{};
			vec2		size	{};
			float_t		alpha	{};
			nodes_t		nodes	{};

			uint32_t begin_builder(int32_t flags = {});

			uint32_t end_builder(uint32_t root);

			uint32_t dock(cstring name, uint32_t id);

			uint32_t split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

			uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

			uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * value);

			auto & operator[](size_t const i) & noexcept { return nodes[i]; }

			auto const & operator[](size_t const i) const & noexcept { return nodes[i]; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}
		dockspace{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto get_context() const noexcept -> ImGuiContext * { return m_imgui; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ImGuiContext * m_imgui;
		event_bus * m_bus;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_GUI_MANAGER_HPP_