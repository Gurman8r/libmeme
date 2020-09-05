#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <libmeme/Core/StringUtility.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ML_IMPL_DEBUG_MSG_I
#define ML_IMPL_DEBUG_MSG_I "[i] " // info message
#endif

#ifndef ML_IMPL_DEBUG_MSG_E
#define ML_IMPL_DEBUG_MSG_E "[e] " // error message
#endif

#ifndef ML_IMPL_DEBUG_MSG_W
#define ML_IMPL_DEBUG_MSG_W "[w] " // warning message
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// assert
#ifndef ML_assert
#	ifdef assert
#		define ML_assert(expr)	assert(expr)
#	else
#		define ML_assert(expr)	(void)(expr)
#	endif
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// breakpoint
#ifndef ML_breakpoint

#	if defined(ML_cc_msvc)
#		define ML_breakpoint()	::__debugbreak()

#	elif defined(ML_cc_clang)
#		define ML_breakpoint()	::__builtin_debugtrap()

#	else
#		define ML_breakpoint()	::raise(SIGTRAP)

#	endif
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::debug
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline int32_t clear(int32_t exit_code = 0) noexcept
	{
#ifdef ML_os_windows
		std::system("cls");
#else
		std::system("clear");
#endif
		return exit_code;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline int32_t pause(int32_t exit_code = 0) noexcept
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
	> decltype(auto) puts(Str && str) noexcept
	{
		return std::cout << ML_forward(str) << '\n';
	}

	template <class Fmt, class Arg0, class ... Args
	> decltype(auto) puts(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return _ML debug::puts(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t info() noexcept { return 1; } // true

	template <class Str
	> int32_t info(Str && str) noexcept
	{
		std::cout << ML_IMPL_DEBUG_MSG_I << ML_forward(str) << '\n';

		return _ML debug::info();
	}

	template <class Fmt, class Arg0, class ... Args
	> int32_t info(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return _ML debug::info(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t error() noexcept { return 0; } // false

	template <class Str
	> int32_t error(Str && str) noexcept
	{
		std::cout << ML_IMPL_DEBUG_MSG_E << ML_forward(str) << '\n';

		return _ML debug::error();
	}

	template <class Fmt, class Arg0, class ... Args
	> int32_t error(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return _ML debug::error(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t warning() noexcept { return -1; } // true

	template <class Str
	> int32_t warning(Str && str) noexcept
	{
		std::cout << ML_IMPL_DEBUG_MSG_W << ML_forward(str) << '\n';

		return _ML debug::warning();
	}

	template <class Fmt, class Arg0, class ... Args
	> int32_t warning(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return _ML debug::warning(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_