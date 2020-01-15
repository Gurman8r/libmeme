#ifndef _ML_WINDOW_SETTINGS_HPP_
#define _ML_WINDOW_SETTINGS_HPP_

#include <libmeme/Core/BitMask.hpp>
#include <libmeme/Core/EnumInfo.hpp>

namespace ml
{
	struct WindowSettings final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_type = typename mask16_t;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Flag : uint32_t
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
			
		static constexpr auto flag_info{ enum_info<Flag>
		{ {
			Flag::Resizable, Flag::Visible, Flag::Decorated, Flag::Focused,
			Flag::AutoIconify, Flag::Floating, Flag::Maximized,
			Flag::Fullscreen, Flag::VerticalSync,
		}, {
			"Resizable", "Visible", "Decorated", "Focused",
			"AutoIconify", "Floating", "Maximized",
			"Fullscreen", "Vertical Sync",
		}, {
			"Resizable", "Visible", "Decorated", "Focused",
			"Auto Iconify", "Floating", "Maximized",
			"Fullscreen", "Vertical Sync",
		} } };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit WindowSettings(
			bool resizable, bool visible, bool decorated, bool focused,
			bool autoIconify, bool floating, bool maximized, 
			bool fullscreen, bool vertical_sync
		) : WindowSettings{ storage_type { {
			resizable, visible, decorated, focused, 
			autoIconify, floating, maximized, 
			fullscreen, vertical_sync
		} } }
		{
		}

		constexpr WindowSettings(WindowSettings const & copy)
			: WindowSettings{ copy.m_data }
		{
		}

		constexpr WindowSettings(storage_type const & data)
			: m_data{ data }
		{
		}

		constexpr WindowSettings() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto data() const -> storage_type const & { return m_data; }
		
		ML_NODISCARD constexpr auto data() -> storage_type & { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr bool resizable() const { return m_data[Flag::Resizable]; }
		
		ML_NODISCARD constexpr bool visible() const { return m_data[Flag::Visible]; }
		
		ML_NODISCARD constexpr bool decorated() const { return m_data[Flag::Decorated]; }
		
		ML_NODISCARD constexpr bool focused() const { return m_data[Flag::Focused]; }
		
		ML_NODISCARD constexpr bool autoIconify() const { return m_data[Flag::AutoIconify]; }
		
		ML_NODISCARD constexpr bool floating() const { return m_data[Flag::Floating]; }
		
		ML_NODISCARD constexpr bool maximized() const { return m_data[Flag::Maximized]; }
		
		ML_NODISCARD constexpr bool fullscreen() const { return m_data[Flag::Fullscreen]; }
		
		ML_NODISCARD constexpr bool vsync() const { return m_data[Flag::VerticalSync]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_data{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_WINDOW_SETTINGS_HPP_