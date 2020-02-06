#ifndef _ML_EVENT_SYSTEM_HPP_
#define _ML_EVENT_SYSTEM_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Event.hpp>
#include <libmeme/Core/FlatMap.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API event_listener
	{
		virtual ~event_listener();

		virtual void on_event(event const & value) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API event_system final
	{
		static ds::flat_map<
			size_t, ds::flat_set<event_listener *>
		> s_listeners;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ev
		> static inline bool add_listener(event_listener * listener)
		{
			return add_listener(hashof_v<Ev>, listener);
		}

		template <class Ev, class ... Args
		> static inline void fire_event(Args && ... args)
		{
			return fire_event(Ev{ ML_FWD(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool add_listener(size_t type, event_listener * listener);
		
		static void fire_event(event const & value);

		static void remove_listener(size_t type, event_listener * listener);
		
		static void remove_listener(event_listener * listener);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_