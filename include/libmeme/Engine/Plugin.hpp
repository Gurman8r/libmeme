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

		explicit plugin(system_context * sys, void * user) noexcept
			: event_listener{ sys->bus }
			, m_system		{ sys }
			, m_user		{ user }
		{
			ML_assert("PLUGIN BUS MISMATCH" && (event_listener::m_event_bus == m_system->bus));
		}

		virtual ~plugin() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event const &) override = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto getbus()	const noexcept -> event_bus			* { return m_system->bus; }
		ML_NODISCARD auto geted()	const noexcept -> editor_context	* { return m_system->ed; }
		ML_NODISCARD auto getio()	const noexcept -> io_context		* { return m_system->io; }
		ML_NODISCARD auto getmem()	const noexcept -> memory			* { return m_system->mem; }
		ML_NODISCARD auto getscr()	const noexcept -> script_context	* { return m_system->scr; }
		ML_NODISCARD auto getuser()	const noexcept -> void				* { return m_user; }
		ML_NODISCARD auto getwin()	const noexcept -> render_window		* { return m_system->win; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		system_context * const	m_system	; // system pointer
		void * const			m_user	; // user pointer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_HPP_