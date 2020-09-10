#ifndef _ML_SYSTEM_HPP_
#define _ML_SYSTEM_HPP_

// WIP

#include <libmeme/Core/Events.hpp>
#include <libmeme/Core/Performance.hpp>
#include <libmeme/Engine/API_Embed.hpp>
#include <libmeme/Engine/Editor.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD io_context final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t const	argc;
		char ** const	argv;
		json			conf{ R"({"path":""})"_json };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		fs::path const
			program_name{ argv[0] },
			program_path{ fs::current_path() },
			content_path{ conf.at("path").get<fs::path>() };

		ML_NODISCARD fs::path path2(fs::path const & path) const noexcept
		{
			return content_path.native() + path.native();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using fps_t = pmr::vector<float_t>;

		timer const	main_timer	{};
		timer		loop_timer	{ false };
		duration	delta_time	{};
		uint64_t	frame_count	{};
		float_t		frame_rate	{};
		float_t		fps_accum	{};
		size_t		fps_index	{};
		fps_t		fps_times	{ 120, fps_t::allocator_type{} };

		void begin_step() noexcept
		{
			loop_timer.restart();
			auto const dt{ (float_t)delta_time.count() };
			fps_accum += dt - fps_times[fps_index];
			fps_times[fps_index] = dt;
			fps_index = (fps_index + 1) % fps_times.size();
			frame_rate = (0.f < fps_accum) ? 1.f / (fps_accum / (float_t)fps_times.size()) : FLT_MAX;
		}

		void end_step() noexcept
		{
			performance::refresh_samples();
			delta_time = loop_timer.elapsed();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD system_context final
	{
		event_bus		* const bus	; // event bus
		editor_context	* const ed	; // editor context
		io_context		* const io	; // io context
		memory			* const mem	; // memory manager
		script_context	* const scr	; // script context
		render_window	* const win	; // render window
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> struct system_object : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit system_object(system_context * sys) noexcept : m_sys{ sys }
		{
			ML_assert(m_sys);
		}

		virtual ~system_object() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto getbus()	const noexcept -> event_bus			* { return m_sys->bus	; }
		ML_NODISCARD auto geted()	const noexcept -> editor_context	* { return m_sys->ed	; }
		ML_NODISCARD auto getio()	const noexcept -> io_context		* { return m_sys->io	; }
		ML_NODISCARD auto getmem()	const noexcept -> memory			* { return m_sys->mem	; }
		ML_NODISCARD auto getsys()	const noexcept -> system_context	* { return m_sys		; }
		ML_NODISCARD auto getscr()	const noexcept -> script_context	* { return m_sys->scr	; }
		ML_NODISCARD auto getwin()	const noexcept -> render_window		* { return m_sys->win	; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		system_context * const m_sys;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SYSTEM_HPP_