#ifndef _ML_FLOW_HPP_
#define _ML_FLOW_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// flow control declarator helper
#define ML_flow_control_decl(Fn, type)                                                  \
    template <class Fn> struct type;                                                    \
    enum class ML_concat(type, _tag) {};                                                \
    template <class Fn> inline auto operator+(ML_concat(type, _tag), Fn && fn) noexcept \
    {                                                                                   \
        return type<Fn>{ ML_forward(fn) };                                              \
    }                                                                                   \
    template <class Fn> struct ML_NODISCARD type final

// flow control implementor helper
#define ML_flow_control_impl(type) \
    auto ML_anon = _ML impl:: ML_concat(type, _tag){} + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke in constructor
    ML_flow_control_decl(Fn, immediate_scope)
    {
        immediate_scope(Fn && fn) noexcept { std::invoke(ML_forward(fn)); }
    };

    // invoke in destructor
    ML_flow_control_decl(Fn, deferred_scope)
    {
        deferred_scope(Fn && fn) noexcept : m_fn{ ML_forward(fn) } {}

        ~deferred_scope() noexcept { std::invoke(m_fn); }

    private: Fn const m_fn;
    };
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// invoke immediately
#define ML_scope \
    ML_flow_control_impl(immediate_scope)

// invoke on scope exit
#define ML_defer \
    ML_flow_control_impl(deferred_scope)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_FLOW_HPP_