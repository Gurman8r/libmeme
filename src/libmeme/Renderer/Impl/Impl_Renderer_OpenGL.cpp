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

	void opengl_render_api::validate_version(int32_t & major, int32_t & minor)
	{
		glCheck(glGetIntegerv(GL_MAJOR_VERSION, &major));
		glCheck(glGetIntegerv(GL_MINOR_VERSION, &minor));

		if (glGetError() == GL_INVALID_ENUM)
		{
			if (auto const version{ glGetString(GL_VERSION) })
			{
				major = version[0] - '0';
				minor = version[2] - '0';
				debug::warning("using opengl version: {0}.{1}", major, minor);
			}
			else
			{
				major = 1;
				minor = 1;
				debug::warning("can't get the opengl version number, assuming 1.1");
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_render_api::get_enabled(uint32_t capability) const
	{
		return glIsEnabled(gl::_capability(capability));
	}

	uint32_t opengl_render_api::get_enum(hash_t code, uint32_t value) const
	{
		switch (code)
		{
		default									: return value;
		case hashof_v<gl::clear_flags_>			: return gl::_clear_flags(value);
		case hashof_v<gl::backend_>				: return gl::_backend(value);
		case hashof_v<gl::error_type_>			: return gl::_error_type(value);
		case hashof_v<gl::capability_>			: return gl::_capability(value);
		case hashof_v<gl::usage_>				: return gl::_usage(value);
		case hashof_v<gl::primitive_>			: return gl::_primitive(value);
		case hashof_v<gl::predicate_>			: return gl::_predicate(value);
		case hashof_v<gl::type_>				: return gl::_type(value);
		case hashof_v<gl::function_>			: return gl::_function(value);
		case hashof_v<gl::facet_>				: return gl::_facet(value);
		case hashof_v<gl::factor_>				: return gl::_factor(value);
		case hashof_v<gl::format_>				: return gl::_format(value);
		case hashof_v<gl::store_>				: return gl::_store(value);
		case hashof_v<gl::texture_>				: return gl::_texture(value);
		case hashof_v<gl::shader_>				: return gl::_shader(value);
		case hashof_v<gl::framebuffer_>			: return gl::_framebuffer(value);
		case hashof_v<gl::color_attachment_>	: return gl::_color_attachment(value);
		case hashof_v<gl::draw_buffer_>			: return gl::_draw_buffer(value);
		case hashof_v<gl::texture_attachment_>	: return gl::_texture_attachment(value);
		}
	}

	uint32_t opengl_render_api::get_error() const
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
		ML_flag_map(temp, flags, GL_ACCUM_BUFFER_BIT	, gl::clear_flags_accum);
		ML_flag_map(temp, flags, GL_COLOR_BUFFER_BIT	, gl::clear_flags_color);
		ML_flag_map(temp, flags, GL_DEPTH_BUFFER_BIT	, gl::clear_flags_depth);
		ML_flag_map(temp, flags, GL_STENCIL_BUFFER_BIT	, gl::clear_flags_stencil);
		glCheck(glClear(temp));
	}

	void opengl_render_api::draw_arrays(uint32_t primitive, uint32_t first, uint32_t count)
	{
		glCheck(glDrawArrays(gl::_primitive(primitive), first, count));
	}

	void opengl_render_api::draw_indexed(uint32_t primitive, int32_t first, uint32_t type, void const * indices)
	{
		glCheck(glDrawElements(gl::_primitive(primitive), first, gl::_type(type), indices));
	}

	void opengl_render_api::flush()
	{
		glCheck(glFlush());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_render_api::set_active_texture(void const * value)
	{
		glCheck(glActiveTexture(value ? *reinterpret_cast<uint32_t const *>(value) : NULL));
	}

	void opengl_render_api::set_alpha_function(uint32_t predicate, float_t ref)
	{
		glCheck(glAlphaFunc(gl::_predicate(predicate), ref));
	}

	void opengl_render_api::set_blend_color(color const & value)
	{
		glCheck(glBlendColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_render_api::set_blend_equation(uint32_t rgb, uint32_t alpha)
	{
		glCheck(glBlendEquationSeparate(gl::_function(rgb), gl::_function(alpha)));
	}

	void opengl_render_api::set_blend_function(uint32_t srgb, uint32_t drgb, uint32_t salpha, uint32_t dalpha)
	{
		glCheck(glBlendFuncSeparate(gl::_factor(srgb), gl::_factor(drgb), gl::_factor(salpha), gl::_factor(dalpha)));
	}

	void opengl_render_api::set_clear_color(color const & value)
	{
		glCheck(glClearColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_render_api::set_cull_mode(uint32_t facet)
	{
		glCheck(glCullFace(gl::_facet(facet)));
	}

	void opengl_render_api::set_depth_function(uint32_t predicate)
	{
		glCheck(glDepthFunc(gl::_predicate(predicate)));
	}

	void opengl_render_api::set_depth_mask(bool enabled)
	{
		glCheck(glDepthMask(enabled));
	}

	void opengl_render_api::set_enabled(uint32_t capability, bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(gl::_capability(capability)));
	}

	void opengl_render_api::set_viewport(int_rect const & bounds)
	{
		glCheck(glViewport(bounds[0], bounds[1], bounds[2], bounds[3]));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_IMPL_RENDERER_OPENGL