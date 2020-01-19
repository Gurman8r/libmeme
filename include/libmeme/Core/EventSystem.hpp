#ifndef _ML_EVENT_SYSTEM_HPP_
#define _ML_EVENT_SYSTEM_HPP_

#include <libmeme/Core/EventListener.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/FlatMap.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API event_system final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool add_listener(int32_t type, event_listener * listener);
		
		static void fire_event(struct event const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void remove_listener(int32_t type, event_listener * listener);
		
		static void remove_listener(event_listener * listener);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ev
		> static inline bool add_listener(event_listener * listener)
		{
			return add_listener(hashof_v<Ev>, listener);
		}

		template <class Ev, class ... Args
		> static inline void fire_event(Args && ... args)
		{
			return fire_event(Ev{ std::forward<Args>(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_