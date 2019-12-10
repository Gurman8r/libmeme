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
#		define ML_BREAKPOINT raise(SIGTRAP)
#	endif
#else
#	define ML_BREAKPOINT
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	enum class FG : uint16_t
	{
		Black,
		DarkBlue	= (1 << 0),
		DarkGreen	= (1 << 1),
		DarkCyan	= DarkGreen | DarkBlue,
		DarkRed		= (1 << 2),
		DarkMagenta = DarkRed | DarkBlue,
		DarkYellow	= DarkRed | DarkGreen,
		Normal		= DarkRed | DarkGreen | DarkBlue,
		Gray		= (1 << 3),
		Blue		= Gray | DarkBlue,
		Green		= Gray | DarkGreen,
		Cyan		= Gray | DarkGreen | DarkBlue,
		Red			= Gray | DarkRed,
		Magenta		= Gray | DarkRed | DarkBlue,
		Yellow		= Gray | DarkRed | DarkGreen,
		White		= Gray | DarkRed | DarkGreen | DarkBlue,

		None = static_cast<uint16_t>(-1)
	};

	/* * * * * * * * * * * * * * * * * * * * */

	enum class BG : uint16_t
	{
		Black,
		DarkBlue	= (1 << 4),
		DarkGreen	= (1 << 5),
		DarkCyan	= DarkGreen | DarkBlue,
		DarkRed		= (1 << 6),
		DarkMagenta = DarkRed | DarkBlue,
		DarkYellow	= DarkRed | DarkGreen,
		Gray		= DarkRed | DarkGreen | DarkBlue,
		DarkGray	= (1 << 7),
		Blue		= DarkGray | DarkBlue,
		Green		= DarkGray | DarkGreen,
		Cyan		= DarkGray | DarkGreen | DarkBlue,
		Red			= DarkGray | DarkRed,
		Magenta		= DarkGray | DarkRed | DarkBlue,
		Yellow		= DarkGray | DarkRed | DarkGreen,
		White		= DarkGray | DarkRed | DarkGreen | DarkBlue,
		
		None = static_cast<uint16_t>(-1)
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API COL final
	{
		FG fg; BG bg;

		constexpr COL() : COL{ FG::Normal, BG::Black } {}
		constexpr COL(FG fg) : COL{ fg, BG::Black } {}
		constexpr COL(BG bg) : COL{ FG::Normal, bg } {}
		constexpr COL(COL const & copy) : COL{ copy.fg, copy.bg } {}
		constexpr COL(FG fg, BG bg) : fg{ fg }, bg{ bg } {}

		constexpr uint16_t operator*() const
		{
			return (((this->fg != FG::None) && (this->bg != BG::None))
				? ((uint16_t)this->fg | (uint16_t)this->bg)
				: ((this->fg != FG::None)
					? ((uint16_t)this->fg) 
					: ((this->bg != BG::None)
						? ((uint16_t)this->bg) 
						: ((uint16_t)FG::Normal | (uint16_t)BG::Black)
						)));
		}

		std::ostream & operator()(std::ostream & out) const;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr COL operator|(BG const & bg, FG const & fg) { return COL { fg, bg }; }
	
	constexpr COL operator|(FG const & fg, BG const & bg) { return COL { fg, bg }; }

	inline ML_SERIALIZE(std::ostream & out, COL const & value) { return value(out); }
	
	inline ML_SERIALIZE(std::ostream & out, FG const & value) { return out << COL { value }; }
	
	inline ML_SERIALIZE(std::ostream & out, BG const & value) { return out << COL { value }; }

	/* * * * * * * * * * * * * * * * * * * * */

	static inline std::ostream & endcol(std::ostream & out)
	{
		return out << COL{};
	}

	/* * * * * * * * * * * * * * * * * * * * */

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
			COL const & color,
			std::string const & prefix,
			std::string const & message
		)
		{
			out << COL()
				<< FG::White << "[" << color << prefix << FG::White << "] "
				<< COL() << message
				<< '\n';
			return exitCode;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline int32_t logInfo(std::string const & message)
		{
			return logger_impl(std::cout, ML_SUCCESS, FG::Green, ML_MSG_LOG, message);
		}

		static inline int32_t logError(std::string const & message)
		{
			return logger_impl(std::cout, ML_FAILURE, FG::Red, ML_MSG_ERR, message);
		}

		static inline int32_t logWarning(std::string const & message)
		{
			return logger_impl(std::cout, ML_WARNING, FG::Yellow, ML_MSG_WRN, message);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class ... Args
		> static inline int32_t logInfo(std::string const & fmt, T const & arg0, Args && ... args)
		{
			return Debug::logInfo(util::format(fmt, arg0, std::forward<Args>(args)...));
		}

		template <
			class T, class ... Args
		> static inline int32_t logError(std::string const & fmt, T const & arg0, Args && ... args)
		{
			return Debug::logError(util::format(fmt, arg0, std::forward<Args>(args)...));
		}

		template <
			class T, class ... Args
		> static inline int32_t logWarning(std::string const & fmt, T const & arg0, Args && ... args)
		{
			return Debug::logWarning(util::format(fmt, arg0, std::forward<Args>(args)...));
		}
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_