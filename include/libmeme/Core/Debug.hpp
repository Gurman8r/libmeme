#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <libmeme/Core/Console.hpp>
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
#		define ML_BREAKPOINT raise(SIGTRAP)
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

		static inline int32_t logger_impl(
			std::ostream & out,
			int32_t exitCode,
			cio::COL const & color,
			std::string const & prefix,
			std::string const & message
		)
		{
			out << cio::endcolor
				<< cio::FG::White << "[" << color << prefix << cio::FG::White << "] "
				<< cio::endcolor << message
				<< '\n';
			return exitCode;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Str> static inline int32_t logInfo(Str && value)
		{
			return logger_impl(std::cout, ML_SUCCESS, cio::FG::Green, ML_MSG_LOG, std::forward<Str>(value));
		}

		template <class Str> static inline int32_t logError(Str && value)
		{
			return logger_impl(std::cout, ML_FAILURE, cio::FG::Red, ML_MSG_ERR, std::forward<Str>(value));
		}

		template <class Str> static inline int32_t logWarning(Str && value)
		{
			return logger_impl(std::cout, ML_WARNING, cio::FG::Yellow, ML_MSG_WRN, std::forward<Str>(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Fmt, class Arg0, class ... Args
		> static inline int32_t logInfo(Fmt const & fmt, Arg0 const & arg0, Args && ... args)
		{
			return Debug::logInfo(util::format(fmt, arg0, std::forward<Args>(args)...));
		}

		template <
			class Fmt, class Arg0, class ... Args
		> static inline int32_t logError(Fmt const & fmt, Arg0 const & arg0, Args && ... args)
		{
			return Debug::logError(util::format(fmt, arg0, std::forward<Args>(args)...));
		}

		template <
			class Fmt, class Arg0, class ... Args
		> static inline int32_t logWarning(Fmt const & fmt, Arg0 const & arg0, Args && ... args)
		{
			return Debug::logWarning(util::format(fmt, arg0, std::forward<Args>(args)...));
		}
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_