#ifndef _ML_FLOW_HPP_
#define _ML_FLOW_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// flow control declaration helper
#define ML_decl_flow_control(Fn, type)                                                  \
    template <class Fn> struct type;                                                    \
    enum class ML_concat(type, _tag) {};                                                \
    template <class Fn> inline auto operator+(ML_concat(type, _tag), Fn && fn) noexcept \
    {                                                                                   \
        return type<Fn>{ ML_forward(fn) };                                              \
    }                                                                                   \
    template <class Fn> struct type final

// flow control implementation helper
#define ML_impl_flow_control(type) \
    ML_anon_v(ML_concat(_ML impl::, ML_concat(type, _tag))) {} + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke body in constructor
    ML_decl_flow_control(Fn, immediate_call)
    {
        explicit immediate_call(Fn && fn) noexcept { std::invoke(ML_forward(fn)); }
    };

    // invoke body immediately
#define ML_block \
    ML_impl_flow_control(immediate_call)
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke body in destructor
    ML_decl_flow_control(Fn, deferred_call)
    {
        explicit deferred_call(Fn && fn) noexcept : m_fn{ ML_forward(fn) } {}

        ~deferred_call() noexcept { std::invoke(m_fn); }

    private: Fn const m_fn;
    };

    // invoke body on scope exit
#define ML_defer \
    ML_impl_flow_control(deferred_call)
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_FLOW_HPP_