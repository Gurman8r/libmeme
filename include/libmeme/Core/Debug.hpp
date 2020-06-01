#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <libmeme/Core/StringUtility.hpp>

namespace ml::debug
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline int32_t clear(int32_t const exit_code = 0) noexcept
	{
#ifdef ML_os_windows
		std::system("cls");
#else
		std::system("clear");
#endif
		return exit_code;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline int32_t pause(int32_t const exit_code = 0) noexcept
	{
#ifdef ML_os_windows
		std::system("pause");
#else
		std::cin.get();
#endif
		return exit_code;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T = pmr::string
	> void puts(T && value) noexcept
	{
		std::cout << ML_forward(value) << '\n';
	}

	template <class Fmt, class Arg0, class ... Args
	> void puts(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		debug::puts(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t ok() noexcept { return 1; } // true

	template <class T = pmr::string
	> int32_t ok(T && value) noexcept
	{
		std::cout << "[ info ] " << ML_forward(value) << '\n';
		return ok();
	}

	template <class Fmt, class Arg0, class ... Args
	> int32_t ok(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return ok(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t error() noexcept { return 0; } // false

	template <class T = pmr::string
	> int32_t error(T && value) noexcept
	{
		std::cout << "[ error ] " << ML_forward(value) << '\n';
		return error();
	}

	template <class Fmt, class Arg0, class ... Args
	> int32_t error(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return error(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t warning() noexcept { return -1; } // true

	template <class T = pmr::string
	> int32_t warning(T && value) noexcept
	{
		std::cout << "[ warn ] " << ML_forward(value) << '\n';
		return warning();
	}

	template <class Fmt, class Arg0, class ... Args
	> int32_t warning(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return warning(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_