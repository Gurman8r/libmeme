#ifndef _ML_FLOW_HPP_
#define _ML_FLOW_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// flow struct helper
#define ML_decl_flow_struct(Fn, T)                                                      \
    template <class Fn> struct T;                                                       \
    enum class ML_concat(T, _tag) {};                                                   \
    template <class Fn> inline auto operator+(ML_concat(T, _tag), Fn && fn) noexcept    \
    {                                                                                   \
        return T<Fn>{ ML_forward(fn) };                                                 \
    }                                                                                   \
    template <class Fn> struct T final

// flow variable helper
#define ML_decl_flow_variable(T) \
    auto ML_anon(T) = _ML impl:: ML_concat(T, _tag){} + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke in constructor
    ML_decl_flow_struct(Fn, immediate_call)
    {
        explicit immediate_call(Fn && fn) noexcept { std::invoke(ML_forward(fn)); }
    };

    // invoke lambda immediately
#define ML_call \
    ML_decl_flow_variable(immediate_call)
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke in destructor
    ML_decl_flow_struct(Fn, deferred_call)
    {
        explicit deferred_call(Fn && fn) noexcept : m_fn{ ML_forward(fn) } {}

        ~deferred_call() noexcept { std::invoke(m_fn); }

    private: Fn const m_fn;
    };

    // invoke lambda on scope exit
#define ML_defer \
    ML_decl_flow_variable(deferred_call)
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_FLOW_HPP_