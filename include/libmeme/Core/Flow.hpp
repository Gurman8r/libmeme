#ifndef _ML_FLOW_HPP_
#define _ML_FLOW_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// flow control declarator
#define ML_flow_control_decl(Fn, type)                                                  \
    template <class Fn> struct type;                                                    \
    enum class ML_concat(type, _tag) {};                                                \
    template <class Fn> inline auto operator+(ML_concat(type, _tag), Fn && fn) noexcept \
    {                                                                                   \
        return type<Fn>{ ML_forward(fn) };                                              \
    }                                                                                   \
    template <class Fn> struct type final

// flow control implementor
#define ML_flow_control_impl(type) \
    ML_anon_v(ML_concat(_ML impl::, ML_concat(type, _tag))) {} + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke body in constructor
    ML_flow_control_decl(Fn, immediate_call)
    {
        explicit immediate_call(Fn && fn) noexcept { std::invoke(ML_forward(fn)); }
    };

    // invoke body immediately
#define ML_block \
    ML_flow_control_impl(immediate_call)
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke body in destructor
    ML_flow_control_decl(Fn, deferred_call)
    {
        explicit deferred_call(Fn && fn) noexcept : m_fn{ ML_forward(fn) } {}

        ~deferred_call() noexcept { std::invoke(m_fn); }

    private: Fn const m_fn;
    };

    // invoke body on scope exit
#define ML_defer \
    ML_flow_control_impl(deferred_call)
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_FLOW_HPP_