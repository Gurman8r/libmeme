#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <libmeme/Core/StringUtility.hpp>

#define ML_WARNING	-1
#define ML_FAILURE	 0
#define ML_SUCCESS	+1

#define ML_MSG_LOG "info"
#define ML_MSG_WRN "warn"
#define ML_MSG_ERR "error"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace debug
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline int32_t clear(int32_t const exit_code = 0)
		{
#if ML_DEBUG
#	ifdef ML_OS_WINDOWS
			return std::system("cls");
#	else
			return std::system("clear");
#	endif
#endif
			return exit_code;
		}

		static inline void exit(int32_t const exit_code = 0)
		{
			return std::exit(exit_code);
		}

		static inline int32_t pause(int32_t const exit_code = 0)
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

		namespace log
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <class Str
			> static inline int32_t info(Str && value)
			{
				std::cout << "[" ML_MSG_LOG "] " << ML_fwd(value) << "\n";
				return ML_SUCCESS;
			}

			template <class Str
			> static inline int32_t error(Str && value)
			{
				std::cout << "[" ML_MSG_ERR "] " << ML_fwd(value) << "\n";
				return ML_FAILURE;
			}

			template <class Str
			> static inline int32_t warning(Str && value)
			{
				std::cout << "[" ML_MSG_WRN "] " << ML_fwd(value) << "\n";
				return ML_WARNING;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <class Fmt, class Arg0, class ... Args
			> static inline int32_t info(Fmt && fmt, Arg0 && arg0, Args && ... args)
			{
				return info(util::format(ML_fwd(fmt), ML_fwd(arg0), ML_fwd(args)...));
			}

			template <class Fmt, class Arg0, class ... Args
			> static inline int32_t error(Fmt && fmt, Arg0 && arg0, Args && ... args)
			{
				return error(util::format(ML_fwd(fmt), ML_fwd(arg0), ML_fwd(args)...));
			}

			template <class Fmt, class Arg0, class ... Args
			> static inline int32_t warning(Fmt && fmt, Arg0 && arg0, Args && ... args)
			{
				return warning(util::format(ML_fwd(fmt), ML_fwd(arg0), ML_fwd(args)...));
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_