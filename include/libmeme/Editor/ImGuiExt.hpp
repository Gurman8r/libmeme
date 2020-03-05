#ifndef _ML_IMGUI_EXT_HPP_
#define _ML_IMGUI_EXT_HPP_

#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Renderer/Texture.hpp>
#include <libmeme/Core/StringUtility.hpp>

// TOOLTIP
namespace ml::gui
{
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
		std::vsnprintf(buf, ML_ARRAYSIZE(buf), fmt, args);
		va_end(args);
		tooltip(buf);
	}

	template <class Str = pmr::string
	> static inline void tooltip(Str const & value) noexcept
	{
		tooltip(&(*value.begin()), &(*value.end()));
	}
}

// HELP MARKER
namespace ml::gui
{
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
}

// PLOTS
namespace ml::gui
{
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

	struct plot_controller final
	{
		pmr::vector<plot> m_plots{};
		
		float64_t m_ref_time{};

		template <class Fn> inline auto for_each(Fn && fn) noexcept
		{
			return std::for_each(m_plots.begin(), m_plots.end(), ML_FWD(fn));
		}

		inline void update(float64_t const tt, float_t const dt = 1.f / 60.f) noexcept
		{
			if (m_ref_time == 0.0)
			{
				m_ref_time = tt; return;
			}
			while (m_ref_time < tt)
			{
				this->for_each([&](auto & p) { p.update(); });

				m_ref_time += dt;
			}
		}
	};
}

// WIDGET
namespace ml::gui
{
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
				std::invoke(ML_FWD(fn), *this);
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
}

// TEXTURE PREVIEW
namespace ml::gui
{
	struct texture_preview final
	{
		texture const * value	{ nullptr };
		vec2			size	{ 0.f, 0.f };
		float_t			reg_zoom{ 4.f };
		float_t			reg_size{ 32.f };

		template <class Fn
		> inline void render(Fn && fn) noexcept
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

			std::invoke(ML_FWD(fn));

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
}

// CONSOLE
namespace ml::gui
{
	struct console final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using command = typename std::pair
		<
			cstring, std::function<void(pmr::vector<pmr::string>)>
		>;

		ds::array<char, 256>		input			{};
		pmr::vector<pmr::string>	lines			{};
		pmr::vector<command>		commands		{};
		pmr::vector<pmr::string>	history			{};
		int32_t						history_pos		{ -1 };
		ImGuiTextFilter				filter			{};
		bool						auto_scroll		{ true };
		bool						scroll_to_bot	{};
		cstring						overload		{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline console & clear()
		{
			lines.clear();
			return (*this);
		}

		inline console & write(char value)
		{
			switch (value)
			{
			default:
				if (lines.empty()) { lines.push_back({}); }
				lines.back().push_back(value);
				break;
			case '\n':
				lines.push_back({});
				break;
			}
			return (*this);
		}

		inline console & printl(pmr::string const & value = {})
		{
			return this->print(value).write('\n');
		}

		inline console & print(pmr::string const & value)
		{
			if (value.empty())
			{
				return (*this);
			}
			else if (value.find('\n') == value.npos)
			{
				for (char const c : value)
				{
					this->write(c);
				}
			}
			else if (auto const toks{ util::tokenize(value, "\n") }; !toks.empty())
			{
				auto it{ toks.begin() };
				this->print(*it);
				while (++it != toks.end())
				{
					this->printl(*it);
				}
			}
			return (*this);
		}

		inline console & printss(std::stringstream & value)
		{
			if (auto const str{ value.str() }; !str.empty())
			{
				std::stringstream ss{ str };
				pmr::string line{};
				while (std::getline(ss, line))
				{
					this->printl(line);
				}
				value.str({});
			}
			return (*this);
		}

		inline console & printf(cstring fmt, ...)
		{
			ds::array<char, 1024> buf{};
			va_list args;
			va_start(args, fmt);
			std::vsnprintf(buf.data(), buf.size(), fmt, args);
			buf.back() = 0;
			va_end(args);
			return this->print(buf.data());
		}

		inline console & execute(cstring value)
		{
			this->printf("# %s\n", value);

			// update history
			history_pos = -1;
			if (auto const it{ std::find(history.begin(), history.end(), value) }
			; it != history.end())
			{
				history.erase(it);
			}
			history.push_back(value);

			// process command
			if (auto toks{ util::tokenize(value, " ") }; !toks.empty())
			{
				if (overload) { toks.insert(toks.begin(), overload); }

				if (auto const it{ std::find_if(commands.begin(), commands.end(), [&](auto & e)
				{
					return e.first == toks.front();
				}) }
				; it != commands.end())
				{
					toks.erase(toks.begin());

					std::invoke(it->second, std::move(toks));
				}
				else
				{
					this->printf("unknown command: \'%s\'\n", toks.front().c_str());
				}
			}

			scroll_to_bot = true;
			return (*this);
		}

		inline void render()
		{
			ML_ImGui_ScopeID(ML_ADDRESSOF(this));

			// HEADER
			filter.Draw("filter", 180); ImGui::SameLine();
			ImGui::Checkbox("auto-scroll", &auto_scroll); ImGui::SameLine();
			if (ImGui::Button("clear")) clear(); //ImGui::SameLine();
			ImGui::Separator();

			float_t const footer_height{ ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing() };

			// CONTENT
			ImGui::BeginChild("console content area",
				{ 0, -footer_height }, false, ImGuiWindowFlags_HorizontalScrollbar
			);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 4, 1 });
			for (auto const & line : lines)
			{
				auto const str{ line.c_str() };
				if (!filter.PassFilter(str)) continue;

				bool pop_color{};
				if (std::strstr(str, "[error]"))
				{
					ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.4f, 0.4f, 1.0f }); pop_color = true;
				}
				else if (std::strncmp(str, "# ", 2) == 0)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.8f, 0.6f, 1.0f }); pop_color = true;
				}
				ImGui::TextUnformatted(str);
				if (pop_color)
				{
					ImGui::PopStyleColor();
				}
			}
			if (scroll_to_bot || (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
			{
				ImGui::SetScrollHereY(1.0f);
			}
			scroll_to_bot = false;
			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();

			// COMMAND LINE
			bool reclaim_focus{};
			ImGui::TextDisabled("$:"); ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			if (ImGui::InputText("##input", &input[0], ML_ARRAYSIZE(input),
				ImGuiInputTextFlags_EnterReturnsTrue |
				ImGuiInputTextFlags_CallbackCompletion |
				ImGuiInputTextFlags_CallbackHistory,
				[](auto * u) { return ((console *)u->UserData)->text_edit_callback(u); },
				this
			))
			{
				if (auto const s{ util::trim((cstring)input.data()) }; !s.empty())
				{
					execute(s.c_str());
				}
				std::strcpy(input.data(), "");
				reclaim_focus = true;
			}
			ImGui::PopItemWidth();

			// Auto-focus on window apparition
			ImGui::SetItemDefaultFocus();
			if (reclaim_focus)
			{
				ImGui::SetKeyboardFocusHere(-1); // Auto focus previous window
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		inline int32_t text_edit_callback(ImGuiInputTextCallbackData * data)
		{
			switch (data->EventFlag)
			{
			case ImGuiInputTextFlags_CallbackCompletion:
			{
				// TEXT COMPLETION

				// Locate beginning of current word
				cstring word_end = data->Buf + data->CursorPos;
				cstring word_start = word_end;
				while (word_start > data->Buf)
				{
					const char c = word_start[-1];
					if (c == ' ' || c == '\t' || c == ',' || c == ';')
						break;
					word_start--;
				}

				// Build a list of candidates
				pmr::vector<cstring> candidates;
				for (size_t i = 0; i < commands.size(); i++)
					if (std::strncmp(commands[i].first, word_start, (size_t)(word_end - word_start)) == 0)
						candidates.push_back(commands[i].first);

				if (candidates.size() == 0)
				{
					// No match
					this->printf("No match for \'%.*s\'!\n", (size_t)(word_end - word_start), word_start);
				}
				else if (candidates.size() == 1)
				{
					// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
					data->DeleteChars((int32_t)(word_start - data->Buf), (int32_t)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0]);
					data->InsertChars(data->CursorPos, " ");
				}
				else
				{
					// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
					size_t match_len = (size_t)(word_end - word_start);
					for (;;)
					{
						size_t c = 0;
						bool all_candidates_matches = true;
						for (size_t i = 0; i < candidates.size() && all_candidates_matches; i++)
							if (i == 0)
								c = toupper(candidates[i][match_len]);
							else if (c == 0 || c != toupper(candidates[i][match_len]))
								all_candidates_matches = false;
						if (!all_candidates_matches)
							break;
						match_len++;
					}

					if (match_len > 0)
					{
						data->DeleteChars((int32_t)(word_start - data->Buf), (int32_t)(word_end - word_start));
						data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
					}

					// List matches
					this->printf("Possible matches:\n");
					for (size_t i = 0; i < candidates.size(); i++)
						this->printf("- %s\n", candidates[i]);
				}

				break;
			}
			case ImGuiInputTextFlags_CallbackHistory:
			{
				// Example of HISTORY
				const size_t prev_history_pos = history_pos;
				if (data->EventKey == ImGuiKey_UpArrow)
				{
					if (history_pos == -1)
						history_pos = (int32_t)history.size() - 1;
					else if (history_pos > 0)
						history_pos--;
				}
				else if (data->EventKey == ImGuiKey_DownArrow)
				{
					if (history_pos != -1)
						if (++history_pos >= (int32_t)history.size())
							history_pos = -1;
				}

				// A better implementation would preserve the data on the current input line along with cursor position.
				if (prev_history_pos != history_pos)
				{
					cstring history_str = (history_pos >= 0) ? history[history_pos].c_str() : "";
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, history_str);
				}
			}
			}
			return 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_IMGUI_EXT_HPP_