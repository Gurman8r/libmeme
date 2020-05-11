#ifndef _ML_RENDERER_IMPL_HPP_
#define _ML_RENDERER_IMPL_HPP_

// WIP

#include <libmeme/Renderer/RendererAPI.hpp>

namespace ml
{
	// renderer implementation base
	struct renderer_impl : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~renderer_impl() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		static ds::array<uint32_t, gl_MAX_ENUM> const g_enum_table; // global enum table

		ML_NODISCARD static auto get_enum(size_t const i) noexcept { return g_enum_table[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDERER_IMPL_HPP_