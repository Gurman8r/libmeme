#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <libmeme/Core/StringUtility.hpp>

#define _ML_DEBUG _ML debug::

namespace ml::debug
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline std::ostream & std_err() noexcept { return std::cerr; }

	inline std::ostream & std_out() noexcept { return std::cout; }

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

	template <class Str
	> void puts(Str && str) noexcept
	{
		_ML_DEBUG std_out() << ML_forward(str) << '\n';
	}

	template <class Fmt, class Arg0, class ... Args
	> void puts(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		_ML_DEBUG puts(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t info() noexcept { return 1; } // true

	template <class Str
	> int32_t info(Str && str) noexcept
	{
		_ML_DEBUG std_out() << "[i] " << ML_forward(str) << '\n';

		return _ML_DEBUG info();
	}

	template <class Fmt, class Arg0, class ... Args
	> int32_t info(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return _ML_DEBUG info(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t error() noexcept { return 0; } // false

	template <class Str
	> int32_t error(Str && str) noexcept
	{
		_ML_DEBUG std_out() << "[e] " << ML_forward(str) << '\n';

		return _ML_DEBUG error();
	}

	template <class Fmt, class Arg0, class ... Args
	> int32_t error(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return _ML_DEBUG error(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t warning() noexcept { return -1; } // true

	template <class Str
	> int32_t warning(Str && str) noexcept
	{
		_ML_DEBUG std_out() << "[w] " << ML_forward(str) << '\n';

		return _ML_DEBUG warning();
	}

	template <class Fmt, class Arg0, class ... Args
	> int32_t warning(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return _ML_DEBUG warning(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_