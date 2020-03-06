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
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ev
		> static inline bool add_listener(event_listener * value)
		{
			return add_listener(hashof_v<Ev>, value);
		}

		template <class Ev, class ... Args
		> static inline void fire_event(Args && ... args)
		{
			return fire_event(Ev{ ML_fwd(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool add_listener(hash_t type, event_listener * value);
		
		static void fire_event(event const & value);

		static void remove_listener(hash_t type, event_listener * value);
		
		static void remove_listener(event_listener * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API event_listener
	{
		virtual ~event_listener()
		{
			event_system::remove_listener(this);
		}

		virtual void on_event(event const & value) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_