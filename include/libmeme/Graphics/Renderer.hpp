#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

// WIP

#include <libmeme/Graphics/Export.hpp>
#include <libmeme/Graphics/RendererAPI.hpp>

// renderer singleton
#define ML_renderer_api \
	_ML renderer::get_instance()

namespace ml
{
	// global renderer implementation
	struct ML_GRAPHICS_API renderer final : singleton<renderer>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool is_initialized() const;

		bool initialize();

		bool finalize();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



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