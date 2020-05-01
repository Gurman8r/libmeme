#ifndef _ML_EVENT_HPP_
#define _ML_EVENT_HPP_

#include <libmeme/Core/NonCopyable.hpp>
#include <libmeme/Core/NameOf.hpp>

#define ML_event(name) struct name final : _ML event_impl<name>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// is event
	template <class Ev
	> static constexpr bool is_event_v{ std::is_base_of_v<struct event, Ev> };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// base event
	struct event : non_copyable
	{
		hash_t const ID;

		constexpr explicit event(hash_t const id) noexcept : ID{ id }
		{
		}

		template <class Ev
		> ML_NODISCARD constexpr Ev const * cast() const noexcept
		{
			static_assert(is_event_v<Ev>, "invalid event type");

			return (ID == hashof_v<Ev>) ? static_cast<Ev const *>(this) : nullptr;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// template event interface
	template <class T> struct event_impl : event
	{
		constexpr event_impl() noexcept : event{ hashof_v<T> }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_HPP_