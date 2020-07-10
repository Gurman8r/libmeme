#ifndef _ML_RENDER_COMMAND_HPP_
#define _ML_RENDER_COMMAND_HPP_

#include <libmeme/Graphics/RenderAPI.hpp>

namespace ml::gfx
{
	// command
	struct command final : public std::function< void(render_context *) >
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

		ML_NODISCARD static auto set_alpha_state(alpha_state const & value) noexcept
		{
			return make_command(&render_context::set_alpha_state, value);
		}

		ML_NODISCARD static auto set_blend_state(blend_state const & value) noexcept
		{
			return make_command(&render_context::set_blend_state, value);
		}

		ML_NODISCARD static auto set_clear_color(color const & value) noexcept
		{
			return make_command(&render_context::set_clear_color, value);
		}

		ML_NODISCARD static auto set_cull_state(cull_state const & value) noexcept
		{
			return make_command(&render_context::set_cull_state, value);
		}

		ML_NODISCARD static auto set_depth_state(depth_state const & value) noexcept
		{
			return make_command(&render_context::set_depth_state, value);
		}

		ML_NODISCARD static auto set_stencil_state(stencil_state const & value) noexcept
		{
			return make_command(&render_context::set_stencil_state, value);
		}

		ML_NODISCARD static auto set_viewport(int_rect const & bounds) noexcept
		{
			return make_command(&render_context::set_viewport, bounds);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto clear(uint32_t mask) noexcept
		{
			return make_command(&render_context::clear, mask);
		}

		ML_NODISCARD static auto draw(shared<vertexarray> const & value) noexcept
		{
			return make_command(&render_context::draw, value);
		}

		ML_NODISCARD static auto draw_arrays(uint32_t mode, uint32_t first, size_t count) noexcept
		{
			return make_command(&render_context::draw_arrays, mode, first, count);
		}

		ML_NODISCARD static auto draw_indexed(uint32_t mode, size_t count) noexcept
		{
			return make_command(&render_context::draw_indexed, mode, count);
		}

		ML_NODISCARD static auto flush() noexcept
		{
			return make_command(&render_context::flush);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto bind_vertexarray(vertexarray const * value) noexcept
		{
			return make_command(&render_context::bind_vertexarray, value);
		}

		ML_NODISCARD static auto bind_vertexbuffer(vertexbuffer const * value) noexcept
		{
			return make_command(&render_context::bind_vertexbuffer, value);
		}

		ML_NODISCARD static auto bind_indexbuffer(indexbuffer const * value) noexcept
		{
			return make_command(&render_context::bind_indexbuffer, value);
		}

		ML_NODISCARD static auto bind_texture(texture const * value, uint32_t slot = 0) noexcept
		{
			return make_command(&render_context::bind_texture, value, slot);
		}

		ML_NODISCARD static auto bind_framebuffer(framebuffer const * value) noexcept
		{
			return make_command(&render_context::bind_framebuffer, value);
		}

		ML_NODISCARD static auto bind_program(program const * value) noexcept
		{
			return make_command(&render_context::bind_program, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD static auto upload(binding_id loc, T value) noexcept
		{
			return make_command(static_cast<void(render_context:: *)(binding_id, T)
			>(&render_context::upload), loc, value);
		}

		template <class T
		> ML_NODISCARD static auto upload(binding_id loc, T const & value) noexcept
		{
			return make_command(static_cast<void(render_context:: *)(binding_id, T const &)
			>(&render_context::upload), loc, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_COMMAND_HPP_