#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>

struct ImGuiContext;
struct ImFont;
struct ImFontAtlas;
struct ImFontConfig;
struct ImGuiStyle;

namespace ml { struct event_bus; }
namespace ml::gui { struct widget; }

// EDITOR DOCKSPACE
namespace ml
{
	struct ML_ENGINE_API editor_dockspace final : trackable, non_copyable
	{
		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		static constexpr auto	title		{ "dockspace##libmeme" };
		bool					visible		{ true };
		bool					menubar		{ true };
		float_t					border		{};
		vec2					padding		{};
		float_t					rounding	{};
		vec2					size		{};
		float_t					alpha		{};
		pmr::vector<uint32_t>	nodes		{};

		auto & operator[](size_t const i) & noexcept { return nodes[i]; }

		auto const & operator[](size_t const i) const & noexcept { return nodes[i]; }

		uint32_t begin_builder(int32_t flags = 0);

		uint32_t end_builder(uint32_t root);

		uint32_t dock(cstring name, uint32_t id);

		uint32_t dock(gui::widget const & w, uint32_t id);

		uint32_t split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

		uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * value);

		uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * value);

	private:
		friend struct editor_context;

		void render(event_bus * bus);

		editor_dockspace(json const & j, allocator_type alloc);
	};
}

// EDITOR CONTEXT
namespace ml
{
	struct ML_ENGINE_API editor_context final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		editor_context(event_bus * bus, render_window * win, json const & j, allocator_type alloc);

		~editor_context() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool startup();

		void shutdown();

		void new_frame();

		void render_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void show_imgui_demo(bool * p_open = {}) const;

		void show_imgui_metrics(bool * p_open = {}) const;

		void show_imgui_about(bool * p_open = {}) const;

		void show_imgui_style_editor(ImGuiStyle * ref = {}) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ImFont * load_font(fs::path const & path, float_t size, ImFontConfig const * cfg = 0, uint16_t const * ranges = 0);

		bool load_style(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_bus() const noexcept -> event_bus * { return m_bus; }

		ML_NODISCARD auto get_dockspace() noexcept -> editor_dockspace & { return m_dock; }

		ML_NODISCARD auto get_imgui() const noexcept -> ImGuiContext * { return m_imgui; }

		ML_NODISCARD auto get_window() const noexcept -> render_window * { return m_win; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		event_bus * const		m_bus		; // event bus
		editor_dockspace		m_dock		; // dockspace
		ImGuiContext *			m_imgui		; // imgui context
		render_window * const	m_win		; // render window

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EDITOR_HPP_