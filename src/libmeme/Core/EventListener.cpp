#include <libmeme/Core/EventListener.hpp>
#include <libmeme/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	event_listener::~event_listener()
	{
		event_system::remove_listener(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}