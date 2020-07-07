#ifndef _ML_RENDER_COMMAND_HPP_
#define _ML_RENDER_COMMAND_HPP_

#include <libmeme/Graphics/RenderAPI.hpp>

namespace ml::gfx
{
	// command
	struct command final : public std::function<void(context *)>
	{
		using function::function;
	};

	// make command
	template <class Fn, class ... Args
	> ML_NODISCARD command make_command(Fn && fn, Args && ... args) noexcept
	{
		return std::bind(ML_forward(fn), std::placeholders::_1, ML_forward(args)...);
	}

	// builtin commands
	class render_command final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto set_alpha_enabled(bool enabled) noexcept
		{
			return make_command(&context::set_alpha_enabled, enabled);
		}

		ML_NODISCARD static auto set_alpha_mode(alpha_mode const & value) noexcept
		{
			return make_command(&context::set_alpha_mode, value);
		}

		ML_NODISCARD static auto set_blend_enabled(bool enabled) noexcept
		{
			return make_command(&context::set_blend_enabled, enabled);
		}
		
		ML_NODISCARD static auto set_blend_color(color const & value) noexcept
		{
			return make_command(&context::set_blend_color, value);
		}

		ML_NODISCARD static auto set_blend_mode(blend_mode const & value) noexcept
		{
			return make_command(&context::set_blend_mode, value);
		}

		ML_NODISCARD static auto set_clear_color(color const & value) noexcept
		{
			return make_command(&context::set_clear_color, value);
		}

		ML_NODISCARD static auto set_cull_enabled(bool enabled) noexcept
		{
			return make_command(&context::set_cull_enabled, enabled);
		}

		ML_NODISCARD static auto set_cull_mode(cull_mode const & value) noexcept
		{
			return make_command(&context::set_cull_mode, value);
		}

		ML_NODISCARD static auto set_depth_enabled(bool enabled) noexcept
		{
			return make_command(&context::set_depth_enabled, enabled);
		}

		ML_NODISCARD static auto set_depth_mode(depth_mode const & value) noexcept
		{
			return make_command(&context::set_depth_mode, value);
		}

		ML_NODISCARD static auto set_stencil_enabled(bool enabled) noexcept
		{
			return make_command(&context::set_stencil_enabled, enabled);
		}

		ML_NODISCARD static auto set_stencil_mode(stencil_mode const & value) noexcept
		{
			return make_command(&context::set_stencil_mode, value);
		}

		ML_NODISCARD static auto set_viewport(int_rect const & bounds) noexcept
		{
			return make_command(&context::set_viewport, bounds);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto clear(uint32_t mask) noexcept
		{
			return make_command(&context::clear, mask);
		}

		ML_NODISCARD static auto draw(shared<vertexarray> const & value) noexcept
		{
			return make_command(&context::draw, value);
		}

		ML_NODISCARD static auto draw_arrays(uint32_t prim, uint32_t first, size_t count) noexcept
		{
			return make_command(&context::draw_arrays, prim, first, count);
		}

		ML_NODISCARD static auto draw_indexed(uint32_t prim, size_t count) noexcept
		{
			return make_command(&context::draw_indexed, prim, count);
		}

		ML_NODISCARD static auto flush() noexcept
		{
			return make_command(&context::flush);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto bind_vertexarray(vertexarray const * value) noexcept
		{
			return make_command(&context::bind_vertexarray, value);
		}

		ML_NODISCARD static auto bind_vertexbuffer(vertexbuffer const * value) noexcept
		{
			return make_command(&context::bind_vertexbuffer, value);
		}

		ML_NODISCARD static auto bind_indexbuffer(indexbuffer const * value) noexcept
		{
			return make_command(&context::bind_indexbuffer, value);
		}

		ML_NODISCARD static auto bind_texture(texture const * value, uint32_t slot = 0) noexcept
		{
			return make_command(&context::bind_texture, value, slot);
		}

		ML_NODISCARD static auto bind_framebuffer(framebuffer const * value) noexcept
		{
			return make_command(&context::bind_framebuffer, value);
		}

		ML_NODISCARD static auto bind_program(program const * value) noexcept
		{
			return make_command(&context::bind_program, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD static auto upload(uniform_id loc, T value) noexcept
		{
			return make_command(static_cast<void(context:: *)(uniform_id, T)
			>(&context::upload), loc, value);
		}

		template <class T
		> ML_NODISCARD static auto upload(uniform_id loc, T const & value) noexcept
		{
			return make_command(static_cast<void(context:: *)(uniform_id, T const &)
			>(&context::upload), loc, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_COMMAND_HPP_