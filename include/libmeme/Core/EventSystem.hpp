#ifndef _ML_EVENT_SYSTEM_HPP_
#define _ML_EVENT_SYSTEM_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Event.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/Singleton.hpp>

// event_system singleton
#define ML_event_system \
	_ML event_system::get_instance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event listener
	struct ML_CORE_API event_listener
	{
		virtual ~event_listener() noexcept;

		virtual void on_event(event const & ev) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event system
	struct ML_CORE_API event_system final : singleton<event_system>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool add_listener(hash_t id, event_listener * value) noexcept
		{
			static auto & inst{ get_instance() };
			
			if (!value) { return false; }

			// insert listener into category
			return inst.m_listeners.at(id).insert(value).second;
		}
		
		template <class Ev
		> static bool add_listener(event_listener * value) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			return add_listener(hashof_v<Ev>, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void fire_event(event const & value) noexcept
		{
			static auto & inst{ get_instance() };

			// find category
			if (auto const cat{ inst.m_listeners.find(value.ID) })
			{
				// for each listener
				for (auto const & listener : (*cat->second))
				{
					// handle event
					listener->on_event(value);
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

		static void remove_listener(hash_t id, event_listener * value) noexcept
		{
			static auto & inst{ get_instance() };

			if (!value) { return; }

			// find category
			if (auto const cat{ inst.m_listeners.find(id) })
			{
				// find listener
				if (auto const it{ cat->second->find(value) }; it != cat->second->end())
				{
					// remove listener
					cat->second->erase(it);
				}
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void remove_listener(event_listener * value) noexcept
		{
			static auto & inst{ get_instance() };

			if (!value) { return; }

			// for each category
			inst.m_listeners.for_each([&](hash_t, auto & cat) noexcept
			{
				// find listener
				if (auto const it{ cat.find(value) }; it != cat.end())
				{
					// remove listener
					cat.erase(it);
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
		event_system::remove_listener(this); // remove from all events
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_