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

		explicit render_window(window_settings const & ws) noexcept;

		virtual ~render_window() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool open(window_settings const & ws) override;

		virtual void close() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_device() const & noexcept -> unique<gfx::device> const & { return m_device; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		unique<gfx::device> m_device;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_WINDOW_HPP_