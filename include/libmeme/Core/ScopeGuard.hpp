#ifndef _ML_SCOPE_GUARD_HPP_
#define _ML_SCOPE_GUARD_HPP_

#include <libmeme/Common.hpp>

#define ML_SCOPE_EXIT auto ML_ANON(ML_SCOPE_GUARD_ON_EXIT) \
	= ::ml::detail::scope_guard_on_exit() + [&]() noexcept

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn
	> struct scope_guard
	{
		scope_guard(Fn && fun) noexcept : m_fn{ std::forward<Fn>(fun) } {}

		~scope_guard() { std::invoke(m_fn); }

	private: Fn m_fn;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class scope_guard_on_exit {};

	template <class Fn
	> inline scope_guard<Fn> operator+(scope_guard_on_exit, Fn && fun)
	{
		return scope_guard<Fn>{ std::forward<Fn>(fun) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCOPE_GUARD_HPP_