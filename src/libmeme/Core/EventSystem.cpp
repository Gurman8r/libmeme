#include <libmeme/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ds::flat_map<hash_t, ds::flat_set<event_listener *>
	> g_event_system{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool event_system::add_listener(hash_t type, event_listener * value)
	{
		return value && g_event_system[type].insert(value).second;
	}

	void event_system::fire_event(event const & value)
	{
		if (auto const it{ g_event_system.find(value.id()) })
		{
			for (auto const & listener : (*it->second))
			{
				listener->on_event(value);
			}
		}
	}

	void event_system::remove_listener(hash_t type, event_listener * value)
	{
		if (auto it{ g_event_system.find(type) })
		{
			if (auto listener{ it->second->find(value) }; listener != it->second->end())
			{
				it->second->erase(listener);
			}
		}
	}

	void event_system::remove_listener(event_listener * value)
	{
		g_event_system.for_each([value](auto, auto & it)
		{
			if (auto listener{ it.find(value) }; listener != it.end())
			{
				it.erase(listener);
			}
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}