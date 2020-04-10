#ifndef _ML_SCOPE_GUARD_HPP_
#define _ML_SCOPE_GUARD_HPP_

#include <libmeme/Common.hpp>

// defer
#define ML_defer \
	 auto const ML_anon(scope_guard) = _ML impl::scope_guard_tag{} + [&]() noexcept

namespace ml::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn> struct scope_guard final
	{
		explicit scope_guard(Fn && fn) noexcept : m_fn{ ML_forward(fn) } {}

		~scope_guard() noexcept { std::invoke(m_fn); }

	private: Fn const m_fn;
	};

	enum class scope_guard_tag {};

	template <class Fn> inline auto operator+(scope_guard_tag, Fn && fn) noexcept
	{
		return scope_guard<Fn>{ ML_forward(fn) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCOPE_GUARD_HPP_