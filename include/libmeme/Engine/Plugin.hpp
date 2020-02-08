#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/EventSystem.hpp>

#define ML_PLUGIN_API ML_API_EXPORT

namespace ml
{
	struct plugin : trackable, event_listener, non_copyable
	{
		virtual ~plugin() noexcept = default;

		virtual void on_event(event const & value) override = 0;
	};
}

#endif // !_ML_PLUGIN_HPP_