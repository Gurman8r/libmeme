#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/System/Memory.hpp>
#include <libmeme/System/EventBus.hpp>

#ifndef ML_PLUGIN_API
#define ML_PLUGIN_API	ML_API_EXPORT
#endif

#ifndef ML_PLUGIN_MAIN
#define ML_PLUGIN_MAIN	"ml_plugin_main"
#endif

namespace ml
{
	struct application;

	ML_decl_handle(plugin_handle);

	struct plugin : non_copyable, trackable, event_listener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit plugin(application * app) noexcept : m_app{ app } { ML_assert(app); }

		virtual ~plugin() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event const &) override = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto app() & noexcept -> application & { return *m_app; }

		auto app() const & noexcept -> application const & { return *m_app; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		application * const m_app;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_HPP_