#ifndef _ML_IMGUI_EXT_HPP_
#define _ML_IMGUI_EXT_HPP_

#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Renderer/Texture.hpp>

namespace ml::gui
{
	// TOOLTIP
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn
	> static inline void tooltip_ex(Fn && fn) noexcept
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			std::invoke(ML_FWD(fn));
			ImGui::EndTooltip();
		}
	}

	static inline void tooltip(cstring first, cstring last = nullptr) noexcept
	{
		tooltip_ex([first, last]() noexcept
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(first, last);
			ImGui::PopTextWrapPos();
		});
	}

	static inline void tooltip_fmt(cstring fmt, ...) noexcept
	{
		char buf[1024] = "";
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, ML_ARRAYSIZE(buf), fmt, args);
		va_end(args);
		tooltip(buf);
	}

	template <class Str = pmr::string
	> static inline void tooltip(Str const & value) noexcept
	{
		tooltip(&(*value.begin()), &(*value.end()));
	}

	// HELP MARKER
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn
	> static inline void help_marker_ex(Fn && fn) noexcept
	{
		ImGui::TextDisabled("(?)");
		tooltip_ex(ML_FWD(fn));
	}

	static inline void help_marker(cstring first, cstring last = nullptr) noexcept
	{
		ImGui::TextDisabled("(?)");
		tooltip(first, last);
	}

	template <class Str = pmr::string
	> static inline void help_marker(Str const & value) noexcept
	{
		ImGui::TextDisabled("(?)");
		tooltip(value);
	}

	// PLOT
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct plot final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using getter_t	= typename float_t(*)(void);
		using overlay_t	= typename ds::array<char, 32>;
		using buffer_t	= typename pmr::vector<float_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		buffer_t	buffer		{};
		int32_t		mode		{};
		cstring		label		{};
		cstring		format		{ "%f" };
		vec2		graph_size	{};
		vec2		graph_scale	{ FLT_MAX, FLT_MAX };
		getter_t	getter		{ []() { return 0.f; } };

		int32_t		offset		{};
		overlay_t	overlay		{};
		bool		animate		{ true };
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void update() noexcept
		{
			ML_ASSERT(getter);
			update(std::invoke(getter));
		}

		inline void update(float_t const v) noexcept
		{
			if (!animate || buffer.empty()) return;
			std::sprintf(overlay.data(), format, v);
			buffer[offset] = v;
			offset = (offset + 1) % buffer.size();
		}

		inline void render() const noexcept
		{
			ML_ImGui_ScopeID(ML_ADDRESSOF(this));

			float_t width{ graph_size[0] };
			if ((width == 0.f) && (label && label[0] == '#' && label[1] == '#'))
			{
				width = ImGui::GetContentRegionAvailWidth();
			}

			switch (mode)
			{
			case 0: ImGui::PlotLines(label,
				buffer.data(), (int32_t)buffer.size(), offset,
				overlay.data(), graph_scale[0], graph_scale[1],
				{ width, graph_size[1] }, sizeof(float_t)
			); break;

			case 1: ImGui::PlotHistogram(label,
				buffer.data(), (int32_t)buffer.size(), offset,
				overlay.data(), graph_scale[0], graph_scale[1],
				{ width, graph_size[1] }, sizeof(float_t)
			); break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	template <class ... Args
	> static inline auto make_plot(size_t const cap, Args && ... args) noexcept
	{
		return plot{
			pmr::vector<float_t>{ cap, pmr::polymorphic_allocator<byte_t>{} }, ML_FWD(args)...
		};
	}

	// WIDGET
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct widget final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		cstring		title		{};
		bool		open		{};
		cstring		shortcut	{};
		int32_t		flags		{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn> inline void render(Fn && fn) noexcept
		{
			if (!open) return;
			ML_ImGui_ScopeID(ML_ADDRESSOF(this));
			ML_DEFER{ ImGui::End(); };
			if (ImGui::Begin(title, &open, flags))
			{
				std::invoke(ML_FWD(fn), flags);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline uint32_t get_id() const noexcept
		{
			return ImGui::GetID(title);
		}

		inline widget & set_focused() noexcept
		{
			open = true;
			ImGui::SetWindowFocus(title);
			return (*this);
		}

		inline bool menu_item() noexcept
		{
			return ImGui::MenuItem(title, shortcut, &open);
		}

		inline bool selectable(int32_t selectable_flags = 0, vec2 const & size = {}) noexcept
		{
			return ImGui::Selectable(title, &open, selectable_flags, size);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// PREVIEW
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct texture_preview final
	{
		texture const * value	{ nullptr };
		vec2			size	{ 0.f, 0.f };
		float_t			reg_zoom{ 4.f };
		float_t			reg_size{ 32.f };

		inline void render() noexcept
		{
			if (!value) return;

			auto const & io{ ImGui::GetIO() };

			auto const tex_addr	{ value->address() };
			auto const reg_avail{ ImGui::GetContentRegionAvail() };
			auto const scr_pos	{ ImGui::GetCursorScreenPos() };
			auto const tex_size	{ util::scale_to_fit((vec2)value->size(),
			{
				size[0] == 0.f ? reg_avail[0] : size[0],
				size[1] == 0.f ? reg_avail[1] : size[1]
			}) };

			ImGui::Image(
				tex_addr, tex_size,
				{ 0, 0 }, { 1, 1 },
				{ 1.f, 1.f, 1.f, 1.f },
				{ 1.f, 1.f, 1.f, .5f }
			);

			tooltip_ex([&]() noexcept
			{
				float_t region_x{ io.MousePos.x - scr_pos.x - reg_size * .5f };
				if (region_x < 0.f) region_x = 0.f;
				else if (region_x > (tex_size[0] - reg_size)) region_x = (tex_size[0] - reg_size);

				float_t region_y{ io.MousePos.y - scr_pos.y - reg_size * .5f };
				if (region_y < 0.f) region_y = 0.f;
				else if (region_y > (tex_size[1] - reg_size)) region_y = (tex_size[1] - reg_size);

				ImGui::Text("%u: %ux%u (%.0fx%.0f)",
					value->handle(), value->width(), value->height(), tex_size[0], tex_size[1]
				);
				ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
				ImGui::Text("Max: (%.2f, %.2f)", region_x + reg_size, region_y + reg_size);
				ImGui::Image(tex_addr,
					{ reg_size * reg_zoom, reg_size * reg_zoom },
					{ region_x / tex_size[0], region_y / tex_size[1] },
					{ (region_x + reg_size) / tex_size[0], (region_y + reg_size) / tex_size[1] },
					{ 1.f, 1.f, 1.f, 1.f },
					{ 1.f, 1.f, 1.f, .5f }
				);
			});
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IMGUI_EXT_HPP_