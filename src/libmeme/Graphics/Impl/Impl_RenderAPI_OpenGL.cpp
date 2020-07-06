#if defined(ML_IMPL_RENDERER_OPENGL)

#include "Impl_RenderAPI_OpenGL.hpp"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl loader
#ifdef ML_OPENGL_LOADER_CUSTOM
#	if ML_has_include(ML_OPENGL_LOADER_CUSTOM)
#		include ML_OPENGL_LOADER_CUSTOM
#	endif

#elif defined(ML_IMPL_OPENGL_ES2)
#	include <GLES2/gl2.h>

#elif defined(ML_IMPL_OPENGL_ES3)
#	if defined(ML_os_apple) && (TARGET_OS_IOS || TARGET_OS_TV)
#		include <OpenGLES/ES3/gl.h>
#	else
#		include <GLES3/gl3.h>
#	endif

#elif defined(ML_IMPL_OPENGL_LOADER_GLEW)
#	include <GL/glew.h>

#elif defined(ML_IMPL_OPENGL_LOADER_GL3W)
#	include <GL/gl3w.h>

#elif defined(ML_IMPL_OPENGL_LOADER_GLAD)
#	include <glad/glad.h>

#else
#	error "unknown or invalid opengl loader specified"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl init
#ifndef ML_IMPL_OPENGL_INIT
#	if defined(ML_IMPL_OPENGL_LOADER_GLEW)
//										glew
#		define ML_IMPL_OPENGL_INIT()	((glewExperimental = true) && (glewInit() == GLEW_OK))

#	elif defined(ML_IMPL_OPENGL_LOADER_GL3W)
//										gl3w
#		define ML_IMPL_OPENGL_INIT()	gl3wInit()

#	elif defined(ML_IMPL_OPENGL_LOADER_GLAD)
//										glad
#		define ML_IMPL_OPENGL_INIT()	gladLoadGL()
#	else
//										custom
#		define ML_IMPL_OPENGL_INIT()	false
#	endif
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// helpers / custom stuff

// get error
#define ML_glGetError() _ML_GFX _error<_ML_GFX to_user>( glGetError() )

// check error
#if ML_is_debug
#	define ML_glCheck(expr) do{ (expr); gl_check_error(#expr, __FILE__, __LINE__); } while(0)
#else
#	define ML_glCheck(expr) (expr)
#endif

// enable / disable
#define ML_glEnable(id, enabled) (enabled ? &glEnable : &glDisable)( id )

// info log
#define ML_glGetObjectInfoLogLength(obj, x)			glGetShaderiv( obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, x )
#define ML_glGetObjectInfoLog(obj, size, len, str)	glGetProgramInfoLog( obj, size, len, str )

// shaders
#define ML_glCreateShader(type)						glCreateShaderObjectARB( _ML_GFX _shader_type<_ML_GFX to_impl>(type) )
#define ML_glDeleteShader(obj)						glDeleteObjectARB( obj )
#define ML_glShaderSource(obj, count, str, len)		glShaderSourceARB( obj, (uint32_t)count, str, len )
#define ML_glCompileShader(obj)						glCompileShaderARB( obj )
#define ML_glGetShaderCompileStatus(obj, x)			glGetObjectParameterivARB( obj, GL_OBJECT_COMPILE_STATUS_ARB, x )

// programs
#define ML_glCreateProgram()						glCreateProgramObjectARB()
#define ML_glDeleteProgram(obj)						glDeleteObjectARB( obj )
#define ML_glGetProgram()							glGetHandleARB( GL_PROGRAM_OBJECT_ARB )
#define ML_glUseProgram(obj)						glUseProgramObjectARB( obj )
#define ML_glAttachShader(obj, x)					glAttachObjectARB( obj, ML_handle(uint32_t, x) )
#define ML_glDetachShader(obj, x)					glDetachObjectARB( obj, ML_handle(uint32_t, x) )
#define ML_glLinkProgram(obj)						glLinkProgramARB( obj )
#define ML_glGetProgramLinkStatus(obj, x)			glGetObjectParameterivARB( obj, GL_OBJECT_LINK_STATUS_ARB, x )
#define ML_glValidateProgram(obj)					glValidateProgramARB( obj )
#define ML_glGetProgramValidateStatus(obj, x)		glGetObjectParameterivARB( obj, GL_OBJECT_VALIDATE_STATUS_ARB, x )

// uniforms
#define ML_glGetUniformLocation(obj, name)			ML_handle( _ML_GFX uniform_id, glGetUniformLocationARB(obj, name) )
#define ML_glUniform1i(loc, x)						glUniform1iARB( ML_handle(int32_t, loc), x )
#define ML_glUniform1f(loc, x)						glUniform1fARB( ML_handle(int32_t, loc), x )
#define ML_glUniform2f(loc, x, y)					glUniform2fARB( ML_handle(int32_t, loc), x, y )
#define ML_glUniform3f(loc, x, y, z)				glUniform3fARB( ML_handle(int32_t, loc), x, y, z )
#define ML_glUniform4f(loc, x, y, z, w)				glUniform4fARB( ML_handle(int32_t, loc), x, y, z, w )
#define ML_glUniformMatrix2fv(loc, transpose, ptr)	glUniformMatrix2fvARB( ML_handle(int32_t, loc), 2 * 2, transpose, ptr )
#define ML_glUniformMatrix3fv(loc, transpose, ptr)	glUniformMatrix3fvARB( ML_handle(int32_t, loc), 3 * 3, transpose, ptr )
#define ML_glUniformMatrix4fv(loc, transpose, ptr)	glUniformMatrix4fvARB( ML_handle(int32_t, loc), 4 * 4, transpose, ptr )

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//  enums
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _action(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default				: return value;
			case action_keep	: return GL_KEEP;
			case action_zero	: return GL_ZERO;
			case action_replace	: return GL_REPLACE;
			case action_inc		: return GL_INCR;
			case action_inc_wrap: return GL_INCR_WRAP;
			case action_dec		: return GL_DECR;
			case action_dec_wrap: return GL_DECR_WRAP;
			case action_invert	: return GL_INVERT;
			}
		}
		else // to user
		{
			switch (value)
			{
			default				: return value;
			case GL_KEEP		: return action_keep;
			case GL_ZERO		: return action_zero;
			case GL_REPLACE		: return action_replace;
			case GL_INCR		: return action_inc;
			case GL_INCR_WRAP	: return action_inc_wrap;
			case GL_DECR		: return action_dec;
			case GL_DECR_WRAP	: return action_dec_wrap;
			case GL_INVERT		: return action_invert;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _error(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default									: return value;
			case error_none							: return GL_NO_ERROR;
			case error_invalid_enum					: return GL_INVALID_ENUM;
			case error_invalid_value				: return GL_INVALID_VALUE;
			case error_invalid_operation			: return GL_INVALID_OPERATION;
			case error_stack_overflow				: return GL_STACK_OVERFLOW;
			case error_stack_underflow				: return GL_STACK_UNDERFLOW;
			case error_out_of_memory				: return GL_OUT_OF_MEMORY;
			case error_invalid_framebuffer_operation: return GL_INVALID_FRAMEBUFFER_OPERATION;
			case error_context_lost					: return GL_CONTEXT_LOST;
			}
		}
		else // to user
		{
			switch (value)
			{
			default									: return value;
			case GL_NO_ERROR						: return error_none;
			case GL_INVALID_ENUM					: return error_invalid_enum;
			case GL_INVALID_VALUE					: return error_invalid_value;
			case GL_INVALID_OPERATION				: return error_invalid_operation;
			case GL_STACK_OVERFLOW					: return error_stack_overflow;
			case GL_STACK_UNDERFLOW					: return error_stack_underflow;
			case GL_OUT_OF_MEMORY					: return error_out_of_memory;
			case GL_INVALID_FRAMEBUFFER_OPERATION	: return error_invalid_framebuffer_operation;
			case GL_CONTEXT_LOST					: return error_context_lost;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _equation(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default							: return value;
			case equation_add				: return GL_FUNC_ADD;
			case equation_subtract			: return GL_FUNC_SUBTRACT;
			case equation_reverse_subtract	: return GL_FUNC_REVERSE_SUBTRACT;
			case equation_min				: return GL_MIN;
			case equation_max				: return GL_MAX;
			}
		}
		else // to user
		{
			switch (value)
			{
			default							: return value;
			case GL_FUNC_ADD				: return equation_add;
			case GL_FUNC_SUBTRACT			: return equation_subtract;
			case GL_FUNC_REVERSE_SUBTRACT	: return equation_reverse_subtract;
			case GL_MIN						: return equation_min;
			case GL_MAX						: return equation_max;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _facet(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default						: return value;
			case facet_front_left		: return GL_FRONT_LEFT;
			case facet_front_right		: return GL_FRONT_RIGHT;
			case facet_back_left		: return GL_BACK_LEFT;
			case facet_back_right		: return GL_BACK_RIGHT;
			case facet_front			: return GL_FRONT;
			case facet_back				: return GL_BACK;
			case facet_left				: return GL_LEFT;
			case facet_right			: return GL_RIGHT;
			case facet_front_and_back	: return GL_FRONT_AND_BACK;
			}
		}
		else // to user
		{
			switch (value)
			{
			default					: return value;
			case GL_FRONT_LEFT		: return facet_front_left;
			case GL_FRONT_RIGHT		: return facet_front_right;
			case GL_BACK_LEFT		: return facet_back_left;
			case GL_BACK_RIGHT		: return facet_back_right;
			case GL_FRONT			: return facet_front;
			case GL_BACK			: return facet_back;
			case GL_LEFT			: return facet_left;
			case GL_RIGHT			: return facet_right;
			case GL_FRONT_AND_BACK	: return facet_front_and_back;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _factor(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default							: return value;
			case factor_zero				: return GL_ZERO;
			case factor_one					: return GL_ONE;
			case factor_src_color			: return GL_SRC_COLOR;
			case factor_one_minus_src_color	: return GL_ONE_MINUS_SRC_COLOR;
			case factor_src_alpha			: return GL_SRC_ALPHA;
			case factor_one_minus_src_alpha	: return GL_ONE_MINUS_SRC_ALPHA;
			case factor_dst_alpha			: return GL_DST_ALPHA;
			case factor_one_minus_dst_alpha	: return GL_ONE_MINUS_DST_ALPHA;
			case factor_dst_color			: return GL_DST_COLOR;
			case factor_one_minus_dst_color	: return GL_ONE_MINUS_DST_COLOR;
			case factor_src_alpha_saturate	: return GL_SRC_ALPHA_SATURATE;
			}
		}
		else // to user
		{
			switch (value)
			{
			default						: return value;
			case GL_ZERO				: return factor_zero;
			case GL_ONE					: return factor_one;
			case GL_SRC_COLOR			: return factor_src_color;
			case GL_ONE_MINUS_SRC_COLOR	: return factor_one_minus_src_color;
			case GL_SRC_ALPHA			: return factor_src_alpha;
			case GL_ONE_MINUS_SRC_ALPHA	: return factor_one_minus_src_alpha;
			case GL_DST_ALPHA			: return factor_dst_alpha;
			case GL_ONE_MINUS_DST_ALPHA	: return factor_one_minus_dst_alpha;
			case GL_DST_COLOR			: return factor_dst_color;
			case GL_ONE_MINUS_DST_COLOR	: return factor_one_minus_dst_color;
			case GL_SRC_ALPHA_SATURATE	: return factor_src_alpha_saturate;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _format(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default							: return value;
			case format_red					: return GL_RED;
			case format_green				: return GL_GREEN;
			case format_blue				: return GL_BLUE;
			case format_alpha				: return GL_ALPHA;
			case format_rgb					: return GL_RGB;
			case format_rgba				: return GL_RGBA;
			case format_luminance			: return GL_LUMINANCE;
			case format_luminance_alpha		: return GL_LUMINANCE_ALPHA;
			case format_srgb				: return GL_SRGB;
			case format_srgb8				: return GL_SRGB8;
			case format_srgb_alpha			: return GL_SRGB_ALPHA;
			case format_srgb8_alpha8		: return GL_SRGB8_ALPHA8;
			case format_sluminance_alpha	: return GL_SLUMINANCE_ALPHA;
			case format_sluminance8_alpha8	: return GL_SLUMINANCE8_ALPHA8;
			case format_sluminance			: return GL_SLUMINANCE;
			case format_sluminance8			: return GL_SLUMINANCE8;

			case format_depth_stencil		: return GL_DEPTH_STENCIL;
			case format_depth24_stencil8	: return GL_DEPTH24_STENCIL8;
			}
		}
		else // to user
		{
			switch (value)
			{
			default							: return value;
			case GL_RED						: return format_red;
			case GL_GREEN					: return format_green;
			case GL_BLUE					: return format_blue;
			case GL_ALPHA					: return format_alpha;
			case GL_RGB						: return format_rgb;
			case GL_RGBA					: return format_rgba;
			case GL_LUMINANCE				: return format_luminance;
			case GL_LUMINANCE_ALPHA			: return format_luminance_alpha;
			case GL_SRGB					: return format_srgb;
			case GL_SRGB8					: return format_srgb8;
			case GL_SRGB_ALPHA				: return format_srgb_alpha;
			case GL_SRGB8_ALPHA8			: return format_srgb8_alpha8;
			case GL_SLUMINANCE_ALPHA		: return format_sluminance_alpha;
			case GL_SLUMINANCE8_ALPHA8		: return format_sluminance8_alpha8;
			case GL_SLUMINANCE				: return format_sluminance;
			case GL_SLUMINANCE8				: return format_sluminance8;

			case GL_DEPTH_STENCIL			: return format_depth_stencil;
			case GL_DEPTH24_STENCIL8		: return format_depth24_stencil8;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _order(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default			: return value;
			case order_cw	: return GL_CW;
			case order_ccw	: return GL_CCW;
			}
		}
		else // to user
		{
			switch (value)
			{
			default		: return value;
			case GL_CW	: return order_cw;
			case GL_CCW	: return order_ccw;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _predicate(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default					: return value;
			case predicate_never	: return GL_NEVER;
			case predicate_less		: return GL_LESS;
			case predicate_equal	: return GL_EQUAL;
			case predicate_lequal	: return GL_LEQUAL;
			case predicate_greater	: return GL_GREATER;
			case predicate_notequal	: return GL_NOTEQUAL;
			case predicate_gequal	: return GL_GEQUAL;
			case predicate_always	: return GL_ALWAYS;
			}
		}
		else // to user
		{
			switch (value)
			{
			default			: return value;
			case GL_NEVER	: return predicate_never;
			case GL_LESS	: return predicate_less;
			case GL_EQUAL	: return predicate_equal;
			case GL_LEQUAL	: return predicate_lequal;
			case GL_GREATER	: return predicate_greater;
			case GL_NOTEQUAL: return predicate_notequal;
			case GL_GEQUAL	: return predicate_gequal;
			case GL_ALWAYS	: return predicate_always;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _primitive(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default							: return value;
			case primitive_points			: return GL_POINTS;
			case primitive_lines			: return GL_LINES;
			case primitive_line_loop		: return GL_LINE_LOOP;
			case primitive_line_strip		: return GL_LINE_STRIP;
			case primitive_triangles		: return GL_TRIANGLES;
			case primitive_triangle_strip	: return GL_TRIANGLE_STRIP;
			case primitive_triangle_fan		: return GL_TRIANGLE_FAN;
			case primitive_fill				: return GL_FILL;
			}
		}
		else // to user
		{
			switch (value)
			{
			default					: return value;
			case GL_POINTS			: return primitive_points;
			case GL_LINES			: return primitive_lines;
			case GL_LINE_LOOP		: return primitive_line_loop;
			case GL_LINE_STRIP		: return primitive_line_strip;
			case GL_TRIANGLES		: return primitive_triangles;
			case GL_TRIANGLE_STRIP	: return primitive_triangle_strip;
			case GL_TRIANGLE_FAN	: return primitive_triangle_fan;
			case GL_FILL			: return primitive_fill;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _shader_type(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default						: return value;
			case shader_type_vertex		: return GL_VERTEX_SHADER;
			case shader_type_fragment	: return GL_FRAGMENT_SHADER;
			case shader_type_geometry	: return GL_GEOMETRY_SHADER;
			}
		}
		else // to user
		{
			switch (value)
			{
			default					: return value;
			case GL_VERTEX_SHADER	: return shader_type_vertex;
			case GL_FRAGMENT_SHADER	: return shader_type_fragment;
			case GL_GEOMETRY_SHADER	: return shader_type_geometry;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _texture_type(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default						: return value;
			case texture_type_1d		: return GL_TEXTURE_1D;
			case texture_type_2d		: return GL_TEXTURE_2D;
			case texture_type_3d		: return GL_TEXTURE_3D;
			case texture_type_cubemap	: return GL_TEXTURE_CUBE_MAP;
			}
		}
		else // to user
		{
			switch (value)
			{
			default					: return value;
			case GL_TEXTURE_1D		: return texture_type_1d;
			case GL_TEXTURE_2D		: return texture_type_2d;
			case GL_TEXTURE_3D		: return texture_type_3d;
			case GL_TEXTURE_CUBE_MAP: return texture_type_cubemap;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _type(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default						: return value;
			case type_byte				: return GL_BYTE;
			case type_unsigned_byte		: return GL_UNSIGNED_BYTE;
			case type_short				: return GL_SHORT;
			case type_unsigned_short	: return GL_UNSIGNED_SHORT;
			case type_int				: return GL_INT;
			case type_unsigned_int		: return GL_UNSIGNED_INT;
			case type_float				: return GL_FLOAT;
			case type_half_float		: return GL_HALF_FLOAT;
			case type_unsigned_int_24_8	: return GL_UNSIGNED_INT_24_8;
			}
		}
		else // to user
		{
			switch (value)
			{
			default						: return value;
			case GL_BYTE				: return type_byte;
			case GL_UNSIGNED_BYTE		: return type_unsigned_byte;
			case GL_SHORT				: return type_short;
			case GL_UNSIGNED_SHORT		: return type_unsigned_short;
			case GL_INT					: return type_int;
			case GL_UNSIGNED_INT		: return type_unsigned_int;
			case GL_FLOAT				: return type_float;
			case GL_HALF_FLOAT			: return type_half_float;
			case GL_UNSIGNED_INT_24_8	: return type_unsigned_int_24_8;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _usage(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default					: return value;
			case usage_stream	: return GL_STREAM_DRAW;
			case usage_static	: return GL_STATIC_DRAW;
			case usage_dynamic	: return GL_DYNAMIC_DRAW;
			}
		}
		else // to user
		{
			switch (value)
			{
			default					: return value;
			case GL_STREAM_DRAW		: return usage_stream;
			case GL_STATIC_DRAW		: return usage_static;
			case GL_DYNAMIC_DRAW	: return usage_dynamic;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// check error
namespace ml::gfx
{
	// check opengl error
	static void gl_check_error(cstring expr, cstring file, size_t line) noexcept
	{
		if (auto const code{ ML_glGetError() })
		{
			std::cout
				<< "| An internal OpenGL error occurred ( " << code << " )\n"
				<< "|    " << fs::path{ file }.filename() << " (" << line << ")\n"
				<< "|  expression:\n"
				<< "|    " << expr << "\n"
				<< "|  description:\n"
				<< "|    " << error_names[code] << "\n"
				<< "|    " << error_descriptions[code] << "\n"
				<< "\n";
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_device::opengl_device() : device{}
	{
		static bool const opengl_init{ ML_IMPL_OPENGL_INIT() };
		ML_assert("failed initializing opengl" && opengl_init);

		// renderer
		ML_glCheck(m_devinfo.renderer = (cstring)glGetString(GL_RENDERER));

		// vendor
		ML_glCheck(m_devinfo.vendor = (cstring)glGetString(GL_VENDOR));

		// version
		ML_glCheck(m_devinfo.version = (cstring)glGetString(GL_VERSION));

		// major version
		if (glGetIntegerv(GL_MAJOR_VERSION, &m_devinfo.major_version); glGetError() == GL_INVALID_ENUM)
		{
			m_devinfo.major_version = !m_devinfo.version.empty() ? m_devinfo.version[0] - '0' : 1;
		}

		// minor version
		if (glGetIntegerv(GL_MINOR_VERSION, &m_devinfo.minor_version); glGetError() == GL_INVALID_ENUM)
		{
			m_devinfo.minor_version = !m_devinfo.version.empty() ? m_devinfo.version[2] - '0' : 1;
		}

		// extensions
		{
			int32_t num{};
			ML_glCheck(glGetIntegerv(GL_NUM_EXTENSIONS, &num));
			m_devinfo.extensions.reserve(num);

			pmr::stringstream ss{};
			ML_glCheck(ss.str((cstring)glGetString(GL_EXTENSIONS)));

			pmr::string line{};
			while (std::getline(ss, line, ' '))
			{
				m_devinfo.extensions.push_back(line);
			}
		}

		// texture edge clamp available
#if defined(GL_EXT_texture_edge_clamp) \
|| defined(GLEW_EXT_texture_edge_clamp) \
|| defined(GL_SGIS_texture_edge_clamp)
		m_devinfo.texture_edge_clamp_available = true;
#endif
		// max texture slots
		ML_glCheck(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (int32_t *)&m_devinfo.max_texture_slots));

		// max color attachments
		ML_glCheck(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, (int32_t *)&m_devinfo.max_color_attachments));

		// max samples
		ML_glCheck(glGetIntegerv(GL_MAX_SAMPLES, (int32_t *)&m_devinfo.max_samples));

		// shaders available
#if defined(GL_ARB_shading_language_100) \
|| defined(GL_ARB_shader_objects) \
|| defined(GL_ARB_vertex_shader) \
|| defined(GL_ARB_fragment_shader)
		m_devinfo.shaders_available = true;

		// geometry shaders available
#	ifdef GL_ARB_geometry_shader4
		m_devinfo.geometry_shaders_available = true;
#	endif

		// shading language version
		ML_glCheck(m_devinfo.shading_language_version = (cstring)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared<context> opengl_device::create_context(context_settings const & cs) noexcept
	{
		return _ML make_shared<opengl_context>(this, cs);
	}

	shared<vertexarray> opengl_device::create_vertexarray(uint32_t prim) noexcept
	{
		return _ML make_shared<opengl_vertexarray>(this, prim);
	}

	shared<vertexbuffer> opengl_device::create_vertexbuffer(uint32_t usage, size_t count, address_t data) noexcept
	{
		return _ML make_shared<opengl_vertexbuffer>(this, usage, count, data);
	}

	shared<indexbuffer> opengl_device::create_indexbuffer(uint32_t usage, size_t count, address_t data) noexcept
	{
		return _ML make_shared<opengl_indexbuffer>(this, usage, count, data);
	}

	shared<texture2d> opengl_device::create_texture2d(texopts const & opts, address_t data) noexcept
	{
		return _ML make_shared<opengl_texture2d>(this, opts, data);
	}

	shared<texturecube> opengl_device::create_texturecube(texopts const & opts) noexcept
	{
		return _ML make_shared<opengl_texturecube>(this, opts);
	}

	shared<framebuffer> opengl_device::create_framebuffer(texopts const & opts) noexcept
	{
		return _ML make_shared<opengl_framebuffer>(this, opts);
	}

	shared<shader> opengl_device::create_shader(uint32_t type, pmr::vector<pmr::string> const & src) noexcept
	{
		return _ML make_shared<opengl_shader>(this, type, src);
	}

	shared<program> opengl_device::create_program() noexcept
	{
		return _ML make_shared<opengl_program>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device context
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_context::opengl_context(device * parent, context_settings const & cs)
		: context{ parent }, m_settings{ cs }
	{
		ML_assert(cs.api == context_api_opengl);
		ML_assert(cs.major == parent->get_info().major_version);
		ML_assert(cs.minor == parent->get_info().minor_version);

		ML_glCheck(ML_glEnable(GL_MULTISAMPLE, cs.multisample));
		ML_glCheck(ML_glEnable(GL_FRAMEBUFFER_SRGB, cs.srgb_capable));
		ML_glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_context::get_alpha_enabled() const
	{
		bool temp{};
		ML_glCheck(glGetBooleanv(GL_ALPHA_TEST, (uint8_t *)&temp));
		return temp;
	}

	alpha_mode opengl_context::get_alpha_mode() const
	{
		alpha_mode temp{};

		ML_glCheck(glGetIntegerv(GL_ALPHA_TEST_FUNC, (int32_t *)&temp.pred));
		temp.pred = _predicate<to_user>(temp.pred);

		ML_glCheck(glGetFloatv(GL_ALPHA_TEST_REF, &temp.ref));

		return temp;
	}

	bool opengl_context::get_blend_enabled() const
	{
		bool temp{};
		ML_glCheck(glGetBooleanv(GL_BLEND, (uint8_t *)&temp));
		return temp;
	}

	color opengl_context::get_blend_color() const
	{
		color temp{};
		ML_glCheck(glGetFloatv(GL_BLEND_COLOR, temp));
		return temp;
	}

	blend_mode opengl_context::get_blend_mode() const
	{
		blend_mode temp{};

		ML_glCheck(glGetIntegerv(GL_BLEND_EQUATION_RGB, (int32_t *)&temp.color_equation));
		temp.color_equation = _equation<to_user>(temp.color_equation);

		ML_glCheck(glGetIntegerv(GL_BLEND_SRC_RGB, (int32_t *)&temp.color_sfactor));
		temp.color_sfactor = _factor<to_user>(temp.color_sfactor);

		ML_glCheck(glGetIntegerv(GL_BLEND_DST_RGB, (int32_t *)&temp.color_dfactor));
		temp.color_dfactor = _factor<to_user>(temp.color_dfactor);

		ML_glCheck(glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (int32_t *)&temp.alpha_equation));
		temp.alpha_equation = _equation<to_user>(temp.alpha_equation);

		ML_glCheck(glGetIntegerv(GL_BLEND_SRC_ALPHA, (int32_t *)&temp.alpha_sfactor));
		temp.alpha_sfactor = _factor<to_user>(temp.alpha_sfactor);

		ML_glCheck(glGetIntegerv(GL_BLEND_DST_ALPHA, (int32_t *)&temp.alpha_dfactor));
		temp.alpha_dfactor = _factor<to_user>(temp.alpha_dfactor);

		return temp;
	}

	color opengl_context::get_clear_color() const
	{
		color temp{};
		ML_glCheck(glGetFloatv(GL_COLOR_CLEAR_VALUE, temp));
		return temp;
	}

	bool opengl_context::get_cull_enabled() const
	{
		bool temp{};
		ML_glCheck(glGetBooleanv(GL_CULL_FACE, (uint8_t *)&temp));
		return temp;
	}

	cull_mode opengl_context::get_cull_mode() const
	{
		cull_mode temp{};

		ML_glCheck(glGetIntegerv(GL_CULL_FACE_MODE, (int32_t *)&temp.facet));
		temp.facet = _facet<to_user>(temp.facet);

		ML_glCheck(glGetIntegerv(GL_FRONT_FACE, (int32_t *)&temp.order));
		temp.order = _order<to_user>(temp.order);

		return temp;
	}

	bool opengl_context::get_depth_enabled() const
	{
		bool temp{};
		ML_glCheck(glGetBooleanv(GL_DEPTH_TEST, (uint8_t *)&temp));
		return temp;
	}

	depth_mode opengl_context::get_depth_mode() const
	{
		depth_mode temp{};

		ML_glCheck(glGetIntegerv(GL_DEPTH_FUNC, (int32_t *)&temp.pred));
		temp.pred = _predicate<to_user>(temp.pred);
		
		ML_glCheck(glGetFloatv(GL_DEPTH_RANGE, temp.range));
		
		return temp;
	}

	bool opengl_context::get_stencil_enabled() const
	{
		bool temp{};
		ML_glCheck(glGetBooleanv(GL_STENCIL_TEST, (uint8_t *)&temp));
		return temp;
	}

	stencil_mode opengl_context::get_stencil_mode() const
	{
		stencil_mode temp{};

		ML_glCheck(glGetIntegerv(GL_STENCIL_FUNC, (int32_t *)&temp.pred));
		temp.pred = _predicate<to_user>(temp.pred);

		ML_glCheck(glGetIntegerv(GL_STENCIL_REF, &temp.ref));
		
		ML_glCheck(glGetIntegerv(GL_STENCIL_VALUE_MASK, (int32_t *)&temp.mask));

		return temp;
	}

	int_rect opengl_context::get_viewport() const
	{
		int_rect temp{};
		ML_glCheck(glGetIntegerv(GL_VIEWPORT, temp));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_context::set_alpha_enabled(bool enabled)
	{
		ML_glCheck(ML_glEnable(GL_ALPHA_TEST, enabled));
	}

	void opengl_context::set_alpha_mode(alpha_mode const & value)
	{
		ML_glCheck(glAlphaFunc(_predicate<to_impl>(value.pred), value.ref));
	}

	void opengl_context::set_blend_color(color const & value)
	{
		ML_glCheck(glBlendColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_context::set_blend_enabled(bool enabled)
	{
		ML_glCheck(ML_glEnable(GL_BLEND, enabled));
	}

	void opengl_context::set_blend_mode(blend_mode const & value)
	{
		ML_glCheck(glBlendFuncSeparate(
			_factor<to_impl>(value.color_sfactor),
			_factor<to_impl>(value.color_dfactor),
			_factor<to_impl>(value.alpha_sfactor),
			_factor<to_impl>(value.alpha_dfactor)));

		ML_glCheck(glBlendEquationSeparate(
			_equation<to_impl>(value.color_equation),
			_equation<to_impl>(value.alpha_equation)));
	}

	void opengl_context::set_clear_color(color const & value)
	{
		ML_glCheck(glClearColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_context::set_cull_enabled(bool enabled)
	{
		ML_glCheck(ML_glEnable(GL_CULL_FACE, enabled));
	}

	void opengl_context::set_cull_mode(cull_mode const & value)
	{
		ML_glCheck(glCullFace(_facet<to_impl>(value.facet)));
		
		ML_glCheck(glFrontFace(_order<to_impl>(value.order)));
	}

	void opengl_context::set_depth_enabled(bool enabled)
	{
		ML_glCheck(ML_glEnable(GL_DEPTH_TEST, enabled));
	}

	void opengl_context::set_depth_mode(depth_mode const & value)
	{
		ML_glCheck(glDepthFunc(_predicate<to_impl>(value.pred)));

		ML_glCheck(glDepthRangef(value.range[0], value.range[1]));
	}

	void opengl_context::set_stencil_enabled(bool enabled)
	{
		ML_glCheck(ML_glEnable(GL_STENCIL_TEST, enabled));
	}

	void opengl_context::set_stencil_mode(stencil_mode const & value)
	{
		ML_glCheck(glStencilFuncSeparate(
			GL_FRONT_AND_BACK,
			_predicate<to_impl>(value.pred),
			value.ref,
			value.mask));
	}

	void opengl_context::set_viewport(int_rect const & bounds)
	{
		ML_glCheck(glViewport(bounds[0], bounds[1], bounds[2], bounds[3]));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_context::clear(uint32_t mask)
	{
		uint32_t temp{};
		ML_flag_map(temp, GL_COLOR_BUFFER_BIT	, mask, clear_color		);
		ML_flag_map(temp, GL_DEPTH_BUFFER_BIT	, mask, clear_depth		);
		ML_flag_map(temp, GL_STENCIL_BUFFER_BIT	, mask, clear_stencil	);
		ML_glCheck(glClear(temp));
	}

	void opengl_context::draw(shared<vertexarray> const & value)
	{
		// could be moved into header file

		if (!value || value->get_vertices().empty()) { return; }

		bind_vertexarray(value.get());

		if (auto const & ib{ value->get_indices() })
		{
			bind_indexbuffer(ib.get());

			for (auto const & vb : value->get_vertices())
			{
				bind_vertexbuffer(vb.get());

				draw_indexed(value->get_primitive(), ib->get_count());
			}
		}
		else
		{
			for (auto const & vb : value->get_vertices())
			{
				bind_vertexbuffer(vb.get());

				draw_arrays(value->get_primitive(), 0, vb->get_count());
			}
		}
	}

	void opengl_context::draw_arrays(uint32_t prim, size_t first, size_t count)
	{
		ML_glCheck(glDrawArrays(_primitive<to_impl>(prim), (uint32_t)first, (uint32_t)count));
	}

	void opengl_context::draw_indexed(uint32_t prim, size_t count)
	{
		ML_glCheck(glDrawElements(_primitive<to_impl>(prim), (uint32_t)count, GL_UNSIGNED_INT, nullptr));
	}

	void opengl_context::flush()
	{
		ML_glCheck(glFlush());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_context::bind_vertexarray(vertexarray const * value)
	{
		ML_glCheck(glBindVertexArray(value ? ML_handle(uint32_t, value->get_handle()) : NULL));
	}

	void opengl_context::bind_vertexbuffer(vertexbuffer const * value)
	{
		ML_glCheck(glBindBuffer(GL_ARRAY_BUFFER, value ? ML_handle(uint32_t, value->get_handle()) : NULL));
	}

	void opengl_context::bind_indexbuffer(indexbuffer const * value)
	{
		ML_glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, value ? ML_handle(uint32_t, value->get_handle()) : NULL));
	}

	void opengl_context::bind_texture(texture const * value, uint32_t slot)
	{
		ML_glCheck(glBindTextureUnit(slot, value ? ML_handle(uint32_t, value->get_handle()) : NULL));
	}

	void opengl_context::bind_framebuffer(framebuffer const * value)
	{
		if (value)
		{
			ML_glCheck(glBindFramebuffer(GL_FRAMEBUFFER, ML_handle(uint32_t, value->get_handle())));

			set_viewport({ vec2i::zero(), value->get_options().size });
		}
		else
		{
			ML_glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
		}
	}

	void opengl_context::bind_program(program const * value)
	{
		ML_glCheck(ML_glUseProgram(value ? ML_handle(uint32_t, value->get_handle()) : NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_context::upload(uniform_id loc, bool value)
	{
		ML_glCheck(ML_glUniform1i(loc, (int32_t)value));
	}

	void opengl_context::upload(uniform_id loc, int32_t value)
	{
		ML_glCheck(ML_glUniform1i(loc, value));
	}

	void opengl_context::upload(uniform_id loc, float_t value)
	{
		ML_glCheck(ML_glUniform1f(loc, value));
	}

	void opengl_context::upload(uniform_id loc, vec2f const & value)
	{
		ML_glCheck(ML_glUniform2f(loc, value[0], value[1]));
	}

	void opengl_context::upload(uniform_id loc, vec3f const & value)
	{
		ML_glCheck(ML_glUniform3f(loc, value[0], value[1], value[2]));
	}

	void opengl_context::upload(uniform_id loc, vec4f const & value)
	{
		ML_glCheck(ML_glUniform4f(loc, value[0], value[1], value[2], value[3]));
	}

	void opengl_context::upload(uniform_id loc, mat2f const & value)
	{
		ML_glCheck(ML_glUniformMatrix2fv(loc, false, value));
	}

	void opengl_context::upload(uniform_id loc, mat3f const & value)
	{
		ML_glCheck(ML_glUniformMatrix3fv(loc, false, value));
	}

	void opengl_context::upload(uniform_id loc, mat4f const & value)
	{
		ML_glCheck(ML_glUniformMatrix4fv(loc, false, value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_vertexarray::opengl_vertexarray(device * parent, uint32_t prim)
		: vertexarray{ parent }, m_primitive{ prim }
	{
		ML_glCheck(glGenVertexArrays(1, &m_handle));
		ML_glCheck(glBindVertexArray(m_handle));
	}

	opengl_vertexarray::~opengl_vertexarray()
	{
		ML_glCheck(glDeleteVertexArrays(1, &m_handle));
	}

	bool opengl_vertexarray::revalue()
	{
		if (m_handle) { ML_glCheck(glDeleteVertexArrays(1, &m_handle)); }
		
		ML_glCheck(glGenVertexArrays(1, &m_handle));

		m_vertices.clear(); m_indices.reset();

		return (bool)m_handle;
	}

	void opengl_vertexarray::add_vertices(shared<vertexbuffer> const & value)
	{
		if (!m_handle || !value) { return; }
		
		bind();
		
		value->bind();

		auto const & layout{ value->get_layout() };

		for (size_t i = 0; i < layout.elements().size(); ++i)
		{
			auto const & e{ layout.elements()[i] };

			if (uint32_t const type{ std::invoke([&]() noexcept -> uint32_t
			{
				switch (e.get_base_type())
				{
				default					: return 0			; // unknown
				case hashof_v<bool>		: return GL_BOOL	; // bool
				case hashof_v<int32_t>	: return GL_INT		; // int
				case hashof_v<float_t>	: return GL_FLOAT	; // float
				}
			}) }; type == GL_INT)
			{
				ML_glCheck(glVertexAttribIPointer(
					(uint32_t)i,
					e.get_component_count(),
					type,
					layout.stride(),
					reinterpret_cast<address_t>(e.offset)));
			}
			else
			{
				ML_glCheck(glVertexAttribPointer(
					(uint32_t)i,
					e.get_component_count(),
					type,
					e.normalized,
					layout.stride(),
					reinterpret_cast<address_t>(e.offset)));
			}
			ML_glCheck(glEnableVertexAttribArray((uint32_t)i));
		}

		m_vertices.push_back(value);
	}

	void opengl_vertexarray::set_indices(shared<indexbuffer> const & value)
	{
		bind();

		if (m_indices = value) { m_indices->bind(); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_vertexbuffer::opengl_vertexbuffer(device * parent, uint32_t usage, size_t count, address_t data)
		: vertexbuffer{ parent }, m_usage{ usage }, m_buffer{ bufcpy<float_t>(count, data) }
	{
		ML_glCheck(glGenBuffers(1, &m_handle));
		ML_glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
		ML_glCheck(glBufferData(
			GL_ARRAY_BUFFER,
			(uint32_t)m_buffer.size(),
			m_buffer.data(),
			_usage<to_impl>(m_usage)));
	}

	opengl_vertexbuffer::~opengl_vertexbuffer()
	{
		ML_glCheck(glDeleteBuffers(1, &m_handle));
	}

	bool opengl_vertexbuffer::revalue()
	{
		if (m_handle) { ML_glCheck(glDeleteBuffers(1, &m_handle)); }

		ML_glCheck(glGenBuffers(1, &m_handle));

		m_buffer.clear();

		return (bool)m_handle;
	}

	void opengl_vertexbuffer::set_data(size_t count, address_t data, size_t offset)
	{
		m_buffer = bufcpy<float_t>(count, data);
		ML_glCheck(glBufferSubData(
			GL_ARRAY_BUFFER,
			offset,
			(uint32_t)m_buffer.size(),
			m_buffer.data()));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_indexbuffer::opengl_indexbuffer(device * parent, uint32_t usage, size_t count, address_t data)
		: indexbuffer{ parent }, m_usage{ usage }, m_buffer{ bufcpy<uint32_t>(count, data) }
	{
		ML_glCheck(glGenBuffers(1, &m_handle));
		ML_glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle));
		ML_glCheck(glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			(uint32_t)m_buffer.size(),
			m_buffer.data(),
			_usage<to_impl>(m_usage)));
	}

	opengl_indexbuffer::~opengl_indexbuffer()
	{
		ML_glCheck(glDeleteBuffers(1, &m_handle));
	}

	bool opengl_indexbuffer::revalue()
	{
		if (m_handle) { ML_glCheck(glDeleteBuffers(1, &m_handle)); }
		
		ML_glCheck(glGenBuffers(1, &m_handle));

		m_buffer.clear();
		
		return (bool)m_handle;
	}

	void opengl_indexbuffer::set_data(size_t count, address_t data, size_t offset)
	{
		m_buffer = bufcpy<uint32_t>(count, data);
		ML_glCheck(glBufferSubData(
			GL_ELEMENT_ARRAY_BUFFER,
			offset,
			(uint32_t)m_buffer.size(),
			m_buffer.data()));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_texture2d::opengl_texture2d(device * parent, texopts const & opts, address_t data)
		: texture2d{ parent }, m_opts{ opts }
	{
		ML_glCheck(glGenTextures(1, &m_handle));
		ML_glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
		ML_glCheck(glTexImage2D(
			GL_TEXTURE_2D, 0,
			_format<to_impl>(m_opts.format.color),
			m_opts.size[0],
			m_opts.size[1],
			0,
			_format<to_impl>(m_opts.format.pixel),
			_type<to_impl>(m_opts.format.type),
			data));
		set_repeated(m_opts.flags & texture_flags_repeated);
		set_smooth(m_opts.flags & texture_flags_smooth);
		set_mipmapped(m_opts.flags & texture_flags_mipmapped);
	}

	opengl_texture2d::~opengl_texture2d()
	{
		ML_glCheck(glDeleteTextures(1, &m_handle));
	}

	bool opengl_texture2d::revalue()
	{
		if (!m_lock) { return debug::error("texture2d is not locked"); }

		if (m_handle) { ML_glCheck(glDeleteTextures(1, &m_handle)); }
		
		ML_glCheck(glGenTextures(1, &m_handle));

		return (bool)m_handle;
	}

	void opengl_texture2d::lock()
	{
		m_lock = true;

		debug::warning("texture lock/unlock NYI");
	}

	void opengl_texture2d::unlock()
	{
		m_lock = false;

		debug::warning("texture lock/unlock NYI");
	}

	void opengl_texture2d::update(vec2i const & size, address_t data)
	{
		if (!m_lock) { return (void)debug::error("texture2d is not locked"); }

		if (m_handle && (m_opts.size == size)) { return; }
		else { m_opts.size = size; }

		revalue(); bind();
		
		ML_glCheck(glTexImage2D(
			GL_TEXTURE_2D, 0,
			_format<to_impl>(m_opts.format.color),
			m_opts.size[0],
			m_opts.size[1],
			0,
			_format<to_impl>(m_opts.format.pixel),
			_type<to_impl>(m_opts.format.type),
			data));
		
		set_repeated(m_opts.flags & texture_flags_repeated);
		set_smooth(m_opts.flags & texture_flags_smooth);
		set_mipmapped(m_opts.flags & texture_flags_mipmapped);
	}

	void opengl_texture2d::update(vec2i const & pos, vec2i const & size, address_t data)
	{
		if (!m_lock) { return (void)debug::error("texture2d is not locked"); }

		if (m_handle && (m_opts.size == size)) { return; }
		else { m_opts.size = size; }

		revalue(); bind();

		ML_glCheck(glTexSubImage2D(
			GL_TEXTURE_2D, 0,
			pos[0],
			pos[1],
			m_opts.size[0],
			m_opts.size[1],
			_format<to_impl>(m_opts.format.color),
			_type<to_impl>(m_opts.format.type),
			data));
		
		set_repeated(m_opts.flags & texture_flags_repeated);
		set_smooth(m_opts.flags & texture_flags_smooth);
		set_mipmapped(m_opts.flags & texture_flags_mipmapped);
	}

	void opengl_texture2d::set_mipmapped(bool value)
	{
		if (!m_lock) { return (void)debug::error("texture2d is not locked"); }

		ML_flag_write(m_opts.flags, texture_flags_mipmapped, value);

		bool const smooth{ ML_flag_read(m_opts.flags, texture_flags_smooth) };

		if (smooth) { ML_glCheck(glGenerateMipmap(GL_TEXTURE_2D)); }

		ML_glCheck(glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			value
			? smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR
			: smooth ? GL_LINEAR : GL_NEAREST));
	}

	void opengl_texture2d::set_repeated(bool value)
	{
		if (!m_lock) { return (void)debug::error("texture2d is not locked"); }

		ML_flag_write(m_opts.flags, texture_flags_repeated, value);

		static bool const edge_clamp_available
		{
			get_device()->get_info().texture_edge_clamp_available
		};

		ML_glCheck(glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S,
			value
			? GL_REPEAT
			: edge_clamp_available ? GL_CLAMP_TO_EDGE : GL_CLAMP));

		ML_glCheck(glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T,
			value
			? GL_REPEAT
			: edge_clamp_available ? GL_CLAMP_TO_EDGE : GL_CLAMP));
	}

	void opengl_texture2d::set_smooth(bool value)
	{
		if (!m_lock) { return (void)debug::error("texture2d is not locked"); }

		ML_flag_write(m_opts.flags, texture_flags_smooth, value);

		ML_glCheck(glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			value ? GL_LINEAR : GL_NEAREST));

		ML_glCheck(glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			ML_flag_read(m_opts.flags, texture_flags_mipmapped)
			? value ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR
			: value ? GL_LINEAR : GL_NEAREST));
	}

	image opengl_texture2d::copy_to_image() const
	{
		if (!m_lock) { debug::error("texture2d is not locked"); return image{}; }

		image temp{ m_opts.size, calc_bits_per_pixel(m_opts.format.color) };
		if (m_handle)
		{
			bind();

			ML_glCheck(glGetTexImage(GL_TEXTURE_2D, 0,
				_format<to_impl>(m_opts.format.color),
				_type<to_impl>(m_opts.format.type),
				temp.data()));

			unbind();
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texturecube
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_texturecube::opengl_texturecube(device * parent, texopts const & opts)
		: texturecube{ parent }, m_opts{ opts }
	{
		ML_glCheck(glGenTextures(1, &m_handle));
	}

	opengl_texturecube::~opengl_texturecube()
	{
		ML_glCheck(glDeleteTextures(1, &m_handle));
	}

	bool opengl_texturecube::revalue()
	{
		if (!m_lock) { return debug::error("texturecube is not locked"); }

		if (m_handle) { ML_glCheck(glDeleteTextures(1, &m_handle)); }
		
		ML_glCheck(glGenTextures(1, &m_handle));
		
		return (bool)m_handle;
	}

	void opengl_texturecube::lock()
	{
		m_lock = true;

		debug::warning("texture lock/unlock NYI");
	}

	void opengl_texturecube::unlock()
	{
		m_lock = false;

		debug::warning("texture lock/unlock NYI");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_framebuffer::opengl_framebuffer(device * parent, texopts const & opts)
		: framebuffer{ parent }, m_opts{ opts }
	{
		resize(m_opts.size);
	}

	opengl_framebuffer::~opengl_framebuffer()
	{
		ML_glCheck(glDeleteFramebuffers(1, &m_handle));
	}

	bool opengl_framebuffer::revalue()
	{
		if (m_handle) { ML_glCheck(glDeleteFramebuffers(1, &m_handle)); }
		
		ML_glCheck(glGenFramebuffers(1, &m_handle));
		
		m_attachments.clear(); m_depth.reset();

		return (bool)m_handle;
	}

	bool opengl_framebuffer::attach(shared<texture2d> const & value)
	{
		static auto const max_color_attachments
		{
			(size_t)get_device()->get_info().max_color_attachments
		};
		
		if (m_attachments.size() < max_color_attachments &&
			!std::binary_search(m_attachments.begin(), m_attachments.end(), value)
		)
		{
			m_attachments.push_back(value);
			return true;
		}
		return false;
	}

	bool opengl_framebuffer::detach(shared<texture2d> const & value)
	{
		if (auto const it{ std::find(m_attachments.begin(), m_attachments.end(), value) }
		; it != m_attachments.end())
		{
			m_attachments.erase(it);

			return true;
		}
		return false;
	}

	void opengl_framebuffer::resize(vec2i const & size)
	{
		if (m_handle && (m_opts.size == size)) { return; }
		else { m_opts.size = size; }

		revalue(); bind();

		// color attachments
		if (m_attachments.empty()) { m_attachments.push_back(get_device()->create_texture2d(m_opts)); }
		for (size_t i = 0, imax = m_attachments.size(); i < imax; ++i)
		{
			m_attachments[i]->update(m_opts.size);

			ML_glCheck(glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + (uint32_t)i,
				GL_TEXTURE_2D,
				ML_handle(uint32_t, m_attachments[i]->get_handle()),
				0));
		}

		// depth attachment
		if (m_depth) { m_depth->update(m_opts.size); }
		else
		{
			m_depth = get_device()->create_texture2d({
				m_opts.size, {
				format_depth24_stencil8,
				format_depth_stencil,
				type_unsigned_int_24_8 },
				m_opts.flags });
		}
		ML_glCheck(glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT,
			GL_TEXTURE_2D,
			ML_handle(uint32_t, m_depth->get_handle()),
			0));

		// check status
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			debug::error("framebuffer is not complete");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_shader::opengl_shader(device * parent, uint32_t type, pmr::vector<pmr::string> const & src)
		: shader{ parent }, m_shader_type{ type }
	{
		ML_glCheck(m_handle = ML_glCreateShader(m_shader_type));

		compile(src);
	}

	opengl_shader::~opengl_shader()
	{
		ML_glCheck(ML_glDeleteShader(m_handle));
	}

	bool opengl_shader::revalue()
	{
		if (m_handle) { ML_glCheck(ML_glDeleteShader(m_handle)); }
		
		ML_glCheck(m_handle = ML_glCreateShader(m_shader_type));

		m_source.clear();
		
		return (bool)m_handle;
	}

	bool opengl_shader::compile(pmr::vector<pmr::string> const & src)
	{
		// check empty
		if ((m_source = src).empty()) { return false; }

		// compile
		cstring src_addr{ m_source.front().c_str() };
		ML_glCheck(ML_glShaderSource(m_handle, m_source.size(), &src_addr, nullptr));
		ML_glCheck(ML_glCompileShader(m_handle));

		// check errors
		m_error_log.clear();
		int32_t success{};
		ML_glCheck(ML_glGetShaderCompileStatus(m_handle, &success));
		if (!success)
		{
			// error log
			int32_t log_len{};
			ML_glCheck(ML_glGetObjectInfoLogLength(m_handle, &log_len));
			m_error_log.resize((size_t)log_len);
			ML_glCheck(ML_glGetObjectInfoLog(m_handle, log_len, &log_len, m_error_log.data()));
			m_error_log.push_back(0);
		}
		return success;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_program::opengl_uniform_binder::opengl_uniform_binder(opengl_program & p, cstring name) noexcept
	{
		if (!name || !*name || !(self = p.m_handle)) { return; }
		
		ML_glCheck(last = ML_glGetProgram());

		if (self != last) { ML_glCheck(ML_glUseProgram(self)); }

		location = p.m_uniforms.find_or_add_fn(util::hash(name, util::strlen(name)), [&
		]() noexcept
		{
			uniform_id temp{};
			ML_glCheck(temp = ML_glGetUniformLocation(self, name));
			return temp;
		});
	}

	opengl_program::opengl_uniform_binder::~opengl_uniform_binder() noexcept
	{
		if (self && (self != last))
		{
			ML_glCheck(ML_glUseProgram(last));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_program::opengl_program(device * parent)
		: program{ parent }
	{
		ML_glCheck(m_handle = ML_glCreateProgram());
	}

	opengl_program::~opengl_program()
	{
		ML_glCheck(ML_glDeleteProgram(m_handle));
	}

	bool opengl_program::revalue()
	{
		if (m_handle) { ML_glCheck(ML_glDeleteProgram(m_handle)); }
		
		ML_glCheck(m_handle = ML_glCreateProgram());
		
		m_uniforms.clear(); m_textures.clear(); m_shaders.clear();
		
		return (bool)m_handle;
	}

	bool opengl_program::attach(shared<shader> const & value)
	{
		if (m_shaders.try_emplace(value->get_shader_type(), value).second)
		{
			if (m_handle && value)
			{
				ML_glCheck(ML_glAttachShader(m_handle, value->get_handle()));
			}

			return true;
		}
		return false;
	}

	bool opengl_program::detach(shared<shader> const & value)
	{
		if (auto const it{ m_shaders.find(value->get_shader_type()) })
		{
			if (m_handle && value)
			{
				ML_glCheck(ML_glDetachShader(m_handle, value->get_handle()));
			}

			m_shaders.erase(it->first);

			return true;
		}
		return false;
	}

	bool opengl_program::link()
	{
		// link
		ML_glCheck(ML_glLinkProgram(m_handle));

		// check errors
		m_error_log.clear();
		int32_t success{};
		ML_glCheck(ML_glGetProgramLinkStatus(m_handle, &success));
		if (!success)
		{
			// error log
			int32_t log_len{};
			ML_glCheck(ML_glGetObjectInfoLogLength(m_handle, &log_len));
			m_error_log.resize((size_t)log_len);
			ML_glCheck(ML_glGetObjectInfoLog(m_handle, log_len, &log_len, m_error_log.data()));
			m_error_log.push_back(0);
		}
		return success;
	}

	bool opengl_program::validate()
	{
		// validate
		ML_glCheck(ML_glValidateProgram(m_handle));

		// check errors
		m_error_log.clear();
		int32_t success{};
		ML_glCheck(ML_glGetProgramValidateStatus(m_handle, &success));
		if (!success)
		{
			// error log
			int32_t log_len{};
			ML_glCheck(ML_glGetObjectInfoLogLength(m_handle, &log_len));
			m_error_log.resize((size_t)log_len);
			ML_glCheck(ML_glGetObjectInfoLog(m_handle, log_len, &log_len, m_error_log.data()));
			m_error_log.push_back(0);
		}
		return success;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_IMPL_RENDERER_OPENGL