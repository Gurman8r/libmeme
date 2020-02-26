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

		ML_NODISCARD static inline std::ostream & out() noexcept { return std::cout; }

		ML_NODISCARD static inline std::ostream & err() noexcept { return std::cerr; }

		ML_NODISCARD static inline std::istream & in() noexcept { return std::cin; }

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
			debug::in().get();
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
				out() << "[" ML_MSG_LOG "] " << ML_FWD(value) << "\n";
				return ML_SUCCESS;
			}

			template <class Str
			> static inline int32_t error(Str && value)
			{
				out() << "[" ML_MSG_ERR "] " << ML_FWD(value) << "\n";
				return ML_FAILURE;
			}

			template <class Str
			> static inline int32_t warning(Str && value)
			{
				out() << "[" ML_MSG_WRN "] " << ML_FWD(value) << "\n";
				return ML_WARNING;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <class Fmt, class Arg0, class ... Args
			> static inline int32_t info(Fmt && fmt, Arg0 && arg0, Args && ... args)
			{
				return info(util::format(ML_FWD(fmt), ML_FWD(arg0), ML_FWD(args)...));
			}

			template <class Fmt, class Arg0, class ... Args
			> static inline int32_t error(Fmt && fmt, Arg0 && arg0, Args && ... args)
			{
				return error(util::format(ML_FWD(fmt), ML_FWD(arg0), ML_FWD(args)...));
			}

			template <class Fmt, class Arg0, class ... Args
			> static inline int32_t warning(Fmt && fmt, Arg0 && arg0, Args && ... args)
			{
				return warning(util::format(ML_FWD(fmt), ML_FWD(arg0), ML_FWD(args)...));
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_