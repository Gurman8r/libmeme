#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/EventSystem.hpp>

#define ML_PLUGIN_API ML_API_EXPORT

namespace ml
{
	struct plugin : trackable, non_copyable, event_listener
	{
		virtual ~plugin() noexcept = default;

		virtual void on_event(event const & value) override = 0;
	};
}

#endif // !_ML_PLUGIN_HPP_