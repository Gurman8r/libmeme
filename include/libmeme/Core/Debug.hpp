#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/StringUtility.hpp>

# ifndef ML_ASSERT
#	define ML_ASSERT(expr) assert(expr)
#endif

#define ML_WARNING	-1
#define ML_FAILURE	 0
#define ML_SUCCESS	+1

#define ML_MSG_LOG "info"
#define ML_MSG_WRN "warn"
#define ML_MSG_ERR "error"

#if (ML_DEBUG)
#	if defined(ML_IMPL_BREAKPOINT_CUSTOM)
#		define ML_BREAKPOINT ML_IMPL_BREAKPOINT_CUSTOM
#	elif defined(ML_CC_MSC)
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

	struct ML_CORE_API Debug final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Debug() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline int32_t clear()
		{
#if (ML_DEBUG)
#	ifdef ML_SYSTEM_WINDOWS
			return std::system("cls");
#	else
			return std::system("clear");
#	endif
#else
			return 0;
#endif
		}

		static inline int32_t pause(int32_t exitCode)
		{
#if (ML_DEBUG)
#	ifdef ML_SYSTEM_WINDOWS
			std::system("pause");
#	else
			std::cin.get();
#	endif
#endif
			return exitCode;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Str
		> static inline int32_t log_info(Str && value)
		{
			std::cout << "[" << ML_MSG_LOG << "] " << std::forward<Str>(value) << "\n";
			return ML_SUCCESS;
		}

		template <class Str
		> static inline int32_t log_error(Str && value)
		{
			std::cout << "[" << ML_MSG_ERR << "] " << std::forward<Str>(value) << "\n";
			return ML_FAILURE;
		}

		template <class Str
		> static inline int32_t log_warning(Str && value)
		{
			std::cout << "[" << ML_MSG_WRN << "] " << std::forward<Str>(value) << "\n";
			return ML_WARNING;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fmt, class Arg0, class ... Args
		> static inline int32_t log_info(Fmt const & fmt, Arg0 const & arg0, Args && ... args)
		{
			return Debug::log_info(util::format(fmt, arg0, std::forward<Args>(args)...));
		}

		template <class Fmt, class Arg0, class ... Args
		> static inline int32_t log_error(Fmt const & fmt, Arg0 const & arg0, Args && ... args)
		{
			return Debug::log_error(util::format(fmt, arg0, std::forward<Args>(args)...));
		}

		template <class Fmt, class Arg0, class ... Args
		> static inline int32_t log_warning(Fmt const & fmt, Arg0 const & arg0, Args && ... args)
		{
			return Debug::log_warning(util::format(fmt, arg0, std::forward<Args>(args)...));
		}
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_