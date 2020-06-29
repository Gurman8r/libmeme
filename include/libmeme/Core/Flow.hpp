#ifndef _ML_FLOW_HPP_
#define _ML_FLOW_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke in constructor
    template <class Fn> struct ML_NODISCARD scope_impl final
    {
        constexpr scope_impl(Fn && fn) noexcept { std::invoke(ML_forward(fn)); }
    };

    enum class ML_NODISCARD scope_tag {};

    template <class Fn
    > ML_NODISCARD constexpr auto operator+(scope_tag, Fn && fn) noexcept
    {
        return scope_impl<Fn>{ ML_forward(fn) };
    }
}

// invoke on enter
#define ML_scope \
    auto ML_anon = _ML_IMPL scope_tag{} + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke in destructor
    template <class Fn> struct ML_NODISCARD defer_impl final
    {
        defer_impl(Fn && fn) noexcept : m_fn{ ML_forward(fn) } {}

        ~defer_impl() noexcept { std::invoke(m_fn); }

    private: Fn const m_fn;
    };

    enum class ML_NODISCARD defer_tag {};

    template <class Fn
    > ML_NODISCARD auto operator+(defer_tag, Fn && fn) noexcept
    {
        return defer_impl<Fn>{ ML_forward(fn) };
    }
}

// invoke on exit
#define ML_defer \
    auto ML_anon = _ML_IMPL defer_tag{} + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_FLOW_HPP_