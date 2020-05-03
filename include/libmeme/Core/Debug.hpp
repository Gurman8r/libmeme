#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <libmeme/Core/StringUtility.hpp>

// exit codes
#define ML_WARNING	(-1)
#define ML_FAILURE	( 0)
#define ML_SUCCESS	(+1)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::debug
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline std::ostream & out() noexcept { return std::cout; }

	inline std::ostream & err() noexcept { return std::cerr; }

	inline std::istream & in() noexcept { return std::cin; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline int32_t clear(int32_t const exit_code = {})
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

	inline int32_t exit(int32_t const exit_code = {})
	{
		std::exit(exit_code);

		return exit_code;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline int32_t pause(int32_t const exit_code = {})
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

	namespace log
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fmt
		> inline int32_t info(Fmt && fmt)
		{
			out() << "[ info ] " << ML_forward(fmt) << '\n';
			return ML_SUCCESS;
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
			out() << "[ error ] " << ML_forward(fmt) << '\n';
			return ML_FAILURE;
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
			out() << "[ warn ] " << ML_forward(fmt) << '\n';
			return ML_WARNING;
		}

		template <class Fmt, class Arg0, class ... Args
		> inline int32_t warning(Fmt && fmt, Arg0 && arg0, Args && ... args)
		{
			return warning(util::format(ML_forward(fmt), ML_forward(arg0), ML_forward(args)...));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_