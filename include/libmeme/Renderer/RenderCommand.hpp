#ifndef _ML_RENDER_COMMAND_HPP_
#define _ML_RENDER_COMMAND_HPP_

#include <libmeme/Renderer/RenderAPI.hpp>

// render commands

namespace ml::gl
{
	class render_command final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static command_t set_alpha_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_alpha_enabled, render_api::get(), enabled);
		}

		ML_NODISCARD static command_t set_alpha_function(alpha_function const & value) noexcept
		{
			return std::bind(&render_api::set_alpha_function, render_api::get(), value);
		}

		ML_NODISCARD static command_t set_blend_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_blend_enabled, render_api::get(), enabled);
		}
		
		ML_NODISCARD static command_t set_blend_color(color const & value) noexcept
		{
			return std::bind(&render_api::set_blend_color, render_api::get(), value);
		}

		ML_NODISCARD static command_t set_blend_equation(blend_equation const & value) noexcept
		{
			return std::bind(&render_api::set_blend_equation, render_api::get(), value);
		}

		ML_NODISCARD static command_t set_blend_function(blend_function const & value) noexcept
		{
			return std::bind(&render_api::set_blend_function, render_api::get(), value);
		}

		ML_NODISCARD static command_t set_clear_color(color const & value) noexcept
		{
			return std::bind(&render_api::set_clear_color, render_api::get(), value);
		}

		ML_NODISCARD static command_t set_cull_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_cull_enabled, render_api::get(), enabled);
		}

		ML_NODISCARD static command_t set_cull_face(uint32_t facet) noexcept
		{
			return std::bind(&render_api::set_cull_face, render_api::get(), facet);
		}

		ML_NODISCARD static command_t set_cull_order(uint32_t order) noexcept
		{
			return std::bind(&render_api::set_cull_order, render_api::get(), order);
		}

		ML_NODISCARD static command_t set_depth_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_depth_enabled, render_api::get(), enabled);
		}

		ML_NODISCARD static command_t set_depth_function(uint32_t predicate) noexcept
		{
			return std::bind(&render_api::set_depth_function, render_api::get(), predicate);
		}
		
		ML_NODISCARD static command_t set_depth_mask(bool enabled) noexcept
		{
			return std::bind(&render_api::set_depth_mask, render_api::get(), enabled);
		}

		ML_NODISCARD static command_t set_depth_range(depth_range const & value) noexcept
		{
			return std::bind(&render_api::set_depth_range, render_api::get(), value);
		}

		ML_NODISCARD static command_t set_viewport(int_rect const & bounds) noexcept
		{
			return std::bind(&render_api::set_viewport, render_api::get(), bounds);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static command_t clear(uint32_t flags) noexcept
		{
			return std::bind(&render_api::clear, render_api::get(), flags);
		}

		ML_NODISCARD static command_t draw(vao_t const & value) noexcept
		{
			return std::bind(&render_api::draw, render_api::get(), value);
		}

		ML_NODISCARD static command_t draw_arrays(uint32_t first, uint32_t count) noexcept
		{
			return std::bind(&render_api::draw_arrays, render_api::get(), first, count);
		}

		ML_NODISCARD static command_t draw_indexed(uint32_t count) noexcept
		{
			return std::bind(&render_api::draw_indexed, render_api::get(), count);
		}

		ML_NODISCARD static command_t flush() noexcept
		{
			return std::bind(&render_api::flush, render_api::get());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_COMMAND_HPP_