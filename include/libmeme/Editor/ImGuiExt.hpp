#ifndef _ML_IMGUI_EXT_HPP_
#define _ML_IMGUI_EXT_HPP_

#include <libmeme/Editor/ImGui.hpp>

namespace ml::gui
{
	// TOOLTIP
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn
	> static inline void tooltip_ex(Fn && fn)
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			std::invoke(ML_FWD(fn));
			ImGui::EndTooltip();
		}
	}

	static inline void tooltip(cstring first, cstring last = nullptr)
	{
		tooltip_ex([first, last]() noexcept
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(first, last);
			ImGui::PopTextWrapPos();
		});
	}

	template <class Str = pmr::string
	> static inline void tooltip(Str const & value)
	{
		tooltip(&(*value.begin()), &(*value.end()));
	}

	// HELP MARKER
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn
	> static inline void help_marker_ex(Fn && fn)
	{
		ImGui::TextDisabled("(?)");
		tooltip_ex(ML_FWD(fn));
	}

	static inline void help_marker(cstring first, cstring last = nullptr)
	{
		ImGui::TextDisabled("(?)");
		tooltip(first, last);
	}

	template <class Str = pmr::string
	> static inline void help_marker(Str const & value)
	{
		ImGui::TextDisabled("(?)");
		tooltip(value);
	}

	// PLOT LINES
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct plot final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum	plot_mode	: int32_t { lines, histogram };
		using	overlay_t	= ds::array<char, 64>;
		using	buffer_t	= ds::array<float_t, 120>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		plot_mode	mode		{};
		cstring		title		{};
		vec2		scale		{ FLT_MAX, FLT_MAX };
		vec2		size		{ 0.f, 0.f };
		int32_t		stride		{ sizeof(float_t) };

		bool		animate		{ true	};
		int32_t		offset		{};
		float64_t	ref_time	{};
		overlay_t	overlay		{};
		buffer_t	buffer		{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline plot & update(float_t const value, cstring fmt, ...) noexcept
		{
			va_list args;
			va_start(args, fmt);
			std::vsnprintf(overlay.data(), overlay.size() - 1, fmt, args);
			va_end(args);
			return update(value);
		}

		inline plot & update(float_t const value, float_t const dt = 1.f / 60.f) noexcept
		{
			return update(value, ImGui::GetTime(), dt);
		}

		inline plot & update(float_t const value, float64_t const tt, float_t const dt) noexcept
		{
			if (!animate || ref_time == 0.0)
			{
				ref_time = tt;
			}
			while (ref_time < tt)
			{
				buffer[offset] = value;
				offset = (offset + 1) % buffer.size();
				ref_time += dt;
			}
			return (*this);
		}

		inline plot const & render() const noexcept
		{
			ML_ImGui_ScopeID(ML_ADDRESSOF(this));

			float_t width{ size[0] };
			if ((width == 0.f) && (title && title[0] == '#' && title[1] == '#'))
			{
				width = ImGui::GetContentRegionAvailWidth();
			}

			switch (mode)
			{
			case lines: ImGui::PlotLines(title,
				buffer.data(), (int32_t)buffer.size(), offset,
				overlay.data(), scale[0], scale[1], { width, size[1] }, stride
			); break;

			case histogram: ImGui::PlotHistogram(title,
				buffer.data(), (int32_t)buffer.size(), offset,
				overlay.data(), scale[0], scale[1], { width, size[1] }, stride
			); break;
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// WIDGET
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct widget final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		cstring		title		{};	// 
		bool		open		{};	// 
		cstring		shortcut	{};	// 
		int32_t		flags		{};	// 

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool menu_item() noexcept
		{
			return ImGui::MenuItem(title, shortcut, &open);
		}

		inline bool selectable(int32_t selectable_flags = 0, vec2 const & size = {}) noexcept
		{
			return ImGui::Selectable(title, &open, selectable_flags, size);
		}

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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IMGUI_EXT_HPP_