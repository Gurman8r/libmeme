#ifndef _ML_RENDERER_API_HPP_
#define _ML_RENDERER_API_HPP_

// WIP

#include <libmeme/Graphics/Export.hpp>
#include <libmeme/Core/Memory.hpp>

// renderer_api singleton
#define ML_renderer_api _ML renderer_api::get_instance()

namespace ml
{
	struct ML_GRAPHICS_API renderer_api final : singleton<renderer_api>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<renderer_api>;

		renderer_api() noexcept;

		~renderer_api() noexcept;

		struct renderer_impl * m_impl;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDERER_API_HPP_