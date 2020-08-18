#ifndef _ML_EVENT_HPP_
#define _ML_EVENT_HPP_

#include <libmeme/Core/NonCopyable.hpp>
#include <libmeme/Core/TypeInfo.hpp>

// event declarator helper
#define ML_decl_event(type) \
	struct type final : _ML impl::event_helper<type>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// base event
	struct ML_NODISCARD event : non_copyable
	{
		hash_t const id;

		template <class Ev> constexpr auto cast() const noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");
			return (this->id == hashof_v<Ev>) ? static_cast<Ev const *>(this) : nullptr;
		}

	protected:
		constexpr explicit event(hash_t const id) noexcept : id{ id }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		// event interface helper
		template <class T> struct event_helper : event
		{
			constexpr event_helper() noexcept : event{ hashof_v<T> }
			{
			}
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_HPP_