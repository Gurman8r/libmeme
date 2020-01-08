#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventSystem::EventSystem() : m_listeners{} {}

	EventSystem::~EventSystem() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventListener * EventSystem::addListener(int32_t type, EventListener * listener)
	{
		return listener
			? m_listeners.insert(std::make_pair(type, listener))->second
			: nullptr;
	}

	void EventSystem::fireEvent(Event const & value)
	{
		auto found{ m_listeners.equal_range(value.id()) };
		for (auto it = found.first; it != found.second; it++)
		{
			if (it->second) { it->second->onEvent(value); }
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool EventSystem::removeListener(int32_t type, EventListener * listener)
	{
		auto found{ m_listeners.equal_range(type) };
		for (auto it = found.first; it != found.second; it++)
		{
			if (it->second == listener)
			{
				m_listeners.erase(it);
				return true;
			}
		}
		return false;
	}

	bool EventSystem::removeListenerFromAllEvents(EventListener * listener)
	{
		bool done{ false };
		while (!done)
		{
			done = true;
			for (auto it = m_listeners.begin(); it != m_listeners.end(); it++)
			{
				if (it->second == listener)
				{
					m_listeners.erase(it);
					done = false;
					break;
				}
			}
		}
		return done;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}