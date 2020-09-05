#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <libmeme/Engine/System.hpp>

#ifndef ML_PLUGIN_API
#define ML_PLUGIN_API ML_API_EXPORT
#endif

namespace ml
{
	ML_decl_handle(plugin_id);

	struct ML_ENGINE_API plugin : non_copyable, trackable, event_listener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit plugin(system_context * sys) noexcept : event_listener{ sys->bus }, m_sys{ sys }
		{
		}

		virtual ~plugin() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event const &) override = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_bus()		const noexcept -> event_bus			& { return *m_sys->bus; }
		ML_NODISCARD auto get_config()	const noexcept -> json				& { return *m_sys->cfg; }
		ML_NODISCARD auto get_io()		const noexcept -> io_context		& { return *m_sys->io; }
		ML_NODISCARD auto get_memory()	const noexcept -> memory			& { return *m_sys->mem; }
		ML_NODISCARD auto get_scripts()	const noexcept -> script_context	& { return *m_sys->scr; }
		ML_NODISCARD auto get_time()	const noexcept -> timer_context		& { return *m_sys->time; }
		ML_NODISCARD auto get_window()	const noexcept -> gui_window		& { return *m_sys->win; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		system_context * const m_sys;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_HPP_