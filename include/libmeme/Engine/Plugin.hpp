#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <libmeme/Core/EventBus.hpp>
#include <libmeme/Core/Performance.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Engine/GuiManager.hpp>

#ifndef ML_PLUGIN_API
#define ML_PLUGIN_API	ML_API_EXPORT
#endif

namespace ml
{
	ML_decl_handle(plugin_id);

	struct file_context final
	{
		fs::path const
			program_name,
			program_path,
			content_path;

		fs::path path(fs::path const & path = {}) const noexcept
		{
			return program_path.native() + path.native();
		}

		fs::path path2(fs::path const & path = {}) const noexcept
		{
			return content_path.native() + path.native();
		}
	};

	struct timer_context final
	{
		using fps_times_t = pmr::vector<float_t>;

		timer		main_timer	{};
		timer		loop_timer	{ false };
		duration	delta_time	{};
		uint64_t	frame_count	{};
		float_t		frame_rate	{};
		float_t		fps_accum	{};
		size_t		fps_index	{};
		fps_times_t	fps_times	{ 120, fps_times_t::allocator_type{} };
	};

	struct system_context final
	{
		event_bus		* const bus	; // bus
		json			* const cfg	; // config
		file_context	* const fs	; // files
		gui_manager		* const gui	; // gui
		memory			* const mem	; // memory
		timer_context	* const time; // timers
		render_window	* const win	; // window
	};

	struct plugin : non_copyable, trackable, event_listener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit plugin(system_context * sys) noexcept : event_listener{ sys->bus }, m_sys{ sys }
		{
			ML_assert("BUS MISMATCH" && (event_listener::m_event_bus == m_sys->bus));
		}

		virtual ~plugin() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event const &) override = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto getbus	() const noexcept	-> event_bus		& { return *m_sys->bus	; }
		auto getcfg	() const noexcept	-> json				& { return *m_sys->cfg	; }
		auto getfs	() const noexcept	-> file_context		& { return *m_sys->fs	; }
		auto getgui	() const noexcept	-> gui_manager		& { return *m_sys->gui	; }
		auto getmem	() const noexcept	-> memory			& { return *m_sys->mem	; }
		auto gettime() const noexcept	-> timer_context	& { return *m_sys->time	; }
		auto getwin	() const noexcept	-> render_window	& { return *m_sys->win	; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		system_context * const m_sys;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_HPP_