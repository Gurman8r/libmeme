#ifndef _ML_RENDER_COMMAND_HPP_
#define _ML_RENDER_COMMAND_HPP_

#include <libmeme/Graphics/RenderAPI.hpp>

namespace ml::gl
{
	// render command generators
	class render_command final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn, class ... Args
		> ML_NODISCARD static command builtin(Fn && fn, Args && ... args) noexcept
		{
			return std::bind(ML_forward(fn), render_api::get(), ML_forward(args)...);
		}

		template <class Fn, class ... Args
		> ML_NODISCARD static command custom(Fn && fn, Args && ... args) noexcept
		{
			return std::bind(ML_forward(fn), ML_forward(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto set_alpha_enabled(bool enabled) noexcept
		{
			return builtin(&render_api::set_alpha_enabled, enabled);
		}

		ML_NODISCARD static auto set_alpha_fn(alpha_fn const & value) noexcept
		{
			return builtin(&render_api::set_alpha_fn, value);
		}

		ML_NODISCARD static auto set_blend_enabled(bool enabled) noexcept
		{
			return builtin(&render_api::set_blend_enabled, enabled);
		}
		
		ML_NODISCARD static auto set_blend_color(color const & value) noexcept
		{
			return builtin(&render_api::set_blend_color, value);
		}

		ML_NODISCARD static auto set_blend_eq(blend_eq const & value) noexcept
		{
			return builtin(&render_api::set_blend_eq, value);
		}

		ML_NODISCARD static auto set_blend_fn(blend_fn const & value) noexcept
		{
			return builtin(&render_api::set_blend_fn, value);
		}

		ML_NODISCARD static auto set_clear_color(color const & value) noexcept
		{
			return builtin(&render_api::set_clear_color, value);
		}

		ML_NODISCARD static auto set_cull_enabled(bool enabled) noexcept
		{
			return builtin(&render_api::set_cull_enabled, enabled);
		}

		ML_NODISCARD static auto set_cull_facet(uint32_t facet) noexcept
		{
			return builtin(&render_api::set_cull_facet, facet);
		}

		ML_NODISCARD static auto set_cull_order(uint32_t order) noexcept
		{
			return builtin(&render_api::set_cull_order, order);
		}

		ML_NODISCARD static auto set_depth_enabled(bool enabled) noexcept
		{
			return builtin(&render_api::set_depth_enabled, enabled);
		}

		ML_NODISCARD static auto set_depth_mask(bool enabled) noexcept
		{
			return builtin(&render_api::set_depth_mask, enabled);
		}
		
		ML_NODISCARD static auto set_depth_pr(uint32_t predicate) noexcept
		{
			return builtin(&render_api::set_depth_pr, predicate);
		}

		ML_NODISCARD static auto set_depth_range(depth_range const & value) noexcept
		{
			return builtin(&render_api::set_depth_range, value);
		}

		ML_NODISCARD static auto set_stencil_enabled(bool enabled) noexcept
		{
			return builtin(&render_api::set_stencil_enabled, enabled);
		}

		ML_NODISCARD static auto set_stencil_fn(stencil_fn const & value) noexcept
		{
			return builtin(&render_api::set_stencil_fn, value);
		}

		ML_NODISCARD static auto set_viewport(int_rect const & bounds) noexcept
		{
			return builtin(&render_api::set_viewport, bounds);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto clear(uint32_t flags) noexcept
		{
			return builtin(&render_api::clear, flags);
		}

		ML_NODISCARD static auto draw(shared<vertexarray> const & value) noexcept
		{
			return builtin(&render_api::draw, value);
		}

		ML_NODISCARD static auto draw_arrays(uint32_t first, uint32_t count) noexcept
		{
			return builtin(&render_api::draw_arrays, first, count);
		}

		ML_NODISCARD static auto draw_indexed(uint32_t count) noexcept
		{
			return builtin(&render_api::draw_indexed, count);
		}

		ML_NODISCARD static auto flush() noexcept
		{
			return builtin(&render_api::flush);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD static auto upload(handle loc, T const & value) noexcept
		{
			return builtin(static_cast<void(render_api::*)(handle, T const &)
			>(&render_api::upload), loc, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_COMMAND_HPP_