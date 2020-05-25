#ifndef _ML_FLOW_HPP_
#define _ML_FLOW_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke in constructor
    template <class Fn> struct ML_NODISCARD immediate_scope final
    {
        constexpr immediate_scope(Fn && fn) noexcept { ML_invoke(ML_forward(fn)); }
    };

    enum class ML_NODISCARD scope_tag {};

    template <class Fn
    > ML_NODISCARD constexpr auto operator+(scope_tag, Fn && fn) noexcept
    {
        return immediate_scope<Fn>{ ML_forward(fn) };
    }
}

// invoke on enter
#define ML_scope \
    auto ML_anon = _ML impl::scope_tag{} + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke in destructor
    template <class Fn> struct ML_NODISCARD deferred_scope final
    {
        deferred_scope(Fn && fn) noexcept : m_fn{ ML_forward(fn) } {}

        ~deferred_scope() noexcept { std::invoke(m_fn); }

    private: Fn const m_fn;
    };

    enum class ML_NODISCARD defer_tag {};

    template <class Fn
    > ML_NODISCARD auto operator+(defer_tag, Fn && fn) noexcept
    {
        return deferred_scope<Fn>{ ML_forward(fn) };
    }
}

// invoke on exit
#define ML_defer \
    auto ML_anon = _ML impl::defer_tag{} + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_FLOW_HPP_