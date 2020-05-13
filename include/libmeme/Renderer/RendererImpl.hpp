#ifndef _ML_RENDERER_IMPL_HPP_
#define _ML_RENDERER_IMPL_HPP_

// WIP

#include <libmeme/Renderer/RendererAPI.hpp>

namespace ml
{
	// renderer implementation base
	struct renderer_base : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~renderer_base() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t get_enum(size_t const i) const noexcept { return get_enum_table()[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		virtual gl_enum_table const & get_enum_table() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDERER_IMPL_HPP_