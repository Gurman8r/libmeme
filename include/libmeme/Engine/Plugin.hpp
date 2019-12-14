#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/EventListener.hpp>

#define ML_PLUGIN_API ML_API_EXPORT

namespace ml
{
	struct Plugin : public Trackable, public EventListener
	{
		Plugin() = default;

		virtual ~Plugin() {}

		virtual void onEvent(Event const & value) override = 0;
	};
}

#endif // !_ML_PLUGIN_HPP_