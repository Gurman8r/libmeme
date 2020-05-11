#ifndef _ML_IMGUI_EXT_HPP_
#define _ML_IMGUI_EXT_HPP_

#include <libmeme/Engine/ImGui.hpp>
#include <libmeme/Graphics/Texture.hpp>
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
			std::invoke(ML_forward(fn));
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
		std::vsnprintf(buf, ML_arraysize(buf), fmt, args);
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
		tooltip_ex(ML_forward(fn));
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
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// plot
	struct plot final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : int32_t { lines, histogram };

		using delta_t	= typename float_t;
		using buffer_t	= typename pmr::vector<delta_t>;
		using overtxt_t	= typename ds::array<char, 32>;
		using get_fn_t	= typename std::function<delta_t(void)>;

		buffer_t	buffer	{};
		int32_t		mode	{};
		cstring		label	{};
		cstring		fmt		{ "%f" };
		vec2		size	{};
		vec2		scale	{ FLT_MAX, FLT_MAX };
		get_fn_t	get_fn	{ []() { return 0.f; } };
		int32_t		offset	{};
		overtxt_t	overtxt	{ "" };
		bool		animate	{ true };
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// make plot
		template <class ... Args
		> static auto create(size_t const cap, Args && ... args) noexcept
		{
			return plot{ buffer_t{ cap, buffer_t::allocator_type{} }, ML_forward(args)... };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void update() noexcept
		{
			ML_assert(get_fn);
			update(std::invoke(get_fn));
		}

		template <class Delta = delta_t
		> void update(Delta const v) noexcept
		{
			static_assert(std::is_floating_point_v<Delta>);
			if (!animate || buffer.empty()) { return; }
			std::sprintf(overtxt.data(), fmt, v);
			buffer[offset] = static_cast<delta_t>(v);
			offset = (offset + 1) % buffer.size();
		}

		void render() const noexcept
		{
			ML_scoped_imgui_id(ML_addressof(this));

			// expand to available width
			float_t width{ size[0] };
			if ((width == 0.f) && (label && label[0] == '#' && label[1] == '#'))
			{
				width = ImGui::GetContentRegionAvailWidth();
			}

			// draw
			switch (mode)
			{
			case lines:
				return ImGui::PlotLines(label
					, buffer.data(), (int32_t)buffer.size(), offset
					, overtxt.data()
					, scale[0], scale[1], { width, size[1] }
					, sizeof(delta_t)
				);
			case histogram:
				return ImGui::PlotHistogram(label
					, buffer.data(), (int32_t)buffer.size(), offset
					, overtxt.data()
					, scale[0], scale[1], { width, size[1] }
					, sizeof(delta_t)
				);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// plot controller
	struct plot_controller final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using delta_t = typename float_t;

		template <class Delta = delta_t
		> static constexpr auto sixty_fps{ Delta{ 1 } / Delta{ 60 } };

		pmr::vector<plot> plots{};
		
		delta_t ref_time{};

		template <class Fn> auto for_each(Fn && fn) noexcept
		{
			return std::for_each(plots.begin(), plots.end(), ML_forward(fn));
		}

		template <class Delta = delta_t
		> void update(Delta const tt, Delta const dt = sixty_fps<Delta>) noexcept
		{
			if (ref_time == delta_t{ 0 })
			{
				return (void)(ref_time = static_cast<delta_t>(tt));
			}
			while (ref_time < static_cast<delta_t>(tt))
			{
				this->for_each([&](auto & p) { p.update(); });

				ref_time += static_cast<delta_t>(dt);
			}
			return;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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

		template <class Fn, class ... Args
		> void render(Fn && fn, Args && ... args) noexcept
		{
			if (!open) { return; }
			ML_scoped_imgui_id(ML_addressof(this));
			ML_defer{ ImGui::End(); };
			if (ImGui::Begin(title, &open, flags))
			{
				std::invoke(ML_forward(fn), ML_forward(args)...);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t get_id() const noexcept
		{
			return ImGui::GetID(title);
		}

		widget & set_focused() noexcept
		{
			open = true;
			ImGui::SetWindowFocus(title);
			return (*this);
		}

		bool menu_item() noexcept
		{
			return ImGui::MenuItem(title, shortcut, &open);
		}

		bool selectable(int32_t selectable_flags = 0, vec2 const & size = {}) noexcept
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
		> void render(Fn && fn) noexcept
		{
			if (!value) { return; }

			auto const & io		{ ImGui::GetIO() };
			auto const tex_addr	{ value->address() };
			auto const reg_avail{ ImGui::GetContentRegionAvail() };
			auto const scr_pos	{ ImGui::GetCursorScreenPos() };
			auto const tex_size	{ util::scale_to_fit((vec2)value->size(),
			{
				size[0] == 0.f ? reg_avail[0] : size[0],
				size[1] == 0.f ? reg_avail[1] : size[1]
			}) };

			std::invoke(ML_forward(fn));

			ImGui::Image(
				tex_addr, tex_size,
				{ 0, 0 }, { 1, 1 },
				{ 1.f, 1.f, 1.f, 1.f },
				{ 1.f, 1.f, 1.f, .5f }
			);

			// zoom tooltip region
			if ((0.f < reg_size) && (0.f < reg_zoom)) tooltip_ex([&]() noexcept
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

		using arguments = typename pmr::vector<pmr::string>;

		using command = typename std::pair<
			cstring, std::function<void(arguments)>
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

		console & clear()
		{
			lines.clear();
			return (*this);
		}

		console & write(char const value)
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

		console & print(pmr::string const & value)
		{
			if (!value.empty())
			{
				for (char const & c : value)
				{
					this->write(c);
				}
			}
			return (*this);
		}

		console & printl(pmr::string const & value = {})
		{
			return this->print(value).write('\n');
		}

		console & printss(pmr::stringstream & value)
		{
			// FIXME: inefficient
			if (auto const str{ value.str() }; !str.empty())
			{
				pmr::stringstream ss{ str };
				pmr::string line{};
				while (std::getline(ss, line))
				{
					this->printl(line);
				}
				value.str({});
			}
			return (*this);
		}

		console & printf(cstring fmt, ...)
		{
			ds::array<char, 1024> buf{};
			va_list args;
			va_start(args, fmt);
			std::vsnprintf(buf.data(), buf.size(), fmt, args);
			buf.back() = 0;
			va_end(args);
			return this->print(buf.data());
		}

		console & execute(cstring value)
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
				// FIXME: inefficient

				if (overload) { toks.insert(toks.begin(), overload); }

				if (auto const it{ std::find_if(commands.begin(), commands.end(), [&
				](auto & e) { return e.first == toks.front(); }) }
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

		console & render()
		{
			ML_scoped_imgui_id(ML_addressof(this));

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
			if (ImGui::InputText("##input", &input[0], ML_arraysize(input),
				ImGuiInputTextFlags_EnterReturnsTrue |
				ImGuiInputTextFlags_CallbackCompletion |
				ImGuiInputTextFlags_CallbackHistory,
				[](auto * u) { return ((console *)u->UserData)->text_edit_callback(u); },
				this
			))
			{
				if (auto const s{ util::trim((pmr::string)input.data()) }; !s.empty())
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

			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		int32_t text_edit_callback(ImGuiInputTextCallbackData * data)
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
					char const c = word_start[-1];
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
				// HISTORY
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

// FILE TREE
namespace ml::gui
{
	// FIXME: VERY SLOW
	struct file_tree final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		fs::path m_path{};

		file_tree() noexcept = default;

		void render() const
		{
			if (m_path.empty() || !fs::is_directory(m_path)) { return; }

			ML_scoped_imgui_id(ML_addressof(this));

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 2, 2 });
			ImGui::Separator();

			for (auto const & e : fs::directory_iterator{ m_path })
			{
				if (e.is_directory()) { show_directory(e); }
			}

			for (auto const & e : fs::directory_iterator{ m_path })
			{
				if (e.is_regular_file()) { show_regular(e); }
			}

			ImGui::Separator();
			ImGui::PopStyleVar();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void show_directory(fs::directory_entry const & value)
		{
			// check
			if (value.path().empty() || !fs::is_directory(value)) { return; }

			ML_scoped_imgui_id((int32_t)util::hash(value.path().native()));

			// tree node
			ImGui::AlignTextToFramePadding();
			ImGui::PushStyleColor(ImGuiCol_Text, { .0f, 1.f, 1.f, 1.f });
			bool const node_open
			{
				ImGui::TreeNode("directory", "%s", value.path().stem().string().c_str())
			};
			ImGui::PopStyleColor();

			// context popup
			if (ImGui::BeginPopupContextItem("directory##context_popup"))
			{
				if (ImGui::Button("WIP")) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}

			// contents
			if (node_open)
			{
				// directories
				for (auto const & e : fs::directory_iterator{ value })
				{
					if (e.is_directory()) { show_directory(e); }
				}

				// regular files
				for (auto const & e : fs::directory_iterator{ value })
				{
					if (e.is_regular_file()) { show_regular(e); }
				}

				ImGui::TreePop();
			}
		}

		static void show_regular(fs::directory_entry const & value)
		{
			// check
			if (value.path().empty() || !fs::is_regular_file(value)) { return; }

			ML_scoped_imgui_id((int32_t)util::hash(value.path().native()));

			// tree node
			ImGui::AlignTextToFramePadding();
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.f, 1.f, 0.f, 1.f });
			ImGui::TreeNodeEx(
				"regular",
				ImGuiTreeNodeFlags_Leaf |
				ImGuiTreeNodeFlags_NoTreePushOnOpen |
				ImGuiTreeNodeFlags_Bullet,
				"%s",
				value.path().filename().string().c_str()
				);
			ImGui::PopStyleColor();

			// context popup
			if (ImGui::BeginPopupContextItem("regular_file##context_popup"))
			{
				if (ImGui::Button("WIP")) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_IMGUI_EXT_HPP_