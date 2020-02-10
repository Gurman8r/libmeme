#ifndef _ML_WRAPPER_HPP_
#define _ML_WRAPPER_HPP_

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// value wrapper
	template <class T> struct value_wrapper
	{
		T value;

		constexpr T &		operator *() & noexcept			{ return value; }
		constexpr T &&		operator *() && noexcept		{ return value; }
		constexpr T const &	operator *() const & noexcept	{ return value; }
		constexpr T *		operator->() & noexcept			{ return &value; }
		constexpr T const *	operator->() const & noexcept	{ return &value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WRAPPER_HPP_