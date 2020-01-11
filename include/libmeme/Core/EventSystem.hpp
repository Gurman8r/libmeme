#ifndef _ML_EVENT_SYSTEM_HPP_
#define _ML_EVENT_SYSTEM_HPP_

#include <libmeme/Core/EventListener.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/DenseMap.hpp>

namespace ml
{
	struct Event;

	struct ML_CORE_API EventSystem final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		EventSystem() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using listener_map = typename pair_multimap<int32_t, EventListener *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static EventListener * addListener(int32_t type, EventListener * listener);
		
		static void fireEvent(Event const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool removeListener(int32_t type, EventListener * listener);
		
		static bool removeListenerFromAllEvents(EventListener * listener);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ev
		> static inline EventListener * addListener(EventListener * listener)
		{
			return addListener(Ev::ID, listener);
		}

		template <class Ev, class ... Args
		> static inline void fireEvent(Args && ... args)
		{
			return fireEvent(Ev{ std::forward<Args>(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static listener_map m_listeners;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EVENT_SYSTEM_HPP_