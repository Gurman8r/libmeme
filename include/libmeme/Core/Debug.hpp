#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <libmeme/Core/StringUtility.hpp>

#define ML_WARNING	-1
#define ML_FAILURE	 0
#define ML_SUCCESS	+1

#define ML_MSG_LOG "info"
#define ML_MSG_WRN "warn"
#define ML_MSG_ERR "error"

#if ML_DEBUG
#	if defined(ML_IMPL_BREAKPOINT_CUSTOM)
#		define ML_BREAKPOINT ML_IMPL_BREAKPOINT_CUSTOM
#	elif defined(ML_CC_MSVC)
#		define ML_BREAKPOINT ::__debugbreak()
#	else
#		define ML_BREAKPOINT ::raise(SIGTRAP)
#	endif
#else
#	define ML_BREAKPOINT
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace debug
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline int32_t clear()
		{
#if ML_DEBUG
#	ifdef ML_OS_WINDOWS
			return std::system("cls");
#	else
			return std::system("clear");
#	endif
#endif
			return 0;
		}

		static inline int32_t pause(int32_t exit_code)
		{
#if ML_DEBUG
#	ifdef ML_OS_WINDOWS
			std::system("pause");
#	else
			std::cin.get();
#	endif
#endif
			return exit_code;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Str
		> static inline int32_t log_info(Str && value)
		{
			std::cout << "[" << ML_MSG_LOG << "] " << ML_FWD(value) << "\n";
			return ML_SUCCESS;
		}

		template <class Str
		> static inline int32_t log_error(Str && value)
		{
			std::cout << "[" << ML_MSG_ERR << "] " << ML_FWD(value) << "\n";
			return ML_FAILURE;
		}

		template <class Str
		> static inline int32_t log_warning(Str && value)
		{
			std::cout << "[" << ML_MSG_WRN << "] " << ML_FWD(value) << "\n";
			return ML_WARNING;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fmt, class Arg0, class ... Args
		> static inline int32_t log_info(Fmt const & fmt, Arg0 const & arg0, Args && ... args)
		{
			return debug::log_info(util::format(fmt, arg0, ML_FWD(args)...));
		}

		template <class Fmt, class Arg0, class ... Args
		> static inline int32_t log_error(Fmt const & fmt, Arg0 const & arg0, Args && ... args)
		{
			return debug::log_error(util::format(fmt, arg0, ML_FWD(args)...));
		}

		template <class Fmt, class Arg0, class ... Args
		> static inline int32_t log_warning(Fmt const & fmt, Arg0 const & arg0, Args && ... args)
		{
			return debug::log_warning(util::format(fmt, arg0, ML_FWD(args)...));
		}
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_