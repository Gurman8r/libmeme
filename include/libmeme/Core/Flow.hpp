#ifndef _ML_FLOW_HPP_
#define _ML_FLOW_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke in constructor
    template <class Fn> struct ML_NODISCARD immediate_scope final
    {
        immediate_scope(Fn && fn) noexcept { std::invoke(ML_forward(fn)); }
    };

    enum class ML_NODISCARD immediate_scope_tag {};

    template <class Fn
    > ML_NODISCARD inline auto operator+(immediate_scope_tag, Fn && fn) noexcept
    {
        return immediate_scope<Fn>{ ML_forward(fn) };
    }
}

// invoke immediately
#define ML_scope \
    auto ML_anon = _ML impl::immediate_scope_tag{} + [&]() noexcept

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

    enum class ML_NODISCARD deferred_scope_tag {};

    template <class Fn
    > ML_NODISCARD inline auto operator+(deferred_scope_tag, Fn && fn) noexcept
    {
        return deferred_scope<Fn>{ ML_forward(fn) };
    }
}

// invoke on scope exit
#define ML_defer \
    auto ML_anon = _ML impl::deferred_scope_tag{} + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_FLOW_HPP_