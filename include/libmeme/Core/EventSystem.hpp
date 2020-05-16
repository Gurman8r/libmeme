#ifndef _ML_EVENT_SYSTEM_HPP_
#define _ML_EVENT_SYSTEM_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Event.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/Singleton.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// global event listener
	struct ML_CORE_API event_listener
	{
		virtual ~event_listener() noexcept;

		virtual void on_event(event const &) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// global event system
	class ML_CORE_API event_system final : public singleton<event_system>
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool add_listener(hash_t id, event_listener * value) noexcept
		{
			static auto & inst{ get_instance() };
			
			// insert listener into category
			return value && inst.m_listeners[id].insert(value).second;
		}
		
		template <class Ev
		> static bool add_listener(event_listener * value) noexcept
		{
			// add listener by type
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");
			return add_listener(hashof_v<Ev>, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void fire_event(event const & ev) noexcept
		{
			static auto & inst{ get_instance() };

			// get category
			if (auto const v{ inst.m_listeners.find(ev.ID) })
			{
				// for each listener
				for (auto const & l : (*v->second))
				{
					// handle event
					l->on_event(ev);
				}
			}
		}

		template <class Ev, class ... Args
		> static void fire_event(Args && ... args) noexcept
		{
			// fire event by type
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");
			return fire_event(Ev{ ML_forward(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void remove_listener(hash_t id, event_listener * value) noexcept
		{
			static auto & inst{ get_instance() };

			if (!value) { return; }

			// get category
			if (auto const v{ inst.m_listeners.find(id) })
			{
				// get listener
				if (auto const l{ v->second->find(value) }; l != v->second->end())
				{
					// remove listener
					v->second->erase(l);
				}
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void remove_listener(event_listener * value) noexcept
		{
			static auto & inst{ get_instance() };

			if (!value) { return; }

			// for each category
			inst.m_listeners.for_each([&](auto, auto & v) noexcept
			{
				// get listener
				if (auto const l{ v.find(value) }; l != v.end())
				{
					// remove listener
					v.erase(l);
				}
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<event_system>;

		event_system() noexcept = default;

		~event_system() noexcept;

		ds::map<hash_t, ds::set<event_listener *>> m_listeners{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline event_listener::~event_listener() noexcept
	{
		event_system::remove_listener(this); // remove listener from all events
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_