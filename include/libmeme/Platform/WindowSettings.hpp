#ifndef _ML_WINDOW_SETTINGS_HPP_
#define _ML_WINDOW_SETTINGS_HPP_

#include <libmeme/Core/BitSet.hpp>
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
			
		static constexpr auto Flags{ enum_info<Flag>
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

		constexpr WindowSettings() noexcept
			: m_data{ 0 }
		{
		}

		constexpr explicit WindowSettings(
			bool resizable, bool visible, bool decorated, bool focused,
			bool autoIconify, bool floating, bool maximized, 
			bool fullscreen, bool vertical_sync
		) : m_data{ {
			resizable, visible, decorated, focused, 
			autoIconify, floating, maximized, 
			fullscreen, vertical_sync
		} }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit WindowSettings(storage_type const & data)
			: m_data{ data }
		{
		}

		constexpr explicit WindowSettings(storage_type && data) noexcept
			: m_data{ std::move(data) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr WindowSettings(WindowSettings const & copy)
			: m_data{ copy.m_data }
		{
		}

		constexpr WindowSettings(WindowSettings && copy) noexcept
			: m_data{ std::move(copy.m_data) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline WindowSettings & operator=(WindowSettings const & other)
		{
			return (*this);
		}

		inline WindowSettings & operator=(WindowSettings && other) noexcept
		{
			return (*this);
		}

		inline void swap(WindowSettings & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_data, other.m_data);
			}
		}

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD constexpr auto make_window_settings(Args && ... args)
	{
		return WindowSettings{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_SETTINGS_HPP_