#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	decltype(EventSystem::m_listeners) EventSystem::m_listeners{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventListener * EventSystem::add_listener(int32_t type, EventListener * listener)
	{
		return (*m_listeners[type].insert(listener).first);
	}

	void EventSystem::fire_event(Event const & value)
	{
		for (auto const & listener : m_listeners[value.id()])
		{
			listener->onEvent(value);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EventSystem::remove_listener(int32_t type, EventListener * listener)
	{
		if (auto vec = m_listeners.find(type); vec != m_listeners.values().end())
		{
			for (auto it = vec->begin(); it != vec->end(); ++it)
			{
				if ((*it) == listener)
				{
					vec->erase(it);
					return;
				}
			}
		}
	}

	void EventSystem::remove_listener(EventListener * listener)
	{
		bool done{ false };
		while (!done)
		{
			done = true;
			m_listeners.for_each([&done, listener](auto, auto & vec)
			{
				for (auto it = vec.begin(); it != vec.end(); ++it)
				{
					if ((*it) == listener)
					{
						vec.erase(it);
						done = false;
						break;
					}
				}
			});

		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}