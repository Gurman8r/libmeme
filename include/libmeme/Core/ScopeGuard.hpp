#ifndef _ML_SCOPE_GUARD_HPP_
#define _ML_SCOPE_GUARD_HPP_

#include <libmeme/Config.hpp>

#define ML_defer auto ML_ANONYMOUS(scope_guard_on_exit) \
	= ::ml::detail::scope_guard_on_exit() + [&]() noexcept

namespace ml::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn
	> struct scope_guard final
	{
		scope_guard(Fn fn) noexcept : m_fn{ fn } {}

		~scope_guard() noexcept { m_fn(); }

	private: Fn const m_fn;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class scope_guard_on_exit {};

	template <class Fn
	> inline scope_guard<Fn> operator+(scope_guard_on_exit, Fn fn) noexcept
	{
		return scope_guard<Fn>{ fn };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCOPE_GUARD_HPP_