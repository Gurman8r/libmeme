#ifndef _ML_SCOPE_GUARD_HPP_
#define _ML_SCOPE_GUARD_HPP_

#include <libmeme/Common.hpp>
#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/NonCopyable.hpp>

#define ML_SCOPE_EXIT \
	auto ML_ANON(ML_SCOPE_EXIT_STATE) \
	= ::ml::detail::ScopeGuardOnExit() + [&]() noexcept

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fun> struct ScopeGuard final : public NonCopyable
	{
		ScopeGuard(Fun && fun) : m_fun{ std::forward<Fun>(fun) } {}

		~ScopeGuard() { std::invoke(m_fun); }

	private: Fun m_fun;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class ScopeGuardOnExit {};

	template <class Fun> inline ScopeGuard<Fun> operator+(ScopeGuardOnExit, Fun && fun)
	{
		return ScopeGuard<Fun>{ std::forward<Fun>(fun) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCOPE_GUARD_HPP_