#ifndef _ML_EVENT_SYSTEM_HPP_
#define _ML_EVENT_SYSTEM_HPP_

#include <libmeme/Core/EventListener.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

namespace ml
{
	struct Event;

	struct ML_CORE_API EventSystem final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		EventSystem() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool add_listener(int32_t type, EventListener * listener);
		
		static void fire_event(Event const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void remove_listener(int32_t type, EventListener * listener);
		
		static void remove_listener(EventListener * listener);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ev
		> static inline bool add_listener(EventListener * listener)
		{
			return add_listener(Ev::ID, listener);
		}

		template <class Ev, class ... Args
		> static inline void fire_event(Args && ... args)
		{
			return fire_event(Ev{ std::forward<Args>(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static ds::flat_map<int32_t, ds::flat_set<EventListener *>> m_listeners;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EVENT_SYSTEM_HPP_