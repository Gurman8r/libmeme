#ifndef _ML_LIMITS_HPP_
#define _ML_LIMITS_HPP_

#include <libmeme/Core/Cast.hpp>

namespace ml
{
	template <class ... T> struct limits;

	template <> struct limits<> final { limits() = delete; };

	template <class T> struct limits<T> final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type = typename T;

		using self_type = typename limits<type>;

		limits() = delete;

		static constexpr type infinity		{ std::numeric_limits<type>::infinity() };
		static constexpr type nan			{ std::numeric_limits<type>::quiet_NaN() };
		static constexpr type min			{ std::numeric_limits<type>::min() };
		static constexpr type max			{ std::numeric_limits<type>::max() };
		static constexpr type epsilon		{ std::numeric_limits<type>::epsilon() };
		static constexpr type half_epsilon	{ epsilon * cast<type>::half };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_LIMITS_HPP_
