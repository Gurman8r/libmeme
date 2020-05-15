#if defined(ML_IMPL_RENDERER_OPENGL)

#include "Impl_Renderer_OpenGL.hpp"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_render_api::initialize()
	{
		static bool temp{ std::invoke([]() noexcept -> bool
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
		}) };
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_render_api::get_enabled(uint32_t capability) const
	{
		return glIsEnabled(gl::_capability(capability));
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

	cstring opengl_render_api::get_extensions() const
	{
		static cstring temp{};
		static ML_scope // once
		{
			glCheck(temp = reinterpret_cast<cstring>(glGetString(GL_EXTENSIONS)));
		};
		return temp;
	}
	
	int32_t opengl_render_api::get_major_version() const
	{
		static int32_t temp{};
		static ML_scope // once
		{
			if (glGetIntegerv(GL_MAJOR_VERSION, &temp); glGetError() == GL_INVALID_ENUM)
			{
				if (auto const version{ glGetString(GL_VERSION) })
				{
					temp = version[0] - '0';
				}
				else
				{
					temp = 1;
				}
			}
		};
		return temp;
	}
	
	int32_t opengl_render_api::get_minor_version() const
	{
		static int32_t temp{};
		static ML_scope // once
		{
			if (glGetIntegerv(GL_MINOR_VERSION, &temp); glGetError() == GL_INVALID_ENUM)
			{
				if (auto const version{ glGetString(GL_VERSION) })
				{
					temp = version[2] - '0';
				}
				else
				{
					temp = 1;
				}
			}
		};
		return temp;
	}
	
	int32_t opengl_render_api::get_num_extensions() const
	{
		static int32_t temp{};
		static ML_scope // once
		{
			glCheck(glGetIntegerv(GL_NUM_EXTENSIONS, &temp));
		};
		return temp;
	}
	
	cstring opengl_render_api::get_renderer() const
	{
		static cstring temp{};
		static ML_scope // once
		{
			glCheck(temp = reinterpret_cast<cstring>(glGetString(GL_RENDER)));
		};
		return temp;
	}
	
	cstring ml::opengl_render_api::get_shading_language_version() const
	{
		static cstring temp{};
		static ML_scope // once
		{
			glCheck(temp = reinterpret_cast<cstring>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
		};
		return temp;
	}
	
	cstring opengl_render_api::get_vendor() const
	{
		static cstring temp{};
		static ML_scope // once
		{
			glCheck(temp = reinterpret_cast<cstring>(glGetString(GL_VENDOR)));
		};
		return temp;
	}
	
	cstring opengl_render_api::get_version() const
	{
		static cstring temp{};
		static ML_scope // once
		{
			glCheck(temp = reinterpret_cast<cstring>(glGetString(GL_VERSION)));
		};
		return temp;
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

	void opengl_render_api::clear(uint32_t flags)
	{
		uint32_t temp{};
		ML_flag_map(temp, flags, GL_ACCUM_BUFFER_BIT	, gl::clear_flags_accum);
		ML_flag_map(temp, flags, GL_COLOR_BUFFER_BIT	, gl::clear_flags_color);
		ML_flag_map(temp, flags, GL_DEPTH_BUFFER_BIT	, gl::clear_flags_depth);
		ML_flag_map(temp, flags, GL_STENCIL_BUFFER_BIT	, gl::clear_flags_stencil);
		glCheck(glClear(temp));
	}

	void opengl_render_api::draw_arrays(uint32_t mode, uint32_t first, uint32_t count)
	{
		glCheck(glDrawArrays(gl::_primitive(mode), first, count));
	}

	void opengl_render_api::draw_indexed(uint32_t mode, int32_t first, uint32_t type, void const * indices)
	{
		glCheck(glDrawElements(gl::_primitive(mode), first, gl::_type(type), indices));
	}

	void opengl_render_api::flush()
	{
		glCheck(glFlush());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_IMPL_RENDERER_OPENGL