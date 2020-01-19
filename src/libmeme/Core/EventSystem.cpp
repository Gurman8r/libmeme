#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ds::flat_map<
		size_t,
		ds::flat_set<event_listener *>
	> m_listeners{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool event_system::add_listener(size_t type, event_listener * listener)
	{
		return listener && (m_listeners[type].insert(listener).second);
	}

	void event_system::fire_event(event const & value)
	{
		if (auto const it{ m_listeners.find(value.id()) })
		{
			for (auto const & listener : (*it->second))
			{
				listener->on_event(value);
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void event_system::remove_listener(size_t type, event_listener * listener)
	{
		if (auto v{ m_listeners.find(type) })
		{
			for (auto it = (*v->second).begin(); it != (*v->second).end(); ++it)
			{
				if (*it == listener)
				{
					(*v->second).erase(it);
					
					return;
				}
			}
		}
	}

	void event_system::remove_listener(event_listener * listener)
	{
		bool done{ false };
		while (!done)
		{
			done = true;
			m_listeners.for_each([&done, listener](auto, auto & v)
			{
				for (auto it = v.begin(); it != v.end(); ++it)
				{
					if (*it == listener)
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