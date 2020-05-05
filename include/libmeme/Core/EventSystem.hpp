#ifndef _ML_EVENT_SYSTEM_HPP_
#define _ML_EVENT_SYSTEM_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Event.hpp>
#include <libmeme/Core/FlatMap.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct event_listener;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API event_system final
	{
		static ds::flat_map<hash_t, ds::flat_set<event_listener *>
		> g_event_system;
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ev
		> static bool add_listener(event_listener * value) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			return add_listener(hashof_v<Ev>, value);
		}

		template <class Ev, class ... Args
		> static void fire_event(Args && ... args) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			return fire_event(Ev{ ML_forward(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool add_listener(hash_t type, event_listener * value) noexcept;
		
		static void fire_event(event const & value) noexcept;

		static void remove_listener(hash_t type, event_listener * value) noexcept;
		
		static void remove_listener(event_listener * value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API event_listener
	{
		virtual ~event_listener() noexcept
		{
			event_system::remove_listener(this);
		}

		virtual void on_event(event const & ev) = 0;
	};


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline bool event_system::add_listener(hash_t type, event_listener * value) noexcept
	{
		return value && g_event_system.at(type).insert(value).second;
	}

	inline void event_system::fire_event(event const & value) noexcept
	{
		if (auto const listeners{ g_event_system.find(value.ID) })
		{
			for (event_listener * l : (*listeners->second))
			{
				l->on_event(value);
			}
		}
	}

	inline void event_system::remove_listener(hash_t type, event_listener * value) noexcept
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

	inline void event_system::remove_listener(event_listener * value) noexcept
	{
		if (!value) { return; }

		g_event_system.for_each([&](hash_t, auto & listeners)
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

#endif // !_ML_EVENT_SYSTEM_HPP_