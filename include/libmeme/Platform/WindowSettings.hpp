#ifndef _ML_WINDOW_SETTINGS_HPP_
#define _ML_WINDOW_SETTINGS_HPP_

#include <libmeme/Core/BitSet.hpp>

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit WindowSettings(
			bool resizable, bool visible, bool decorated, bool focused,
			bool autoIconify, bool floating, bool maximized, 
			bool fullscreen, bool vertical_sync
		) noexcept : m_storage{ {
			resizable, visible, decorated, focused, 
			autoIconify, floating, maximized, 
			fullscreen, vertical_sync
		} }
		{
		}

		constexpr WindowSettings(WindowSettings const & copy) noexcept
			: m_storage{ copy.m_storage }
		{
		}

		constexpr WindowSettings() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto data() const -> storage_type const & { return m_storage; }
		
		ML_NODISCARD constexpr auto data() -> storage_type & { return m_storage; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr bool resizable() const { return m_storage[Flag::Resizable]; }
		
		ML_NODISCARD constexpr bool visible() const { return m_storage[Flag::Visible]; }
		
		ML_NODISCARD constexpr bool decorated() const { return m_storage[Flag::Decorated]; }
		
		ML_NODISCARD constexpr bool focused() const { return m_storage[Flag::Focused]; }
		
		ML_NODISCARD constexpr bool autoIconify() const { return m_storage[Flag::AutoIconify]; }
		
		ML_NODISCARD constexpr bool floating() const { return m_storage[Flag::Floating]; }
		
		ML_NODISCARD constexpr bool maximized() const { return m_storage[Flag::Maximized]; }
		
		ML_NODISCARD constexpr bool fullscreen() const { return m_storage[Flag::Fullscreen]; }
		
		ML_NODISCARD constexpr bool vsync() const { return m_storage[Flag::VerticalSync]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_window_settings(Args && ... args)
	{
		return WindowSettings{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_SETTINGS_HPP_