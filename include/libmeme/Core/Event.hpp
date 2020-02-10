#ifndef _ML_EVENT_HPP_
#define _ML_EVENT_HPP_

#include <libmeme/Core/NonCopyable.hpp>
#include <libmeme/Core/TypeOf.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ev
	> ML_NODISCARD static constexpr auto event_cast(struct event const * value) noexcept
	{
		static_assert(std::is_base_of_v<struct event, Ev>, "bad event cast");
		return (value && (value->id() == hashof_v<Ev>))
			? static_cast<Ev const *>(value)
			: nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// base event
	struct event : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit event(size_t const value) noexcept
			: m_id{ value }
		{
		}

		ML_NODISCARD constexpr size_t id() const noexcept
		{
			return m_id;
		}

		template <class Ev
		> ML_NODISCARD constexpr Ev const * as() const noexcept
		{
			return event_cast<Ev>(this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		size_t const m_id;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// interface event that uses the hash code of T as its id
	template <class T> struct T_event : event
	{
		constexpr T_event() noexcept : event{ hashof_v<T> }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_HPP_