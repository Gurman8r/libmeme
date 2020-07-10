#ifndef _ML_FLOW_HPP_
#define _ML_FLOW_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke function in constructor
    template <class Fn> struct ML_NODISCARD scope_impl final
    {
        constexpr scope_impl(Fn && fn) noexcept { ML_forward(fn)(); }

        constexpr operator bool() const noexcept { return true; }
    };

    enum class ML_NODISCARD scope_tag {};

    template <class Fn
    > ML_NODISCARD constexpr auto operator+(scope_tag, Fn && fn) noexcept
    {
        return scope_impl<Fn>{ ML_forward(fn) };
    }
}

// scope
#define ML_scope(...) \
    auto ML_anon = _ML_IMPL scope_tag{} + [##__VA_ARGS__]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke function in destructor
    template <class Fn> struct ML_NODISCARD defer_impl final
    {
        defer_impl(Fn && fn) noexcept : m_fn{ ML_forward(fn) } {}

        ~defer_impl() noexcept { m_fn(); }

    private: Fn const m_fn;
    };

    enum class ML_NODISCARD defer_tag {};

    template <class Fn
    > ML_NODISCARD auto operator+(defer_tag, Fn && fn) noexcept
    {
        return defer_impl<Fn>{ ML_forward(fn) };
    }
}

// defer
#define ML_defer(...) \
    auto ML_anon = _ML_IMPL defer_tag{} + [##__VA_ARGS__]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_FLOW_HPP_