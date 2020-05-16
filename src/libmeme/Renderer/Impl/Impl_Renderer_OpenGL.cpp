#if defined(ML_IMPL_RENDERER_OPENGL)

#include "Impl_Renderer_OpenGL.hpp"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_render_api::initialize()
	{
		static bool temp{ std::invoke([]() -> bool
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
			glCheck(temp = (cstring)glGetString(GL_EXTENSIONS));
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
				temp = std::invoke([version = glGetString(GL_VERSION)]() noexcept
				{
					return version ? (version[0] - '0') : 1; // 
				});
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
				temp = std::invoke([version = glGetString(GL_VERSION)]() noexcept
				{
					return version ? (version[2] - '0') : 1;
				});
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
			glCheck(temp = (cstring)glGetString(GL_RENDER));
		};
		return temp;
	}
	
	cstring opengl_render_api::get_shading_language_version() const
	{
		static cstring temp{};
		static ML_scope // once
		{
			glCheck(temp = (cstring)glGetString(GL_SHADING_LANGUAGE_VERSION));
		};
		return temp;
	}
	
	cstring opengl_render_api::get_vendor() const
	{
		static cstring temp{};
		static ML_scope // once
		{
			glCheck(temp = (cstring)glGetString(GL_VENDOR));
		};
		return temp;
	}
	
	cstring opengl_render_api::get_version() const
	{
		static cstring temp{};
		static ML_scope // once
		{
			glCheck(temp = (cstring)glGetString(GL_VERSION));
		};
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	gl::alpha_function opengl_render_api::get_alpha_function() const
	{
		return
		{
			std::invoke([]() -> uint32_t
			{
				uint32_t func{};
				glCheck(glGetIntegerv(GL_ALPHA_TEST_FUNC, (int32_t *)&func));
				switch (func)
				{
				default			: return func;
				case GL_NEVER	: return gl::predicate_never;
				case GL_LESS	: return gl::predicate_less;
				case GL_EQUAL	: return gl::predicate_equal;
				case GL_LEQUAL	: return gl::predicate_lequal;
				case GL_GREATER	: return gl::predicate_greater;
				case GL_NOTEQUAL: return gl::predicate_notequal;
				case GL_GEQUAL	: return gl::predicate_gequal;
				case GL_ALWAYS	: return gl::predicate_always;
				}
			}),
			std::invoke([]() -> float_t
			{
				float_t ref{};
				glCheck(glGetFloatv(GL_ALPHA_TEST_REF, &ref));
				return ref;
			})
		};
	}

	color opengl_render_api::get_blend_color() const
	{
		color temp{};
		glCheck(glGetFloatv(GL_BLEND_COLOR, temp));
		return temp;
	}

	gl::blend_equation opengl_render_api::get_blend_equation() const
	{
		auto helper = [](auto value) -> uint32_t
		{
			uint32_t temp{};
			switch (glGetIntegerv(value, (int32_t *)&temp); temp)
			{
			default							: return temp;
			case GL_FUNC_ADD				: return gl::function_add;
			case GL_FUNC_SUBTRACT			: return gl::function_subtract;
			case GL_FUNC_REVERSE_SUBTRACT	: return gl::function_reverse_subtract;
			case GL_MIN						: return gl::function_min;
			case GL_MAX						: return gl::function_max;
			}
		};
		return {
			helper(GL_BLEND_EQUATION_RGB), helper(GL_BLEND_EQUATION_ALPHA)
		};
	}

	gl::blend_function opengl_render_api::get_blend_function() const
	{
		auto helper = [](auto value) -> uint32_t
		{
			uint32_t temp{};
			switch (glGetIntegerv(value, (int32_t *)&temp); temp)
			{
			default						: return temp;
			case GL_ZERO				: return gl::factor_zero;
			case GL_ONE					: return gl::factor_one;
			case GL_SRC_COLOR			: return gl::factor_src_color;
			case GL_ONE_MINUS_SRC_COLOR	: return gl::factor_one_minus_src_color;
			case GL_SRC_ALPHA			: return gl::factor_src_alpha;
			case GL_ONE_MINUS_SRC_ALPHA	: return gl::factor_one_minus_src_alpha;
			case GL_DST_ALPHA			: return gl::factor_dst_alpha;
			case GL_ONE_MINUS_DST_ALPHA	: return gl::factor_one_minus_dst_alpha;
			case GL_DST_COLOR			: return gl::factor_dst_color;
			case GL_ONE_MINUS_DST_COLOR	: return gl::factor_one_minus_dst_color;
			case GL_SRC_ALPHA_SATURATE	: return gl::factor_src_alpha_saturate;
			}
		};
		return {
			helper(GL_BLEND_SRC_RGB), helper(GL_BLEND_SRC_ALPHA),
			helper(GL_BLEND_DST_RGB), helper(GL_BLEND_DST_ALPHA),
		};
	}

	color opengl_render_api::get_clear_color() const
	{
		color temp{};
		glCheck(glGetFloatv(GL_COLOR_CLEAR_VALUE, temp));
		return temp;
	}

	uint32_t opengl_render_api::get_depth_function() const
	{
		uint32_t temp{};
		switch (glGetIntegerv(GL_DEPTH_FUNC, (int32_t *)&temp); temp)
		{
		default			: return temp;
		case GL_NEVER	: return gl::predicate_never;
		case GL_LESS	: return gl::predicate_less;
		case GL_EQUAL	: return gl::predicate_equal;
		case GL_LEQUAL	: return gl::predicate_lequal;
		case GL_GREATER	: return gl::predicate_greater;
		case GL_NOTEQUAL: return gl::predicate_notequal;
		case GL_GEQUAL	: return gl::predicate_gequal;
		case GL_ALWAYS	: return gl::predicate_always;
		}
	}

	bool opengl_render_api::get_depth_mask() const
	{
		uint8_t temp;
		glCheck(glGetBooleanv(GL_DEPTH_WRITEMASK, &temp));
		return temp;
	}

	gl::depth_range opengl_render_api::get_depth_range() const
	{
		gl::depth_range temp{};
		glCheck(glGetFloatv(GL_DEPTH_RANGE, &temp.nearVal));
		return temp;
	}

	bool opengl_render_api::get_enabled(uint32_t capability) const
	{
		return glIsEnabled(gl::_capability(capability));
	}

	uint32_t opengl_render_api::get_front_face() const
	{
		uint32_t temp{};
		switch (glGetIntegerv(GL_FRONT_FACE, (int32_t *)&temp); temp)
		{
		default		: return temp;
		case GL_CW	: return gl::front_face_cw;
		case GL_CCW	: return gl::front_face_ccw;
		}
	}

	int_rect opengl_render_api::get_viewport() const
	{
		int_rect temp{};
		glCheck(glGetIntegerv(GL_VIEWPORT, temp));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_render_api::set_alpha_function(gl::alpha_function const & value)
	{
		glCheck(glAlphaFunc(gl::_predicate(value.func), value.ref));
	}

	void opengl_render_api::set_blend_color(color const & value)
	{
		glCheck(glBlendColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_render_api::set_blend_equation(gl::blend_equation const & value)
	{
		glCheck(glBlendEquationSeparate(
			gl::_function(value.modeRGB),
			gl::_function(value.modeAlpha)));
	}

	void opengl_render_api::set_blend_function(gl::blend_function const & value)
	{
		glCheck(glBlendFuncSeparate(
			gl::_factor(value.sfactorRGB), gl::_factor(value.dfactorRGB),
			gl::_factor(value.sfactorAlpha), gl::_factor(value.dfactorAlpha)));
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

	void opengl_render_api::set_depth_range(gl::depth_range const & value)
	{
		glCheck(glDepthRangef(value.nearVal, value.farVal));
	}

	void opengl_render_api::set_enabled(uint32_t capability, bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(gl::_capability(capability)));
	}

	void opengl_render_api::set_front_face(uint32_t front_face)
	{
		glCheck(glFrontFace(gl::_front_face(front_face)));
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
}

#endif // ML_IMPL_RENDERER_OPENGL