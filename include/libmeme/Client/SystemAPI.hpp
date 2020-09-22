#ifndef _ML_SYSTEM_API_HPP_
#define _ML_SYSTEM_API_HPP_

#include <libmeme/Core/Events.hpp>
#include <libmeme/Client/GuiManager.hpp>
#include <libmeme/Client/Python.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// I/O
namespace ml
{
	// io context
	struct ML_NODISCARD io_context final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = pmr::polymorphic_allocator<byte_t>;

		io_context(int32_t argc, char ** argv, json const & j, allocator_type alloc) noexcept
			: argc{ argc }, argv{ argv }, prefs{ json{ j } }, alloc{ alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t const	argc	; // 
		char ** const	argv	; // 
		json			prefs	; // 
		allocator_type	alloc	; // 

		fs::path const
			program_name{ argv[0] }							, // 
			program_path{ fs::current_path() }				, // 
			content_path{ prefs["path"].get<fs::path>() }	; // 

		timer const				main_timer	{}				; // 
		timer					loop_timer	{ false }		; // 
		duration				frame_time	{}				; // 
		uint64_t				frame_count	{}				; // 
		float_t					frame_rate	{}				; // 
		float_t					fps_accum	{}				; // 
		size_t					fps_index	{}				; // 
		pmr::vector<float_t>	fps_times	{ 120, alloc }	; // 

		vec2d	cursor						{}				; // 
		int32_t	mouse	[mouse_button_MAX]	{}				; // 
		int32_t	keys	[key_code_MAX]		{}				; // 

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
		memory			* const mem	; // memory manager
		io_context		* const io	; // io context
		event_bus		* const bus	; // event bus
		python_context	* const scr	; // interpreter
		render_window	* const win	; // render window
		gui_manager		* const gui	; // gui manager
	};

	// system object
	template <class Derived
	> struct system_object : trackable, non_copyable, event_listener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		explicit system_object(system_context * sys) : event_listener{ sys->bus }, m_sys{ sys }
		{
			ML_assert("BUS MISMATCH" && event_listener::m_bus == m_sys->bus);
		}

		virtual ~system_object() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event const &) override = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		ML_NODISCARD auto get_bus	() const noexcept -> event_bus		* { return m_sys->bus;	}
		ML_NODISCARD auto get_gui	() const noexcept -> gui_manager	* { return m_sys->gui;	}
		ML_NODISCARD auto get_io	() const noexcept -> io_context		* { return m_sys->io;	}
		ML_NODISCARD auto get_mem	() const noexcept -> memory			* { return m_sys->mem;	}
		ML_NODISCARD auto get_py	() const noexcept -> python_context	* { return m_sys->scr;	}
		ML_NODISCARD auto get_sys	() const noexcept -> system_context	* { return m_sys;		}
		ML_NODISCARD auto get_win	() const noexcept -> render_window	* { return m_sys->win;	}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		system_context * const m_sys;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_SYSTEM_API_HPP_