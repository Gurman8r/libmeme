#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <libmeme/Core/StringUtility.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ML_IMPL_DEBUG_MSG_I
#define ML_IMPL_DEBUG_MSG_I "[info] " // info message
#endif

#ifndef ML_IMPL_DEBUG_MSG_E
#define ML_IMPL_DEBUG_MSG_E "[error] " // error message
#endif

#ifndef ML_IMPL_DEBUG_MSG_W
#define ML_IMPL_DEBUG_MSG_W "[warn] " // warning message
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// assert ex
#ifndef ML_assert_ex

#	ifdef ML_cc_msvc
#		define ML_assert_ex(expr, msg, file, line) (void)((!!(expr)) || (_wassert(_CRT_WIDE(msg), _CRT_WIDE(file), (unsigned)(line)), 0))

#	elif defined(assert)
#		define ML_assert_ex(expr, msg, file, line) assert(expr)

#	else
#		define ML_assert_ex(expr, msg, file, line) ((void)expr)

#	endif
#endif

// assert
#define ML_assert(expr) \
	ML_assert_ex(expr, #expr, __FILE__, __LINE__)

// check
#define ML_check(expr) \
	([](auto p) noexcept { ML_assert_ex(p, "CHECK FAILED: " #expr, __FILE__, __LINE__); return p; })(expr)

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

	static struct // io
	{
		std::ostream & out	{ std::cout };
		std::ostream & err	{ std::cerr };
		std::istream & in	{ std::cin };
	}
	const io;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline int32_t clear(int32_t exit_code = EXIT_SUCCESS) noexcept
	{
#ifdef ML_os_windows
		std::system("cls");
#else
		std::system("clear");
#endif
		return exit_code;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline int32_t pause(int32_t exit_code = EXIT_SUCCESS) noexcept
	{
#ifdef ML_os_windows
		std::system("pause");
#else
		io.in.get();
#endif
		return exit_code;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fmt
	> auto & puts(Fmt && fmt) noexcept
	{
		return io.out << ML_forward(fmt) << '\n';
	}

	template <class Fmt, class Arg0, class ... Args
	> auto & puts(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return debug::puts(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t info() noexcept { return 1; } // true

	template <class Fmt
	> int32_t info(Fmt && fmt) noexcept
	{
		io.out << ML_IMPL_DEBUG_MSG_I << ML_forward(fmt) << '\n';

		return debug::info();
	}

	template <class Fmt, class Arg0, class ... Args
	> int32_t info(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return debug::info(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t error() noexcept { return 0; } // false

	template <class Fmt
	> int32_t error(Fmt && fmt) noexcept
	{
		io.out << ML_IMPL_DEBUG_MSG_E << ML_forward(fmt) << '\n';

		return debug::error();
	}

	template <class Fmt, class Arg0, class ... Args
	> int32_t error(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return debug::error(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr int32_t warning() noexcept { return -1; } // true

	template <class Fmt
	> int32_t warning(Fmt && fmt) noexcept
	{
		io.out << ML_IMPL_DEBUG_MSG_W << ML_forward(fmt) << '\n';

		return debug::warning();
	}

	template <class Fmt, class Arg0, class ... Args
	> int32_t warning(Fmt && fmt, Arg0 && arg0, Args && ... args) noexcept
	{
		return debug::warning(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_