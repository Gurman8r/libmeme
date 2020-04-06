#ifndef _ML_EVENT_HPP_
#define _ML_EVENT_HPP_

#include <libmeme/Core/NonCopyable.hpp>
#include <libmeme/Core/NameOf.hpp>

#define ML_event(name) struct name final : _ML T_event<name>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// is event
	template <class Ev
	> static constexpr bool is_event_v{ std::is_base_of_v<struct event, Ev> };

	// event cast
	template <class Ev
	> ML_NODISCARD constexpr auto event_cast(struct event const & value) noexcept
	{
		static_assert(is_event_v<Ev>, "invalid event cast");
		return (value.id() == hashof_v<Ev>) ? static_cast<Ev const *>(&value) : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// base event
	struct event : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit event(hash_t const value) noexcept
			: m_id{ value }
		{
		}

		ML_NODISCARD constexpr hash_t id() const noexcept
		{
			return m_id;
		}

		template <class Ev
		> ML_NODISCARD constexpr Ev const * as() const noexcept
		{
			return event_cast<Ev>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		hash_t const m_id;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// template event interface
	template <class T> struct T_event : event
	{
		constexpr T_event() noexcept : event{ hashof_v<T> }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_HPP_