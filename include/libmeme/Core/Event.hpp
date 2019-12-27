#ifndef _ML_EVENT_HPP_
#define _ML_EVENT_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Core.hpp>

// The number of events reserved for each sub-library
#ifndef ML_MAX_LIBRARY_EVENTS
#define ML_MAX_LIBRARY_EVENTS 64
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Event
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : int32_t
		{
			MAX_LIBRARY_EVENTS = ML_MAX_LIBRARY_EVENTS,

			EV_INVALID	= -1,
			EV_CORE		= (MAX_LIBRARY_EVENTS * 1),
			EV_EDITOR	= (MAX_LIBRARY_EVENTS * 2),
			EV_ENGINE	= (MAX_LIBRARY_EVENTS * 3),
			EV_PLATFORM = (MAX_LIBRARY_EVENTS * 4),
			EV_NETWORK	= (MAX_LIBRARY_EVENTS * 5),
			EV_RENDERER = (MAX_LIBRARY_EVENTS * 6),
			EV_CUSTOM	= (MAX_LIBRARY_EVENTS * 7),
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Event(int32_t value) noexcept : m_id{ value }
		{
		}

		constexpr Event() noexcept : Event{ EV_INVALID }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator bool() const noexcept
		{
			return (m_id > EV_INVALID);
		}

		constexpr int32_t const & id() const noexcept
		{
			return m_id;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: int32_t const m_id;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <int32_t ID> struct I_Event : public Event
	{
		enum : int32_t { ID = ID };

		constexpr I_Event() noexcept : Event{ ID }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, T ID> struct T_Event : public I_Event<static_cast<int32_t>(ID)>
	{
		constexpr T_Event() noexcept : I_Event<static_cast<int32_t>(ID)>{}
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ev> static constexpr decltype(auto) event_cast(Event const & value)
	{
		return (value.id() == Ev::ID) ? static_cast<Ev const *>(&value) : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_HPP_