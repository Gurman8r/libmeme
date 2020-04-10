#ifndef _ML_ONCE_CALL_HPP_
#define _ML_ONCE_CALL_HPP_

#include <libmeme/Common.hpp>

// once
#define ML_once \
    static auto const ML_anon(once_call) = _ML impl::once_call_tag{} + [&]() noexcept

namespace ml::impl
{
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    template <class Fn> struct once_call final
    {
        explicit once_call(Fn && fn) noexcept
        {
            std::invoke(ML_forward(fn));
        }
    };

    enum class once_call_tag {};

    template <class Fn> inline auto operator+(once_call_tag, Fn && fn) noexcept
    {
        return once_call<Fn>{ ML_forward(fn) };
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ONCE_CALL_HPP_