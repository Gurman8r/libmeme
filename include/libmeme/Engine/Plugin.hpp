#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/EventSystem.hpp>

#ifndef ML_PLUGIN_API
#define ML_PLUGIN_API ML_API_EXPORT
#endif

#ifndef ML_PLUGIN_MAIN
#define ML_PLUGIN_MAIN "ml_plugin_main"
#endif

namespace ml
{
	ML_decl_handle(plugin_handle);

	struct plugin_manager;

	struct plugin : non_copyable, trackable, event_listener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit plugin(plugin_manager * mgr) noexcept : m_parent{ mgr }
		{
		}

		virtual ~plugin() noexcept = default;

		virtual void on_event(event const &) override = 0;

		plugin_manager * const & get_manager() const & noexcept { return m_parent; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		plugin_manager * const m_parent;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_HPP_