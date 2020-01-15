#ifndef _ML_SQRT_HPP_
#define _ML_SQRT_HPP_

#include <libmeme/Core/Limits.hpp>

namespace ml::alg::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct sqrt;

	template <> struct sqrt<> final { sqrt() = delete; };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct sqrt<uintmax_t>
	{
		using type = typename uintmax_t;

		ML_NODISCARD constexpr type operator()(type value) const
		{
			return sqrt<type>{}(cast<type>::one, cast<type>::three, value);
		}

		ML_NODISCARD constexpr type operator()(type value, type curr, type prev) const
		{
			return (value <= prev)
				? sqrt<type>{}(value + curr, curr + cast<type>::two, prev)
				: (curr >> cast<type>::one) - cast<type>::one;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct sqrt<float64_t>
	{
		using type = typename float64_t;

		ML_NODISCARD constexpr type operator()(type value, type curr, type prev) const
		{
			return (curr == prev)
				? curr
				: sqrt<type>{}(value, cast<type>::half * (curr + value / curr), curr);
		}

		ML_NODISCARD constexpr type operator()(type value) const
		{
			return (value >= cast<type>::zero && value < limits<type>::infinity)
				? sqrt<type>{}(value, value, cast<type>::zero)
				: limits<type>::nan;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From> struct sqrt<To, From> : public sqrt<From>
	{
		template <class U> ML_NODISCARD constexpr To operator()(U const & value) const
		{
			return static_cast<To>(sqrt<From>{}(static_cast<From>(value)));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct sqrt<intmax_t>
	{
		ML_NODISCARD constexpr intmax_t operator()(intmax_t value) const
		{
			return sqrt<intmax_t, uintmax_t>{}(value);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct sqrt<float32_t>
	{
		ML_NODISCARD constexpr float32_t operator()(float32_t value) const
		{
			return sqrt<float32_t, float64_t>{}(value);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::alg
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct sqrt final : public _ML alg::impl::sqrt<T>
	{
		ML_NODISCARD constexpr T operator()(T value) const
		{
			return _ML alg::impl::sqrt<T>{}(value);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SQRT_HPP_