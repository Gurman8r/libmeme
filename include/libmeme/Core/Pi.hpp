#ifndef _ML_PI_HPP_
#define _ML_PI_HPP_

#include <libmeme/Core/Cast.hpp>

namespace ml
{
	template <class T> struct pi final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type = typename T;

		static constexpr auto value		{ static_cast<type>(3.14159265358979323846264338327L) };
		static constexpr auto two		{ value * cast<type>::two };
		static constexpr auto half		{ value * cast<type>::half };
		static constexpr auto quarter	{ value * cast<type>::quarter };
		static constexpr auto third		{ value * cast<type>::third };
		static constexpr auto deg2rad	{ value / cast<type>::one_eighty };
		static constexpr auto rad2deg	{ cast<type>::one_eighty / value };

		constexpr operator type const &() const { return pi<type>::value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PI_HPP_