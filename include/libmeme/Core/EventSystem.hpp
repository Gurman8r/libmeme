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
		> static bool add_listener(event_listener * value) noexcept
		{
			static_assert(is_event_v<Ev>, "invalid event type");

			return add_listener(hashof_v<Ev>, value);
		}

		template <class Ev, class ... Args
		> static void fire_event(Args && ... args) noexcept
		{
			static_assert(is_event_v<Ev>, "invalid event type");

			return fire_event(Ev{ ML_forward(args)... });
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
		virtual ~event_listener() noexcept
		{
			event_system::remove_listener(this);
		}

		virtual void on_event(event const & ev) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_