#ifndef _ML_RENDER_WINDOW_HPP_
#define _ML_RENDER_WINDOW_HPP_

#include <libmeme/Platform/Window.hpp>
#include <libmeme/Graphics/RenderAPI.hpp>

namespace ml
{
	struct ML_GRAPHICS_API render_window : window
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		render_window() noexcept;

		explicit render_window(window_settings const & ws, bool install_callbacks = true) noexcept;

		virtual ~render_window() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual bool open(window_settings const & ws, bool install_callbacks = true) override;

		virtual void close() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_devctx() const noexcept -> gfx::device * { return m_device; }

		ML_NODISCARD auto get_devinfo() const noexcept -> gfx::devinfo const & { return m_device->get_devinfo(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		gfx::device * m_device;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_WINDOW_HPP_