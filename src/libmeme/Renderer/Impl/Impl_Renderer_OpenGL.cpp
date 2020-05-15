#if defined(ML_IMPL_RENDERER_OPENGL)

#include "Impl_Renderer_OpenGL.hpp"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_renderer_api::initialize()
	{
#if defined(ML_IMPL_OPENGL_LOADER_GLEW)
		glewExperimental = true;
		return (GLEW_OK == glewInit());

#elif defined(ML_IMPL_OPENGL_LOADER_GL3W)
		return gl3wInit();

#elif defined(ML_IMPL_OPENGL_LOADER_GLAD)
		return gladLoadGL();

#elif defined(ML_IMPL_OPENGL_LOADER_CUSTOM)
		return false;
#endif
	}

	uint32_t opengl_renderer_api::get_error()
	{
		switch (glGetError())
		{
		case GL_INVALID_ENUM					: return gl::error_type_invalid_enum;
		case GL_INVALID_VALUE					: return gl::error_type_invalid_value;
		case GL_INVALID_OPERATION				: return gl::error_type_invalid_operation;
		case GL_STACK_OVERFLOW					: return gl::error_type_stack_overflow;
		case GL_STACK_UNDERFLOW					: return gl::error_type_stack_underflow;
		case GL_OUT_OF_MEMORY					: return gl::error_type_out_of_memory;
		case GL_INVALID_FRAMEBUFFER_OPERATION	: return gl::error_type_invalid_framebuffer_operation;
		case GL_NO_ERROR						:
		default									: return gl::error_type_none;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_renderer_api::clear(uint32_t flags)
	{
		uint32_t temp{};
		ML_flag_map(temp, flags, GL_ACCUM_BUFFER_BIT	, gl::clear_flags_accum);
		ML_flag_map(temp, flags, GL_COLOR_BUFFER_BIT	, gl::clear_flags_color);
		ML_flag_map(temp, flags, GL_DEPTH_BUFFER_BIT	, gl::clear_flags_depth);
		ML_flag_map(temp, flags, GL_STENCIL_BUFFER_BIT	, gl::clear_flags_stencil);
		glCheck(glClear(temp));
	}

	void opengl_renderer_api::flush()
	{
		glCheck(glFlush());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_renderer_api::draw_arrays(uint32_t mode, uint32_t first, uint32_t count)
	{
		glCheck(glDrawArrays(mode, first, count));
	}

	void opengl_renderer_api::draw_indexed(uint32_t mode, int32_t first, uint32_t type, void const * indices)
	{
		glCheck(glDrawElements(mode, first, type, indices));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_renderer_api::set_active_texture(void const * value)
	{
		glCheck(glActiveTexture(value ? *reinterpret_cast<uint32_t const *>(value) : NULL));
	}

	void opengl_renderer_api::set_alpha_function(uint32_t value, float32_t ref)
	{
		glCheck(glAlphaFunc(std::invoke([&]() noexcept -> uint32_t
		{
			switch (value)
			{
			default						: return value;
			case gl::predicate_never	: return GL_NEVER;
			case gl::predicate_less		: return GL_LESS;
			case gl::predicate_equal	: return GL_EQUAL;
			case gl::predicate_lequal	: return GL_LEQUAL;
			case gl::predicate_greater	: return GL_GREATER;
			case gl::predicate_notequal	: return GL_NOTEQUAL;
			case gl::predicate_gequal	: return GL_GEQUAL;
			case gl::predicate_always	: return GL_ALWAYS;
			}
		}), ref));
	}

	void opengl_renderer_api::set_blend_equation(uint32_t modeRGB, uint32_t modeAlpha)
	{
		glCheck(glBlendEquationSeparate(modeRGB, modeAlpha));
	}

	void opengl_renderer_api::set_blend_function(uint32_t sfactorRGB, uint32_t dfactorRGB, uint32_t sfactorAlpha, uint32_t dfactorAlpha)
	{
		auto _factor = [](auto value) noexcept -> uint32_t
		{
			switch (value)
			{
			default								: return value;
			case gl::factor_zero				: return GL_ZERO;
			case gl::factor_one					: return GL_ONE;
			case gl::factor_src_color			: return GL_SRC_COLOR;
			case gl::factor_one_minus_src_color	: return GL_ONE_MINUS_SRC_COLOR;
			case gl::factor_src_alpha			: return GL_SRC_ALPHA;
			case gl::factor_one_minus_src_alpha	: return GL_ONE_MINUS_SRC_ALPHA;
			case gl::factor_dst_alpha			: return GL_DST_ALPHA;
			case gl::factor_one_minus_dst_alpha	: return GL_ONE_MINUS_DST_ALPHA;
			case gl::factor_dst_color			: return GL_DST_COLOR;
			case gl::factor_one_minus_dst_color	: return GL_ONE_MINUS_DST_COLOR;
			case gl::factor_src_alpha_saturate	: return GL_SRC_ALPHA_SATURATE;
			}
		};
		glCheck(glBlendFuncSeparate(
			_factor(sfactorRGB), _factor(dfactorRGB),
			_factor(sfactorAlpha), _factor(dfactorAlpha)));
	}

	void opengl_renderer_api::set_clear_color(color const & value)
	{
		glCheck(glClearColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_renderer_api::set_cull_mode(uint32_t value)
	{
		glCheck(glCullFace(std::invoke([value]() noexcept -> uint32_t
		{
			switch (value)
			{
			case gl::facet_front_left		: return GL_FRONT_LEFT;
			case gl::facet_front_right		: return GL_FRONT_RIGHT;
			case gl::facet_back_left		: return GL_BACK_LEFT;
			case gl::facet_back_right		: return GL_BACK_RIGHT;
			case gl::facet_front			: return GL_FRONT;
			case gl::facet_back				: return GL_BACK;
			case gl::facet_left				: return GL_LEFT;
			case gl::facet_right			: return GL_RIGHT;
			case gl::facet_front_and_back	: return GL_FRONT_AND_BACK;
			default: return value;
			}
		})));
	}

	void opengl_renderer_api::set_depth_function(uint32_t value)
	{
		glCheck(glDepthFunc(std::invoke([&]() noexcept -> uint32_t
		{
			switch (value)
			{
			default						: return value;
			case gl::predicate_never	: return GL_NEVER;
			case gl::predicate_less		: return GL_LESS;
			case gl::predicate_equal	: return GL_EQUAL;
			case gl::predicate_lequal	: return GL_LEQUAL;
			case gl::predicate_greater	: return GL_GREATER;
			case gl::predicate_notequal	: return GL_NOTEQUAL;
			case gl::predicate_gequal	: return GL_GEQUAL;
			case gl::predicate_always	: return GL_ALWAYS;
			}
		})));
	}

	void opengl_renderer_api::set_depth_mask(bool enabled)
	{
		glCheck(glDepthMask(enabled));
	}

	void opengl_renderer_api::set_enabled(uint32_t capability, bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(std::invoke([&]() noexcept -> uint32_t
		{
			switch (capability)
			{
			default											: return capability;
			case gl::capability_alpha_test					: return GL_ALPHA_TEST;
			case gl::capability_blend						: return GL_BLEND;
			case gl::capability_cull_face					: return GL_CULL_FACE;
			case gl::capability_depth_clamp					: return GL_DEPTH_CLAMP;
			case gl::capability_depth_test					: return GL_DEPTH_TEST;
			case gl::capability_dither						: return GL_DITHER;
			case gl::capability_framebuffer_srgb			: return GL_FRAMEBUFFER_SRGB;
			case gl::capability_line_smooth					: return GL_LINE_SMOOTH;
			case gl::capability_multisample					: return GL_MULTISAMPLE;
			case gl::capability_polygon_smooth				: return GL_POLYGON_SMOOTH;
			case gl::capability_sample_mask					: return GL_SAMPLE_MASK;
			case gl::capability_scissor_test				: return GL_SCISSOR_TEST;
			case gl::capability_stencil_test				: return GL_STENCIL_TEST;
			case gl::capability_texture_cube_map_seamless	: return GL_TEXTURE_CUBE_MAP_SEAMLESS;
			}
		})));
	}

	void opengl_renderer_api::set_viewport(int_rect const & value)
	{
		glCheck(glViewport(value[0], value[1], value[2], value[3]));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_IMPL_RENDERER_OPENGL