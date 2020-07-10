#ifndef _ML_IMGUI_EXT_HPP_
#define _ML_IMGUI_EXT_HPP_

#include <libmeme/Engine/ImGui.hpp>
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

		using buffer_t	= typename pmr::vector<float_t>;
		using overtxt_t	= typename ds::array<char, 32>;
		using get_str_t = typename std::function<cstring()>;
		using get_fn_t	= typename std::function<float_t()>;

		buffer_t	buffer		{};
		int32_t		mode		{};
		cstring		label		{};
		get_str_t	get_fmt		{ []() noexcept { return "%f"; } };
		get_fn_t	get_value	{ []() noexcept { return 0.f; } };
		vec2		size		{};
		vec2		scale		{ FLT_MAX, FLT_MAX };
		int32_t		offset		{};
		char		overtxt[32]	{};
		bool		animate		{ true };
		
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
			ML_assert(get_value);
			update(std::invoke(get_value));
		}

		template <class Delta = float_t
		> void update(Delta const v) noexcept
		{
			static_assert(std::is_floating_point_v<Delta>);
			if (!animate || buffer.empty()) { return; }
			std::sprintf(overtxt, get_fmt(), v);
			buffer[offset] = static_cast<float_t>(v);
			offset = (offset + 1) % buffer.size();
		}

		void render() const noexcept
		{
			ML_ImGui_ScopeID(this);

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
					, overtxt
					, scale[0], scale[1], { width, size[1] }
					, sizeof(float_t)
				);
			case histogram:
				return ImGui::PlotHistogram(label
					, buffer.data(), (int32_t)buffer.size(), offset
					, overtxt
					, scale[0], scale[1], { width, size[1] }
					, sizeof(float_t)
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

		pmr::vector<plot> plots{}; float_t ref_time{};

		plot_controller(std::initializer_list<plot> init) noexcept
			: plots{ init.begin(), init.end() }, ref_time{}
		{
		}

		template <class Fn> auto for_each(Fn && fn) noexcept
		{
			return std::for_each(plots.begin(), plots.end(), ML_forward(fn));
		}

		template <class Delta = float_t
		> void update(Delta const tt, Delta const dt = (Delta)(1.f / 60.f)) noexcept
		{
			if (ref_time == 0.f)
			{
				return (void)(ref_time = static_cast<float_t>(tt));
			}
			while (ref_time < static_cast<float_t>(tt))
			{
				this->for_each([&](auto & p) { p.update(); });

				ref_time += static_cast<float_t>(dt);
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
			if (!open || (fn == (Fn)nullptr)) { return; }
			ML_ImGui_ScopeID(this);
			ML_defer(&){ ImGui::End(); };
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
		void *	tex_addr	{ nullptr };
		vec2i	tex_size	{};
		vec2	img_size	{};
		float_t	reg_zoom	{ 4.f };
		float_t	reg_size	{ 32.f };

		void render() noexcept
		{
			if (!tex_addr) { return; }

			auto const & io		{ ImGui::GetIO() };
			auto const reg_avail{ ImGui::GetContentRegionAvail() };
			auto const scr_pos	{ ImGui::GetCursorScreenPos() };
			auto const scr_size	{ util::scale_to_fit((vec2)tex_size,
			{
				img_size[0] == 0.f ? reg_avail[0] : img_size[0],
				img_size[1] == 0.f ? reg_avail[1] : img_size[1]
			}) };

			ImGui::Image(
				tex_addr,
				scr_size,
				{ 0, 1 },
				{ 1, 0 },
				colors::white,
				colors::gray
			);

			// zoom tooltip region
			if ((0.f < reg_size) && (0.f < reg_zoom)) tooltip_ex([&]() noexcept
			{
				float_t rx{ io.MousePos[0] - scr_pos[0] - reg_size * .5f };
				if (rx < 0.f) { rx = 0.f; }
				else if (rx > scr_size[0] - reg_size) { rx = (scr_size[0] - reg_size); }

				float_t ry{ io.MousePos[1] - scr_pos[1] - reg_size * .5f };
				if (ry < 0.f) { ry = 0.f; }
				else if (ry > scr_size[1] - reg_size) { ry = (scr_size[1] - reg_size); }

				ImGui::Text("%u: %dx%d (%.0fx%.0f)",
					(uint32_t)(intptr_t)tex_addr,
					tex_size[0], tex_size[1],
					scr_size[0], scr_size[1]
				);
				ImGui::Text("Min: (%.2f, %.2f)", rx, ry);
				ImGui::Text("Max: (%.2f, %.2f)", rx + reg_size, ry + reg_size);
				ImGui::Image(
					tex_addr,
					{ reg_size * reg_zoom, reg_size * reg_zoom },
					{
						(rx / scr_size[0]),
						(1.f - (ry / scr_size[1]))
					},
					{
						((rx + reg_size) / scr_size[0]),
						(1.f - ((ry + reg_size) / scr_size[1]))
					},
					colors::white,
					colors::gray
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

		using self_type			= typename _ML_GUI console;
		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using command_name		= typename pmr::string;
		using command_args		= typename pmr::vector<pmr::string>;
		using command_clbk		= typename std::function<void(command_args)>;
		using command_info		= typename pmr::vector<pmr::string>;

		struct ML_NODISCARD command_data final
		{
			command_name name; // name
			command_clbk clbk; // callback
			command_info info; // information

			bool operator==(command_data const & other) const noexcept
			{
				return (this == std::addressof(other)) || (name == other.name);
			}

			bool operator<(command_data const & other) const noexcept
			{
				return (this != std::addressof(other)) && (name < other.name);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		console(allocator_type alloc = {}) noexcept
			: commands	{ alloc }
			, history	{ alloc }
			, lines		{ alloc }
		{
		}

		pmr::vector<command_data>	commands		{}		; // commands
		pmr::vector<pmr::string>	history			{}		; // history buffer
		pmr::vector<pmr::string>	lines			{}		; // text buffer

		ds::array<char, 256>		input			{}		; // input buffer
		int32_t						history_pos		{ -1 }	; // history index
		ImGuiTextFilter				filter			{}		; // text filter
		bool						auto_scroll		{ true }; // auto-scroll
		bool						scroll_to_bot	{}		; // scroll-to-bottom
		cstring						command_lock	{}		; // forced command

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & clear() noexcept
		{
			lines.clear();
			return (*this);
		}

		self_type & write(char const value) noexcept
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

		self_type & print(pmr::string const & value) noexcept
		{
			for (char const & c : value)
			{
				this->write(c);
			}
			return (*this);
		}

		self_type & printl(pmr::string const & value = {}) noexcept
		{
			return this->print(value).write('\n');
		}

		self_type & printf(cstring fmt, ...) noexcept
		{
			ds::array<char, 1024> buf{};
			va_list args{};
			va_start(args, fmt);
			std::vsnprintf(buf.data(), buf.size(), fmt, args);
			buf.back() = 0;
			va_end(args);
			return this->print(buf.data());
		}

		self_type & execute(cstring value) noexcept
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
				if (auto const it{ std::find_if(commands.begin(), commands.end(), [&
				](auto & cmd) noexcept
				{
					return cmd.name == (command_lock ? command_lock : toks.front());
				}) }
				; it != commands.end())
				{
					if (!command_lock) { toks.erase(toks.begin()); }

					std::invoke(it->clbk, std::move(toks));
				}
				else
				{
					this->printf("unknown command: \'%s\'\n", toks.front().c_str());
				}
			}

			scroll_to_bot = true;
			return (*this);
		}

		self_type & render()
		{
			ML_ImGui_ScopeID(this);

			// HEADER
			filter.Draw("filter", 180); ImGui::SameLine();
			ImGui::Checkbox("auto-scroll", &auto_scroll); ImGui::SameLine();
			if (ImGui::Button("clear")) clear(); ImGui::SameLine();
			ImGui::Text("lock: %s", command_lock ? command_lock : "-"); //ImGui::SameLine();
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
				if (std::strstr(str, ML_IMPL_DEBUG_MSG_I))
				{
					// [ info ] (green)
					ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 1.0f, 0.0f, 1.0f }); pop_color = true;
				}
				else if (std::strstr(str, ML_IMPL_DEBUG_MSG_E))
				{
					// [ error ] (red)
					ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.2f, 0.4f, 1.0f }); pop_color = true;
				}
				else if (std::strstr(str, ML_IMPL_DEBUG_MSG_W))
				{
					// [ warning ] (yellow)
					ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.75f, 0.0f, 1.0f }); pop_color = true;
				}
				else if (0 == std::strncmp(str, "# ", 2))
				{
					// # (orange)
					ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.8f, 0.6f, 1.0f }); pop_color = true;
				}
				ImGui::TextUnformatted(str);
				if (pop_color) { ImGui::PopStyleColor(); }
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
					if (std::strncmp(commands[i].name.c_str(), word_start, (size_t)(word_end - word_start)) == 0)
						candidates.push_back(commands[i].name.c_str());

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

			ML_ImGui_ScopeID(this);

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

			ML_ImGui_ScopeID((int32_t)util::hash(value.path().native()));

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

			ML_ImGui_ScopeID((int32_t)util::hash(value.path().native()));

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