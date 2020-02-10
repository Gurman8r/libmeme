#include <libmeme/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	decltype(event_system::s_listeners) event_system::s_listeners{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool event_system::add_listener(size_t type, event_listener * value)
	{
		return value && s_listeners[type].insert(value).second;
	}

	void event_system::fire_event(event const & value)
	{
		if (auto const v{ s_listeners.find(value.id()) })
		{
			for (auto const & it : (*v->second))
			{
				it->on_event(value);
			}
		}
	}

	void event_system::remove_listener(size_t type, event_listener * value)
	{
		if (auto v{ s_listeners.find(type) })
		{
			if (auto it{ (*v->second).find(value) }; it != (*v->second).end())
			{
				(*v->second).erase(it);
			}
		}
	}

	void event_system::remove_listener(event_listener * value)
	{
		s_listeners.for_each([value](auto, auto & v)
		{
			if (auto it{ v.find(value) }; it != v.end())
			{
				v.erase(it);
			}
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}