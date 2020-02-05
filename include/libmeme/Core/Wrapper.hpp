#ifndef _ML_WRAPPER_HPP_
#define _ML_WRAPPER_HPP_

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct wrapper
	{
		using type = typename T;

		type value;

		constexpr type &		operator *() noexcept		{ return value; }
		constexpr type const &	operator *() const noexcept { return value; }
		constexpr type *		operator->() noexcept		{ return &value; }
		constexpr type const *	operator->() const noexcept { return &value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WRAPPER_HPP_