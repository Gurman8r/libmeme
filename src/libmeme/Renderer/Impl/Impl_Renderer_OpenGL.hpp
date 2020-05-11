#if defined(ML_IMPL_RENDERER_OPENGL)
#ifndef _ML_IMPL_RENDERER_OPENGL_HPP_
#define _ML_IMPL_RENDERER_OPENGL_HPP_

// WIP

#include <libmeme/Renderer/RendererImpl.hpp>

namespace ml
{
	struct impl_renderer_opengl final : renderer_impl
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		~impl_renderer_opengl() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool initialize();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static uint32_t get_error();

		static void check_error(cstring file, uint32_t line, cstring expr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool is_enabled(uint32_t value);

		static void enable(uint32_t value);

		static void disable(uint32_t value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool get_bool(uint32_t value);

		static int32_t get_int(uint32_t value);

		static float_t get_float(uint32_t value);

		static float64_t get_double(uint32_t value);

		static cstring get_string(uint32_t value);

		static cstring get_string(uint32_t value, uint32_t index);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_IMPL_RENDERER_OPENGL_HPP_
#endif // ML_IMPL_RENDERER_OPENGL