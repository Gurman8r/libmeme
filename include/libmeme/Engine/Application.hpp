#ifndef _ML_APPLICATION_HPP_
#define _ML_APPLICATION_HPP_

// WIP

#include <libmeme/Engine/PluginManager.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>

namespace ml
{
	struct ML_ENGINE_API application final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		application(system_context * sys);

		~application();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto getbus()	noexcept -> event_bus		* { return s_instance->m_system->bus; }
		ML_NODISCARD static auto geted()	noexcept -> editor_context	* { return s_instance->m_system->ed; }
		ML_NODISCARD static auto getio()	noexcept -> io_context		* { return s_instance->m_system->io; }
		ML_NODISCARD static auto getmem()	noexcept -> memory			* { return s_instance->m_system->mem; }
		ML_NODISCARD static auto getmods()	noexcept -> plugin_manager	* { return &s_instance->m_plugins; }
		ML_NODISCARD static auto getscr()	noexcept -> script_context	* { return s_instance->m_system->scr; }
		ML_NODISCARD static auto getwin()	noexcept -> render_window	* { return s_instance->m_system->win; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static application *	s_instance	; // instance
		system_context * const	m_system	; // system pointer
		plugin_manager			m_plugins	; // plugins

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_APPLICATION_HPP_