#ifndef _ML_FLOW_HPP_
#define _ML_FLOW_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// flow struct helper
#define ML_flow_decl(Fn, Type)                                                          \
    template <class Fn> struct Type;                                                    \
    enum class ML_concat(Type, _tag) {};                                                \
    template <class Fn> inline auto operator+(ML_concat(Type, _tag), Fn && fn) noexcept \
    {                                                                                   \
        return Type<Fn>{ ML_forward(fn) };                                              \
    }                                                                                   \
    template <class Fn> struct Type final

// flow variable helper
#define ML_flow_impl(Type) \
    auto ML_anon(Type) = _ML impl:: ML_concat(Type, _tag){} + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke function in constructor
    ML_flow_decl(Fn, immediate_call)
    {
        explicit immediate_call(Fn && fn) noexcept { std::invoke(ML_forward(fn)); }
    };

    // invoke lambda body immediately
#define ML_call ML_flow_impl(immediate_call)
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke function in destructor
    ML_flow_decl(Fn, deferred_call)
    {
        explicit deferred_call(Fn && fn) noexcept : m_fn{ ML_forward(fn) } {}

        ~deferred_call() noexcept { std::invoke(m_fn); }

    private: Fn const m_fn;
    };

    // invoke lambda body on scope exit
#define ML_defer ML_flow_impl(deferred_call)
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_FLOW_HPP_