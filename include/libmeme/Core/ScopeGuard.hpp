#ifndef _ML_SCOPE_GUARD_HPP_
#define _ML_SCOPE_GUARD_HPP_

#include <libmeme/Common.hpp>

// defer
#define ML_impl_defer	::ml::impl::scope_guard_on_exit{} + [&]() noexcept
#define ML_defer		auto ML_anon(scope_guard) = ML_impl_defer

namespace ml::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn
	> struct scope_guard final
	{
		explicit scope_guard(Fn && fn) noexcept : m_fn{ ML_forward(fn) } {}

		~scope_guard() noexcept { std::invoke(m_fn); }

	private: Fn const m_fn;
	};

	enum class scope_guard_on_exit {};

	template <class Fn> inline auto operator+(scope_guard_on_exit, Fn && fn) noexcept
	{
		return scope_guard<Fn>{ ML_forward(fn) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCOPE_GUARD_HPP_