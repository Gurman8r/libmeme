#ifndef _ML_FLOW_HPP_
#define _ML_FLOW_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// invoke lambda
#define ML_call \
    auto ML_anon(call_lambda) = _ML impl::ctor_lambda_tag{} + [&]() noexcept

// invoke lambda ONCE
#define ML_once_call \
    static ML_call

// invoke lambda on scope exit
#define ML_defer \
    auto ML_anon(defer_lambda) = _ML impl::dtor_lambda_tag{} + [&]() noexcept

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke lambda on constructor
    template <class Fn> struct ctor_lambda final
    {
        explicit ctor_lambda(Fn && fn) noexcept { std::invoke(ML_forward(fn)); }
    };

    enum class ctor_lambda_tag {};

    template <class Fn> inline auto operator+(ctor_lambda_tag, Fn && fn) noexcept
    {
        return ctor_lambda<Fn>{ ML_forward(fn) };
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::impl
{
    // invoke lambda on destructor
    template <class Fn> struct dtor_lambda final
    {
        explicit dtor_lambda(Fn && fn) noexcept : m_fn{ ML_forward(fn) } {}

        ~dtor_lambda() noexcept { std::invoke(m_fn); }

    private: Fn const m_fn;
    };

    enum class dtor_lambda_tag {};

    template <class Fn> inline auto operator+(dtor_lambda_tag, Fn && fn) noexcept
    {
        return dtor_lambda<Fn>{ ML_forward(fn) };
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_FLOW_HPP_