#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <libmeme/Engine/System.hpp>

#ifndef ML_PLUGIN_API
#define ML_PLUGIN_API ML_API_EXPORT
#endif

namespace ml
{
	ML_decl_handle(plugin_id);

	struct ML_ENGINE_API plugin : trackable, non_copyable, event_listener
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

		ML_NODISCARD auto getbus()	const noexcept -> event_bus			* { return m_sys->bus; }
		ML_NODISCARD auto getio()	const noexcept -> io_context		* { return m_sys->io; }
		ML_NODISCARD auto getmem()	const noexcept -> memory			* { return m_sys->mem; }
		ML_NODISCARD auto getscr()	const noexcept -> script_context	* { return m_sys->scr; }
		ML_NODISCARD auto getwin()	const noexcept -> editor_window		* { return m_sys->win; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		system_context * const m_sys;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_HPP_