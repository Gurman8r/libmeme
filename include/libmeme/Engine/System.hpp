#ifndef _ML_SYSTEM_HPP_
#define _ML_SYSTEM_HPP_

// WIP

#include <libmeme/Core/EventBus.hpp>
#include <libmeme/Core/Performance.hpp>
#include <libmeme/Engine/API_Embed.hpp>
#include <libmeme/Engine/GuiWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD io_context final
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
			performance::refresh_samples();
			delta_time = loop.elapsed();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD system_context final
	{
		event_bus		* const bus	; // bus
		json			* const cfg	; // config
		io_context		* const io	; // io
		memory			* const mem	; // memory
		script_context	* const scr	; // scripts
		timer_context	* const time; // timers
		gui_window		* const win	; // window
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SYSTEM_HPP_