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

		static void exit(int32_t exitCode);
		
		static int32_t clear();
		
		static int32_t pause(int32_t exitCode);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void * execute(
			std::string const & command,
			std::string const & file = {},
			std::string const & args = {},
			std::string const & path = {},
			int32_t flags = 5
		);

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