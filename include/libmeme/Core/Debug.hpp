#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <libmeme/Core/StringUtility.hpp>

namespace ml::debug
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline int32_t clear(int32_t const exit_code = 0)
	{
#if ML_is_debug
#	ifdef ML_os_windows
		std::system("cls");
#	else
		std::system("clear");
#	endif
#endif
		return exit_code;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline int32_t pause(int32_t const exit_code = 0)
	{
#if ML_is_debug
#	ifdef ML_os_windows
		std::system("pause");
#	else
		std::cin.get();
#	endif
#endif
		return exit_code;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fmt
	> inline int32_t info(Fmt && fmt)
	{
		out() << "[ info ] " << ML_forward(fmt) << '\n';
		return 1; // true
	}

	template <class Fmt, class Arg0, class ... Args
	> inline int32_t info(Fmt && fmt, Arg0 && arg0, Args && ... args)
	{
		return info(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fmt
	> inline int32_t error(Fmt && fmt)
	{
		std::cout << "[ error ] " << ML_forward(fmt) << '\n';
		return 0; // false
	}

	template <class Fmt, class Arg0, class ... Args
	> inline int32_t error(Fmt && fmt, Arg0 && arg0, Args && ... args)
	{
		return error(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fmt
	> inline int32_t warning(Fmt && fmt)
	{
		std::cout << "[ warn ] " << ML_forward(fmt) << '\n';
		return -1; // true
	}

	template <class Fmt, class Arg0, class ... Args
	> inline int32_t warning(Fmt && fmt, Arg0 && arg0, Args && ... args)
	{
		return warning(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_