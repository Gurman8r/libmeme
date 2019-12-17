#ifndef _ML_CONSOLE_HPP_
#define _ML_CONSOLE_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Core.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace console
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

		struct COL final
		{
			FG fg; BG bg;

			constexpr COL(FG fg, BG bg)
				: fg{ fg }, bg{ bg }
			{
			}

			constexpr COL(FG fg)
				: COL{ fg, BG::Black }
			{
			}

			constexpr COL(BG bg)
				: COL{ FG::Normal, bg }
			{
			}
			
			constexpr COL(COL const & copy)
				: COL{ copy.fg, copy.bg }
			{
			}

			constexpr COL(COL && copy) noexcept
				: COL{ std::move(copy.fg), std::move(copy.bg) }
			{
			}

			constexpr COL()
				: COL{ FG::Normal, BG::Black }
			{
			}

			constexpr uint16_t operator()() const
			{
				return ((this->fg != FG::None) && (this->bg != BG::None)
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr COL operator|(BG const & bg, FG const & fg) { return COL { fg, bg }; }
	
		constexpr COL operator|(FG const & fg, BG const & bg) { return COL { fg, bg }; }

		inline ML_SERIALIZE(std::ostream & out, COL const & value) { return value(out); }
	
		inline ML_SERIALIZE(std::ostream & out, FG const & value) { return out << COL { value }; }
	
		inline ML_SERIALIZE(std::ostream & out, BG const & value) { return out << COL { value }; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline std::ostream & endcol(std::ostream & out)
	{
		return out << console::COL{};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONSOLE_HPP_