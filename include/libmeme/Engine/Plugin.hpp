#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <libmeme/Core/EventBus.hpp>
#include <libmeme/Core/Performance.hpp>
#include <libmeme/Engine/GuiManager.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>

#ifndef ML_PLUGIN_API
#define ML_PLUGIN_API ML_API_EXPORT
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct script_context;

	struct ML_NODISCARD file_context final
	{
		fs::path const
			program_name,
			program_path,
			content_path;

		ML_NODISCARD fs::path path2(fs::path const & path) const noexcept
		{
			return content_path.native() + path.native();
		}
	};

	struct ML_NODISCARD timer_context final
	{
		using fps_times_t = pmr::vector<float_t>;

		timer const	main		{};
		timer		loop		{ false };
		duration	delta_time	{};
		uint64_t	frame_count	{};
		float_t		frame_rate	{};
		float_t		fps_accum	{};
		size_t		fps_index	{};
		fps_times_t	fps_times	{ 120, fps_times_t::allocator_type{} };

		void begin_step() noexcept
		{
			loop.restart();
			frame_rate = std::invoke([&, dt = (float_t)delta_time.count()]() noexcept
			{
				fps_accum += dt - fps_times[fps_index];
				fps_times[fps_index] = dt;
				fps_index = (fps_index + 1) % fps_times.size();
				return (0.f < fps_accum) ? 1.f / (fps_accum / (float_t)fps_times.size()) : FLT_MAX;
			});
		}

		void end_step() noexcept
		{
			delta_time = loop.elapsed();
		}
	};

	struct ML_NODISCARD system_context final
	{
		event_bus		* const bus	; // bus
		json			* const cfg	; // config
		file_context	* const fs	; // filesystem
		gui_manager		* const gui	; // gui
		memory			* const mem	; // memory
		script_context	* const scr	; // scripts
		timer_context	* const time; // timers
		render_window	* const win	; // window
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_handle(plugin_id);

	struct ML_PLUGIN_API plugin : non_copyable, trackable, event_listener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit plugin(system_context * sys) noexcept : event_listener{ sys->bus }, m_sys{ sys }
		{
		}

		virtual ~plugin() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event const &) override = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto getbus()	const noexcept -> event_bus			* { return m_sys->bus; }
		ML_NODISCARD auto getcfg()	const noexcept -> json				* { return m_sys->cfg; }
		ML_NODISCARD auto getfs()	const noexcept -> file_context		* { return m_sys->fs; }
		ML_NODISCARD auto getgui()	const noexcept -> gui_manager		* { return m_sys->gui; }
		ML_NODISCARD auto getmem()	const noexcept -> memory			* { return m_sys->mem; }
		ML_NODISCARD auto getscr()	const noexcept -> script_context	* { return m_sys->scr; }
		ML_NODISCARD auto gettime()	const noexcept -> timer_context		* { return m_sys->time; }
		ML_NODISCARD auto getwin()	const noexcept -> render_window		* { return m_sys->win; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		system_context * const m_sys;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_HPP_