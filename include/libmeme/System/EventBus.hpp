#ifndef _ML_EVENT_BUS_HPP_
#define _ML_EVENT_BUS_HPP_

#include <libmeme/System/Export.hpp>
#include <libmeme/System/Event.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/Singleton.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// base event listener
	struct ML_SYSTEM_API event_listener
	{
		virtual ~event_listener() noexcept;

		virtual void on_event(event const &) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event bus singleton
	class ML_SYSTEM_API event_bus final : public singleton<event_bus>
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool add_listener(hash_t type, event_listener * value) noexcept
		{
			static auto & self{ get_instance() };
			
			// insert listener into category
			return value && self.m_listeners[type].insert(value).second;
		}
		
		template <class Ev
		> static bool add_listener(event_listener * value) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			return add_listener(hashof_v<Ev>, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void fire_event(event const & ev) noexcept
		{
			static auto & self{ get_instance() };

			// get category
			if (auto const c{ self.m_listeners.find(ev.ID) })
			{
				// for each listener
				for (auto const & l : (*c->second))
				{
					// handle event
					l->on_event(ev);
				}
			}
		}

		template <class Ev, class ... Args
		> static void fire_event(Args && ... args) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			return fire_event(Ev{ ML_forward(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void remove_listener(hash_t type, event_listener * value) noexcept
		{
			static auto & self{ get_instance() };

			if (!value) { return; }

			// get category
			if (auto const c{ self.m_listeners.find(type) })
			{
				// get listener
				if (auto const l{ c->second->find(value) }; l != c->second->end())
				{
					// remove listener
					c->second->erase(l);
				}
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void remove_listener(event_listener * value) noexcept
		{
			static auto & self{ get_instance() };

			if (!value) { return; }

			// for each category
			self.m_listeners.for_each([&](auto, auto & c) noexcept
			{
				// get listener
				if (auto const l{ c.find(value) }; l != c.end())
				{
					// remove listener
					c.erase(l);
				}
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton;

		~event_bus() noexcept;

		ds::map<hash_t, ds::set<event_listener *>> m_listeners{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline event_listener::~event_listener() noexcept
	{
		event_bus::remove_listener(this); // remove listener from all events
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_