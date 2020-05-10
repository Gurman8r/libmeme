#ifndef _ML_RENDERER_IMPL_HPP_
#define _ML_RENDERER_IMPL_HPP_

// WIP

#include <libmeme/Graphics/RendererAPI.hpp>

namespace ml
{
	// renderer implementation base
	struct renderer_impl : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~renderer_impl() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool is_initialized() const = 0;

		virtual bool initialize() = 0;

		virtual bool finalize() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDERER_IMPL_HPP_