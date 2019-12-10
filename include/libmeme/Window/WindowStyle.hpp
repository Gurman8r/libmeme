#ifndef _ML_WINDOW_STYLE_HPP_
#define _ML_WINDOW_STYLE_HPP_

#include <libmeme/Window/Export.hpp>
#include <libmeme/Core/BitMask.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowStyle final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename mask16_t;
		using self_type = typename WindowStyle;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Flag : size_t
		{
			Resizable,
			Visible,
			Decorated,
			Focused,
			AutoIconify,
			Floating,
			Maximized,
			Fullscreen,
			VerticalSync,
		};

		static constexpr Flag Flag_values[] = {
			Flag::Resizable,
			Flag::Visible,
			Flag::Decorated,
			Flag::Focused,
			Flag::AutoIconify,
			Flag::Floating,
			Flag::Maximized,
			Flag::Fullscreen,
			Flag::VerticalSync,
		};

		static constexpr C_String Flag_names[] = {
			"Resizable",
			"Visible",
			"Decorated",
			"Focused",
			"AutoIconify",
			"Floating",
			"Maximized",
			"Fullscreen",
			"VerticalSync",
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr WindowStyle()
			: self_type{ base_type{ 0 } }
		{
		}

		constexpr WindowStyle(
			bool resizable,
			bool visible,
			bool decorated,
			bool focused,
			bool autoIconify,
			bool floating,
			bool maximized, 
			bool fullscreen,
			bool vertical_sync
		) : self_type{ base_type { {
			resizable, 
			visible, 
			decorated, 
			focused, 
			autoIconify, 
			floating, 
			maximized, 
			fullscreen, 
			vertical_sync
		} } }
		{
		}

		constexpr WindowStyle(WindowStyle const & copy)
			: self_type{ copy.m_data }
		{
		}

		constexpr WindowStyle(base_type const & data)
			: m_data{ data }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto data() const -> base_type const & { return m_data; }
		
		constexpr auto data() -> base_type & { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bool resizable()		const { return m_data[Flag::Resizable]; }
		constexpr bool visible()		const { return m_data[Flag::Visible]; }
		constexpr bool decorated()		const { return m_data[Flag::Decorated]; }
		constexpr bool focused()		const { return m_data[Flag::Focused]; }
		constexpr bool autoIconify()	const { return m_data[Flag::AutoIconify]; }
		constexpr bool floating()		const { return m_data[Flag::Floating]; }
		constexpr bool maximized()		const { return m_data[Flag::Maximized]; }
		constexpr bool fullscreen()		const { return m_data[Flag::Fullscreen]; }
		constexpr bool vertical_sync()	const { return m_data[Flag::VerticalSync]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: base_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_STYLE_HPP_