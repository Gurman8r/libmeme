#if defined(ML_IMPL_RENDERER_OPENGL)

#include "Impl_Renderer_OpenGL.hpp"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_render_api::initialize()
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

	uint32_t opengl_render_api::get_error()
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

	void opengl_render_api::clear(uint32_t flags)
	{
		uint32_t temp{};
		ML_flag_map(temp, GL_DEPTH_BUFFER_BIT	, flags, gl::clear_flags_depth);
		ML_flag_map(temp, GL_ACCUM_BUFFER_BIT	, flags, gl::clear_flags_accum);
		ML_flag_map(temp, GL_STENCIL_BUFFER_BIT	, flags, gl::clear_flags_stencil);
		ML_flag_map(temp, GL_COLOR_BUFFER_BIT	, flags, gl::clear_flags_color);
		glCheck(glClear(temp));
	}

	void opengl_render_api::flush()
	{
		glCheck(glFlush());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_render_api::draw_arrays(uint32_t mode, uint32_t first, uint32_t count)
	{
		glCheck(glDrawArrays(mode, first, count));
	}

	void opengl_render_api::draw_indexed(uint32_t mode, int32_t first, uint32_t type, void const * indices)
	{
		glCheck(glDrawElements(mode, first, type, indices));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_render_api::set_active_texture(void const * value)
	{
		glCheck(glActiveTexture(value ? *reinterpret_cast<uint32_t const *>(value) : NULL));
	}

	void opengl_render_api::set_alpha_function(uint32_t func, float32_t ref)
	{
		glCheck(glAlphaFunc(func, ref));
	}

	void opengl_render_api::set_blend_equation(uint32_t value)
	{
		glCheck(glBlendEquation(value));
	}

	void opengl_render_api::set_blend_equation_separate(uint32_t modeRGB, uint32_t modeAlpha)
	{
		glCheck(glBlendEquationSeparate(modeRGB, modeAlpha));
	}

	void opengl_render_api::set_blend_function(uint32_t sFactor, uint32_t dFactor)
	{
		glCheck(glBlendFunc(sFactor, dFactor));
	}

	void opengl_render_api::set_blend_function_separate(uint32_t sfactorRGB, uint32_t dfactorRGB, uint32_t sfactorAlpha, uint32_t dfactorAlpha)
	{
		glCheck(glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha));
	}

	void opengl_render_api::set_clear_color(color const & value)
	{
		glCheck(glClearColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_render_api::set_cull_mode(uint32_t value)
	{
		glCheck(glCullFace(value));
	}

	void opengl_render_api::set_depth_function(uint32_t value)
	{
		glCheck(glDepthFunc(value));
	}

	void opengl_render_api::set_depth_mask(bool value)
	{
		glCheck(glDepthMask(value));
	}

	void opengl_render_api::set_enabled(uint32_t capability, bool enabled)
	{
		glCheck(enabled ? glEnable(capability) : glDisable(capability));
	}

	void opengl_render_api::set_polygon_mode(uint32_t face, uint32_t mode)
	{
		glCheck(glPolygonMode(face, mode));
	}

	void opengl_render_api::set_viewport(int_rect const & value)
	{
		glCheck(glViewport(value[0], value[1], value[2], value[3]));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_IMPL_RENDERER_OPENGL