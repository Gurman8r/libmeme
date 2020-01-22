#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Event.hpp>
#include <libmeme/Core/EventListener.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ds::flat_map<
		size_t, ds::flat_set<event_listener *>
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
			if (auto it{ (*v->second).find(listener) }; it != (*v->second).end())
			{
				(*v->second).erase(it);
			}
		}
	}

	void event_system::remove_listener(event_listener * listener)
	{
		m_listeners.for_each([listener](auto, auto & v)
		{
			if (auto it{ v.find(listener) }; it != v.end())
			{
				v.erase(it);
			}
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}