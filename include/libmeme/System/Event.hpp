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
		constexpr operator hash_t() const noexcept { return m_id; }

	protected:
		constexpr explicit event(hash_t const m_id) noexcept : m_id{ m_id }
		{
		}

		hash_t const m_id;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		// event interface helper
		template <class T> struct event_helper : event
		{
			enum : hash_t { ID = hashof_v<T> };

			constexpr event_helper() noexcept : event{ ID }
			{
			}
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_HPP_