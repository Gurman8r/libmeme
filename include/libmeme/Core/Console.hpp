#ifndef _ML_CONSOLE_HPP_
#define _ML_CONSOLE_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Core.hpp>

namespace ml::cio
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class FG : uint16_t
	{
		Black		= 0,
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class BG : uint16_t
	{
		Black		= 0,
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API COL final
	{
		FG fg; BG bg;

		constexpr COL(FG fg, BG bg) noexcept : fg{ fg }, bg{ bg } {}

		constexpr COL(FG fg) noexcept : COL{ fg, BG::Black } {}

		constexpr COL(BG bg) noexcept : COL{ FG::Normal, bg } {}

		constexpr COL(COL const & other) noexcept : COL{ other.fg, other.bg } {}

		constexpr COL(COL && other) noexcept : COL{ std::move(other.fg), std::move(other.bg) } {}

		constexpr COL() noexcept : COL{ FG::Normal, BG::Black } {}

		constexpr uint16_t operator()() const noexcept
		{
			return ((fg != FG::None) && (bg != BG::None)
				? ((uint16_t)fg | (uint16_t)bg)
				: ((fg != FG::None)
					? ((uint16_t)fg)
					: ((bg != BG::None)
						? ((uint16_t)bg)
						: ((uint16_t)FG::Normal | (uint16_t)BG::Black)
						)));
		}

		std::ostream & operator()(std::ostream & out) const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr COL operator|(BG const & bg, FG const & fg) { return COL{ fg, bg }; }
	
	constexpr COL operator|(FG const & fg, BG const & bg) { return COL{ fg, bg }; }

	inline ML_SERIALIZE(std::ostream & out, COL const & col) { return col(out); }
	
	inline ML_SERIALIZE(std::ostream & out, FG const & fg) { return out << COL{ fg }; }
	
	inline ML_SERIALIZE(std::ostream & out, BG const & bg) { return out << COL{ bg }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline std::ostream & endcolor(std::ostream & out)
	{
		return out << cio::COL{};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONSOLE_HPP_