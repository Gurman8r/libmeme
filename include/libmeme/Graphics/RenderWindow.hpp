#ifndef _ML_RENDER_WINDOW_HPP_
#define _ML_RENDER_WINDOW_HPP_

#include <libmeme/Window/Window.hpp>
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

		ML_NODISCARD auto get_render_context() const & noexcept -> shared<gfx::render_context> const & { return m_ctx; }

		ML_NODISCARD auto get_render_device() const & noexcept -> scoped<gfx::render_device> const & { return m_dev; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		shared<	gfx::render_context	>	m_ctx;
		scoped<	gfx::render_device	>	m_dev;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_WINDOW_HPP_