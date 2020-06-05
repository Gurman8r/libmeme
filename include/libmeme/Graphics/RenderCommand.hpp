#ifndef _ML_RENDER_COMMAND_HPP_
#define _ML_RENDER_COMMAND_HPP_

#include <libmeme/Graphics/RenderAPI.hpp>

namespace ml::gl
{
	// render command generators
	class command final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn, class ... Args
		> ML_NODISCARD static command_t builtin(Fn && fn, Args && ... args) noexcept
		{
			return std::bind(ML_forward(fn), device::get_context().get(), ML_forward(args)...);
		}

		template <class Fn, class ... Args
		> ML_NODISCARD static command_t custom(Fn && fn, Args && ... args) noexcept
		{
			return std::bind(ML_forward(fn), ML_forward(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto set_alpha_enabled(bool enabled) noexcept
		{
			return builtin(&device::set_alpha_enabled, enabled);
		}

		ML_NODISCARD static auto set_alpha_fn(alpha_fn const & value) noexcept
		{
			return builtin(&device::set_alpha_fn, value);
		}

		ML_NODISCARD static auto set_blend_enabled(bool enabled) noexcept
		{
			return builtin(&device::set_blend_enabled, enabled);
		}
		
		ML_NODISCARD static auto set_blend_color(color const & value) noexcept
		{
			return builtin(&device::set_blend_color, value);
		}

		ML_NODISCARD static auto set_blend_eq(blend_eq const & value) noexcept
		{
			return builtin(&device::set_blend_eq, value);
		}

		ML_NODISCARD static auto set_blend_fn(blend_fn const & value) noexcept
		{
			return builtin(&device::set_blend_fn, value);
		}

		ML_NODISCARD static auto set_clear_color(color const & value) noexcept
		{
			return builtin(&device::set_clear_color, value);
		}

		ML_NODISCARD static auto set_cull_enabled(bool enabled) noexcept
		{
			return builtin(&device::set_cull_enabled, enabled);
		}

		ML_NODISCARD static auto set_cull_facet(uint32_t facet) noexcept
		{
			return builtin(&device::set_cull_facet, facet);
		}

		ML_NODISCARD static auto set_cull_order(uint32_t order) noexcept
		{
			return builtin(&device::set_cull_order, order);
		}

		ML_NODISCARD static auto set_depth_enabled(bool enabled) noexcept
		{
			return builtin(&device::set_depth_enabled, enabled);
		}

		ML_NODISCARD static auto set_depth_mask(bool enabled) noexcept
		{
			return builtin(&device::set_depth_mask, enabled);
		}
		
		ML_NODISCARD static auto set_depth_pr(uint32_t predicate) noexcept
		{
			return builtin(&device::set_depth_pr, predicate);
		}

		ML_NODISCARD static auto set_depth_range(depth_range const & value) noexcept
		{
			return builtin(&device::set_depth_range, value);
		}

		ML_NODISCARD static auto set_stencil_enabled(bool enabled) noexcept
		{
			return builtin(&device::set_stencil_enabled, enabled);
		}

		ML_NODISCARD static auto set_stencil_fn(stencil_fn const & value) noexcept
		{
			return builtin(&device::set_stencil_fn, value);
		}

		ML_NODISCARD static auto set_viewport(int_rect const & bounds) noexcept
		{
			return builtin(&device::set_viewport, bounds);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto clear(uint32_t flags) noexcept
		{
			return builtin(&device::clear, flags);
		}

		ML_NODISCARD static auto draw(shared<vertexarray> const & value) noexcept
		{
			return builtin(&device::draw, value);
		}

		ML_NODISCARD static auto draw_arrays(uint32_t first, size_t count) noexcept
		{
			return builtin(&device::draw_arrays, first, count);
		}

		ML_NODISCARD static auto draw_indexed(size_t count) noexcept
		{
			return builtin(&device::draw_indexed, count);
		}

		ML_NODISCARD static auto flush() noexcept
		{
			return builtin(&device::flush);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD static auto upload(handle_t loc, T const & value) noexcept
		{
			return builtin(static_cast<void(device:: *)(handle_t, T const &)
			>(&device::upload), loc, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_COMMAND_HPP_