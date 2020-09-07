#ifndef _ML_EDITOR_WINDOW_HPP_
#define _ML_EDITOR_WINDOW_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>

struct ImGuiContext;

namespace ml
{
	struct event_bus;

	namespace gui { struct widget; }

	struct ML_ENGINE_API editor_window : render_window
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		editor_window() noexcept;

		explicit editor_window(window_settings const & ws) noexcept;

		virtual ~editor_window() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool open(window_settings const & ws) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void begin_gui_frame(event_bus * bus = {});

		void end_gui_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_ENGINE_API dockspace final : trackable, non_copyable
		{
			using nodes_t = typename pmr::vector<uint32_t>;

			static constexpr auto title{ "dockspace##editor_window##libmeme" };

			bool	visible	{ true };
			bool	menubar	{ true };
			float_t	border	{};
			vec2	padding	{};
			float_t	rounding{};
			vec2	size	{};
			float_t	alpha	{};
			nodes_t	nodes	{};

			uint32_t begin_builder(int32_t flags = 0);

			uint32_t end_builder(uint32_t root);

			uint32_t dock(cstring name, uint32_t id);

			uint32_t dock(gui::widget const & w, uint32_t id);

			uint32_t split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

			uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

			uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * value);

			auto & operator[](size_t const i) & noexcept { return nodes[i]; }

			auto const & operator[](size_t const i) const & noexcept { return nodes[i]; }

		private:
			friend editor_window;

			dockspace() noexcept = default;

			void render(event_bus * bus);
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_imgui_demo(bool * p_open = {}) const;

		void show_imgui_metrics(bool * p_open = {}) const;

		void show_imgui_about(bool * p_open = {}) const;

		void show_imgui_style_editor(void * ref = {}) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_style(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_imgui() noexcept -> ImGuiContext * { return m_imgui; }

		ML_NODISCARD auto get_dockspace() & noexcept -> dockspace & { return m_dockspace; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ImGuiContext *	m_imgui		; // 
		dockspace		m_dockspace	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EDITOR_WINDOW_HPP_