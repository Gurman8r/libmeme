#ifndef _ML_RENDER_COMMAND_HPP_
#define _ML_RENDER_COMMAND_HPP_

#include <libmeme/Graphics/RenderAPI.hpp>

namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	// execute command
	template <class Cmd, class Ctx
	> static void execute(Cmd && cmd, Ctx && ctx) noexcept
	{
		using T = std::decay_t<decltype(ctx)>;
		if constexpr (std::is_same_v<T, shared<render_context>>)
		{
			std::invoke(ML_forward(cmd), ML_forward(ctx).get());
		}
		else if constexpr (std::is_convertible_v<T, render_context const *>)
		{
			std::invoke(ML_forward(cmd), ML_forward(ctx));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		template <class Value
		> ML_NODISCARD static auto bind_vertexarray(Value && value) noexcept
		{
			using T = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<T, shared<vertexarray>>)
			{
				return bind_framebuffer(ML_forward(value).get());
			}
			else if constexpr (std::is_convertible_v<T, vertexarray const *>)
			{
				return make_command(&render_context::bind_vertexarray, ML_forward(value));
			}
		}

		template <class Value
		> ML_NODISCARD static auto bind_vertexbuffer(Value && value) noexcept
		{
			using T = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<T, shared<vertexbuffer>>)
			{
				return bind_framebuffer(ML_forward(value).get());
			}
			else if constexpr (std::is_convertible_v<T, vertexbuffer const *>)
			{
				return make_command(&render_context::bind_vertexbuffer, ML_forward(value));
			}
		}

		template <class Value
		> ML_NODISCARD static auto bind_indexbuffer(Value && value) noexcept
		{
			using T = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<T, shared<indexbuffer>>)
			{
				return bind_framebuffer(ML_forward(value).get());
			}
			else if constexpr (std::is_convertible_v<T, indexbuffer const *>)
			{
				return make_command(&render_context::bind_indexbuffer, ML_forward(value));
			}
		}

		template <class Value
		> ML_NODISCARD static auto bind_texture(Value && value, uint32_t slot = 0) noexcept
		{
			using T = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<T, shared<texture>>)
			{
				return bind_framebuffer(ML_forward(value).get(), slot);
			}
			else if constexpr (std::is_convertible_v<T, texture const *>)
			{
				return make_command(&render_context::bind_texture, ML_forward(value), slot);
			}
		}

		template <class Value
		> ML_NODISCARD static auto bind_framebuffer(Value && value) noexcept
		{
			using T = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<T, shared<framebuffer>>)
			{
				return bind_framebuffer(ML_forward(value).get());
			}
			else if constexpr (std::is_convertible_v<T, framebuffer const *>)
			{
				return make_command(&render_context::bind_framebuffer, ML_forward(value));
			}
		}

		template <class Value
		> ML_NODISCARD static auto bind_program(Value && value) noexcept
		{
			using T = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<T, shared<program>>)
			{
				return bind_framebuffer(ML_forward(value).get());
			}
			else if constexpr (std::is_convertible_v<T, program const *>)
			{
				return make_command(&render_context::bind_program, ML_forward(value));
			}
		}

		template <class Value
		> ML_NODISCARD static auto bind_shader(Value && value) noexcept
		{
			using T = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<T, shared<shader>>)
			{
				return bind_framebuffer(ML_forward(value).get());
			}
			else if constexpr (std::is_convertible_v<T, shader const *>)
			{
				return make_command(&render_context::bind_shader, ML_forward(value));
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD static auto upload(uniform_id loc, T value) noexcept
		{
			return make_command(static_cast<void(render_context:: *)(uniform_id, T)
			>(&render_context::upload), loc, value);
		}

		template <class T
		> ML_NODISCARD static auto upload(uniform_id loc, T const & value) noexcept
		{
			return make_command(static_cast<void(render_context:: *)(uniform_id, T const &)
			>(&render_context::upload), loc, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_COMMAND_HPP_