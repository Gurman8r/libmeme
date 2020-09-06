#ifndef _ML_APPLICATION_HPP_
#define _ML_APPLICATION_HPP_

// WIP

#include <libmeme/Engine/PluginManager.hpp>

namespace ml
{
	struct ML_ENGINE_API application final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		application(system_context * sys);

		~application();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto get()		noexcept -> application		* { return s_instance; }
		ML_NODISCARD static auto getbus()	noexcept -> event_bus		* { return s_instance->m_sys->bus; }
		ML_NODISCARD static auto getio()	noexcept -> io_context		* { return s_instance->m_sys->io; }
		ML_NODISCARD static auto getmem()	noexcept -> memory			* { return s_instance->m_sys->mem; }
		ML_NODISCARD static auto getmods()	noexcept -> plugin_manager	* { return &s_instance->m_plugins; }
		ML_NODISCARD static auto getscr()	noexcept -> script_context	* { return s_instance->m_sys->scr; }
		ML_NODISCARD static auto getwin()	noexcept -> editor_window	* { return s_instance->m_sys->win; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static application *	s_instance	; // 
		system_context * const	m_sys		; // 
		plugin_manager			m_plugins	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_APPLICATION_HPP_