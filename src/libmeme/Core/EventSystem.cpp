#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	decltype(EventSystem::m_listeners) EventSystem::m_listeners{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventListener * EventSystem::add_listener(int32_t type, EventListener * listener)
	{
		return ((type != Event::EV_INVALID) && listener)
			? (*m_listeners[type].insert(listener).first)
			: nullptr;
	}

	void EventSystem::fire_event(Event const & value)
	{
		if (auto const it{ m_listeners.find(value.id()) })
		{
			for (auto const & listener : (**it))
			{
				listener->onEvent(value);
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EventSystem::remove_listener(int32_t type, EventListener * listener)
	{
		if (auto vec{ m_listeners.find(type) })
		{
			for (auto it = (*vec)->begin(); it != (*vec)->end(); ++it)
			{
				if ((*it) == listener)
				{
					(*vec)->erase(it);
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