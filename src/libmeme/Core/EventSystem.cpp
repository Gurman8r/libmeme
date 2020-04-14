#include <libmeme/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ds::flat_map<hash_t, ds::flat_set<event_listener *>
	> g_event_system{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool event_system::add_listener(hash_t type, event_listener * value)
	{
		return value && g_event_system.at(type).insert(value).second;
	}

	void event_system::fire_event(event const & value)
	{
		if (auto const listeners{ g_event_system.find(value.ID) })
		{
			for (event_listener * l : (*listeners->second))
			{
				l->on_event(value);
			}
		}
	}

	void event_system::remove_listener(hash_t type, event_listener * value)
	{
		if (!value) { return; }

		if (auto const listeners{ g_event_system.find(type) })
		{
			if (auto const it{ listeners->second->find(value) }
			; it != listeners->second->end())
			{
				(*listeners->second).erase(it);
			}
		}
	}

	void event_system::remove_listener(event_listener * value)
	{
		if (!value) { return; }

		g_event_system.for_each([value](hash_t, auto & listeners)
		{
			if (auto const it{ listeners.find(value) }
			; it != listeners.end())
			{
				listeners.erase(it);
			}
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}