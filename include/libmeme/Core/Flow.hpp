#ifndef _ML_FLOW_HPP_
#define _ML_FLOW_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// lambda struct helper
#define ML_lambda_decl(Fn, type)                                                        \
    template <class Fn> struct type;                                                    \
    enum class ML_concat(type, _tag) {};                                                \
    template <class Fn> inline auto operator+(ML_concat(type, _tag), Fn && fn) noexcept \
    {                                                                                   \
        return type<Fn>{ ML_forward(fn) };                                              \
    }                                                                                   \
    template <class Fn> struct type final

// lambda variable helper
#define ML_lambda_impl(type) \
    ML_anon_v(ML_concat(_ML impl::, ML_concat(type, _tag))) {} + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke body in constructor
    ML_lambda_decl(Fn, immediate_call)
    {
        explicit immediate_call(Fn && fn) noexcept { std::invoke(ML_forward(fn)); }
    };

    // invoke body in destructor
    ML_lambda_decl(Fn, deferred_call)
    {
        explicit deferred_call(Fn && fn) noexcept : m_fn{ ML_forward(fn) } {}

        ~deferred_call() noexcept { std::invoke(m_fn); }

    private: Fn const m_fn;
    };
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
#define ML_scope ML_lambda_impl(immediate_call) // invoke body immediately
#define ML_defer ML_lambda_impl(deferred_call)  // invoke body on scope exit
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_FLOW_HPP_