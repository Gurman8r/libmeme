#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	decltype(EventSystem::m_listeners) EventSystem::m_listeners{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool EventSystem::add_listener(int32_t type, EventListener * listener)
	{
		return (listener && (type > Event::EV_INVALID))
			&& (m_listeners[type].insert(listener).second);
	}

	void EventSystem::fire_event(Event const & value)
	{
		if (auto const it{ m_listeners.find(value.id()) })
		{
			for (auto const & listener : (*it->second))
			{
				listener->onEvent(value);
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EventSystem::remove_listener(int32_t type, EventListener * listener)
	{
		if (auto v{ m_listeners.find(type) })
		{
			for (auto it = (*v->second).begin(); it != (*v->second).end(); ++it)
			{
				if ((*it) == listener)
				{
					(*v->second).erase(it);
					
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
			m_listeners.for_each([&done, listener](auto, auto & v)
			{
				for (auto it = v.begin(); it != v.end(); ++it)
				{
					if ((*it) == listener)
					{
						v.erase(it);
						done = false;
						break;
					}
				}
			});
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}