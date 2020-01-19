#ifndef _ML_EVENT_HPP_
#define _ML_EVENT_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/TypeOf.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ev
	> ML_NODISCARD static constexpr auto event_cast(struct event const * value) noexcept
	{
		return (value && (value->id() == hashof_v<Ev>))
			? static_cast<Ev const *>(value)
			: nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// basic event
	struct event
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit event(size_t const value) noexcept
			: m_id{ value }
		{
		}

		ML_NODISCARD constexpr size_t const & id() const noexcept
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

	// event interface
	template <class T
	> struct I_event : event
	{
		constexpr I_event() noexcept : event{ hashof_v<T> } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_HPP_