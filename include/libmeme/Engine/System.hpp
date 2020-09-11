#ifndef _ML_SYSTEM_HPP_
#define _ML_SYSTEM_HPP_

// WIP

#include <libmeme/Core/Events.hpp>
#include <libmeme/Core/Performance.hpp>
#include <libmeme/Engine/API_Embed.hpp>
#include <libmeme/Engine/Content.hpp>
#include <libmeme/Engine/Editor.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// io context
	struct ML_NODISCARD io_context final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit io_context(int32_t argc, char ** argv, json const & conf) noexcept
			: argc{ argc }, argv{ argv }, conf{ json{ conf } }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t const	argc;
		char ** const	argv;
		json			conf;

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

	// system context
	struct ML_NODISCARD system_context final
	{
		event_bus		* const bus	; // bus
		editor_context	* const ed	; // editor
		io_context		* const io	; // io
		memory			* const mem	; // memory
		script_context	* const scr	; // scripts
		render_window	* const win	; // window
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// system object
	template <class Derived
	> struct system_object : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		explicit system_object(system_context * sys) noexcept : m_sys{ sys }
		{
		}

		virtual ~system_object() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		ML_NODISCARD auto get_bus		() const noexcept -> event_bus		* { return m_sys->bus	; }
		ML_NODISCARD auto get_editor	() const noexcept -> editor_context	* { return m_sys->ed	; }
		ML_NODISCARD auto get_io		() const noexcept -> io_context		* { return m_sys->io	; }
		ML_NODISCARD auto get_memory	() const noexcept -> memory			* { return m_sys->mem	; }
		ML_NODISCARD auto get_scripts	() const noexcept -> script_context	* { return m_sys->scr	; }
		ML_NODISCARD auto get_system	() const noexcept -> system_context	* { return m_sys		; }
		ML_NODISCARD auto get_window	() const noexcept -> render_window	* { return m_sys->win	; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		system_context * const m_sys;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SYSTEM_HPP_