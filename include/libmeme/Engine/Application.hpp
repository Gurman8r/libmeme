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

		ML_NODISCARD static auto get()			noexcept -> application		& { return *s_instance; }
		ML_NODISCARD static auto get_bus()		noexcept -> event_bus		& { return *s_instance->m_sys->bus; }
		ML_NODISCARD static auto get_config()	noexcept -> json			& { return *s_instance->m_sys->cfg; }
		ML_NODISCARD static auto get_io()		noexcept -> io_context		& { return *s_instance->m_sys->io; }
		ML_NODISCARD static auto get_memory()	noexcept -> memory			& { return *s_instance->m_sys->mem; }
		ML_NODISCARD static auto get_mods()		noexcept -> plugin_manager	& { return s_instance->m_plugins; }
		ML_NODISCARD static auto get_scripts()	noexcept -> script_context	& { return *s_instance->m_sys->scr; }
		ML_NODISCARD static auto get_time()		noexcept -> timer_context	& { return *s_instance->m_sys->time; }
		ML_NODISCARD static auto get_window()	noexcept -> gui_window		& { return *s_instance->m_sys->win; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static application *	s_instance	; // 
		system_context * const	m_sys		; // 
		plugin_manager			m_plugins	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_APPLICATION_HPP_