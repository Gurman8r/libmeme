#ifndef _ML_GUI_MANAGER_HPP_
#define _ML_GUI_MANAGER_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>

struct ImGuiContext;

// DOCKSPACE
namespace ml
{
	struct ML_ENGINE_API dockspace final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using nodes_t = typename pmr::vector<uint32_t>;

		static constexpr auto title{ "dockspace##libmeme" };

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

		uint32_t split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

		uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

		uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * value);

		void render(struct event_bus * bus);

		auto & operator[](size_t const i) & noexcept { return nodes[i]; }

		auto const & operator[](size_t const i) const & noexcept { return nodes[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// GUI WINDOW
namespace ml
{
	struct ML_ENGINE_API gui_window : render_window
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		gui_window() noexcept;

		explicit gui_window(window_settings const & ws) noexcept;

		virtual ~gui_window() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool open(window_settings const & ws) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void new_frame(struct event_bus * bus = {});

		void render_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_imgui_demo(bool * p_open = {}) const;

		void show_imgui_metrics(bool * p_open = {}) const;

		void show_imgui_about(bool * p_open = {}) const;

		void show_imgui_style_editor(void * ref = {}) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_style(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_gui_context() noexcept -> ImGuiContext * { return m_imgui; }

		ML_NODISCARD auto get_dockspace() & noexcept -> dockspace & { return m_dockspace; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ImGuiContext *	m_imgui		; // 
		dockspace		m_dockspace	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_GUI_MANAGER_HPP_