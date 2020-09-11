#ifndef _ML_SYSTEM_HPP_
#define _ML_SYSTEM_HPP_

#include <libmeme/Core/Events.hpp>
#include <libmeme/Engine/API_Embed.hpp>
#include <libmeme/Engine/Editor.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// IO
namespace ml
{
	// io context
	struct ML_NODISCARD io_context final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = pmr::polymorphic_allocator<byte_t>;

		io_context(int32_t argc, char ** argv, allocator_type alloc, json const & j)
			: argc{ argc }, argv{ argv }, alloc{ alloc }, conf{ json{ j } }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t const	argc	; // 
		char ** const	argv	; // 
		allocator_type	alloc	; // 
		json			conf	; // 

		fs::path const
			program_name{ argv[0] }							, // 
			program_path{ fs::current_path() }				, // 
			content_path{ conf["path"].get<fs::path>() }	; // 

		timer const				main_timer	{}				; // 
		timer					loop_timer	{ false }		; // 
		duration				delta_time	{}				; // 
		uint64_t				frame_count	{}				; // 
		float_t					frame_rate	{}				; // 
		float_t					fps_accum	{}				; // 
		size_t					fps_index	{}				; // 
		pmr::vector<float_t>	fps_times	{ 120, alloc }	; // 

		int32_t keyboard	[key_code_MAX]		{}			; // 
		int32_t mouse		[mouse_button_MAX]	{}			; // 
		vec2d	cursor_pos						{}			; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD fs::path path2(fs::path const & path) const noexcept
		{
			return content_path.native() + path.native();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// SYSTEM
namespace ml
{
	// system context
	struct ML_NODISCARD system_context final
	{
		event_bus		* const bus	; // bus
		editor_context	* const gui	; // editor
		io_context		* const io	; // io
		memory			* const mem	; // memory
		script_context	* const scr	; // scripts
		render_window	* const win	; // window

		constexpr operator bool() const noexcept
		{
			return bus && gui && io && mem && scr && win;
		}
	};

	// system object
	template <class Derived
	> struct system_object : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		explicit system_object(system_context * sys) noexcept : m_sys{ sys }
		{
			ML_assert("BAD SYSTEM CONTEXT" && sys && *sys);
		}

		virtual ~system_object() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		ML_NODISCARD auto get_bus		() const noexcept -> event_bus		* { return m_sys->bus	; }
		ML_NODISCARD auto get_gui		() const noexcept -> editor_context	* { return m_sys->gui	; }
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_SYSTEM_HPP_