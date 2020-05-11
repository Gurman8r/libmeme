#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

// WIP

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Renderer/RendererAPI.hpp>

// renderer singleton
#define ML_renderer \
	_ML renderer::get_instance()

namespace ml
{
	// global renderer
	struct ML_RENDERER_API renderer final : singleton<renderer>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool initialize();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<renderer>;

		renderer() noexcept;

		~renderer() noexcept;

		struct renderer_impl * m_impl;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDERER_HPP_