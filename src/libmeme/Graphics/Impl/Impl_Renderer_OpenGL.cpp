#if defined(ML_IMPL_RENDERER_OPENGL)

#include "Impl_Renderer_OpenGL.hpp"

namespace ml { using namespace gl; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_OPENGL_LOADER_CUSTOM
//	custom
#	if (defined(__has_include) && __has_include(ML_OPENGL_LOADER_CUSTOM))
#		include ML_OPENGL_LOADER_CUSTOM
#	endif
#elif defined(ML_IMPL_OPENGL_ES2)
//	GLES2
#	include <GLES2/gl2.h>
#elif defined(ML_IMPL_OPENGL_ES3)
//	GLES3
#	if defined(ML_os_apple && (TARGET_OS_IOS || TARGET_OS_TV))
#		include <OpenGLES/ES3/gl.h>
#	else
#		include <GLES3/gl3.h>
#	endif
#elif defined(ML_IMPL_OPENGL_LOADER_GLEW)
//	glew
#	include <GL/glew.h>
#elif defined(ML_IMPL_OPENGL_LOADER_GL3W)
//	gl3w
#	include <GL/gl3w.h>
#elif defined(ML_IMPL_OPENGL_LOADER_GLAD)
//	glad
#	include <glad/glad.h>
#else
#	error "Unknown or invalid opengl loader specified."
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// enums
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _buffer_bit(uint32_t value) noexcept
	{
		uint32_t temp{};
		if constexpr (Convt()) // to impl
		{
			ML_flag_map(temp, GL_COLOR_BUFFER_BIT	, value, color_bit);
			ML_flag_map(temp, GL_DEPTH_BUFFER_BIT	, value, depth_bit);
			ML_flag_map(temp, GL_STENCIL_BUFFER_BIT	, value, stencil_bit);
		}
		else // to user
		{
			ML_flag_map(temp, color_bit		, value, GL_COLOR_BUFFER_BIT);
			ML_flag_map(temp, depth_bit		, value, GL_DEPTH_BUFFER_BIT);
			ML_flag_map(temp, stencil_bit	, value, GL_STENCIL_BUFFER_BIT);
		}
		return temp;
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

	template <class Convt> constexpr uint32_t _usage(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default					: return value;
			case usage_stream_draw	: return GL_STREAM_DRAW;
			case usage_static_draw	: return GL_STATIC_DRAW;
			case usage_dynamic_draw	: return GL_DYNAMIC_DRAW;
			}
		}
		else // to user
		{
			switch (value)
			{
			default					: return value;
			case GL_STREAM_DRAW		: return usage_stream_draw;
			case GL_STATIC_DRAW		: return usage_static_draw;
			case GL_DYNAMIC_DRAW	: return usage_dynamic_draw;
			}
		}
	}

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

	template <class Convt> constexpr uint32_t _function(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default							: return value;
			case function_add				: return GL_FUNC_ADD;
			case function_subtract			: return GL_FUNC_SUBTRACT;
			case function_reverse_subtract	: return GL_FUNC_REVERSE_SUBTRACT;
			case function_min				: return GL_MIN;
			case function_max				: return GL_MAX;
			}
		}
		else // to user
		{
			switch (value)
			{
			default							: return value;
			case GL_FUNC_ADD				: return function_add;
			case GL_FUNC_SUBTRACT			: return function_subtract;
			case GL_FUNC_REVERSE_SUBTRACT	: return function_reverse_subtract;
			case GL_MIN						: return function_min;
			case GL_MAX						: return function_max;
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

	template <class Convt> constexpr uint32_t _texture(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default					: return value;
			case texture_type_1d			: return GL_TEXTURE_1D;
			case texture_type_2d			: return GL_TEXTURE_2D;
			case texture_type_3d			: return GL_TEXTURE_3D;
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

	template <class Convt> constexpr uint32_t _shader(uint32_t value) noexcept
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// check error macro
#if (!ML_is_debug)
#	define glCheck(expr) (expr)
#else
#	define glCheck(expr) \
	do { expr; _ML gl::device::check_error(__FILE__, __LINE__, #expr); } while (0)
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_device::do_initialize()
	{
#if defined(ML_IMPL_OPENGL_LOADER_GLEW)
		// glew
		glewExperimental = true;
		return (GLEW_OK == glewInit());

#elif defined(ML_IMPL_OPENGL_LOADER_GL3W)
		// gl3w
		return gl3wInit();

#elif defined(ML_IMPL_OPENGL_LOADER_GLAD)
		// glad
		return gladLoadGL();

#elif defined(ML_IMPL_OPENGL_LOADER_CUSTOM)
		// custom
		return false;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t opengl_device::get_error() const noexcept
	{
		return _error<to_user>(glGetError());
	}

	device::info const & opengl_device::get_info() const
	{
		static device::info temp{};
		static ML_scope // once
		{
			// renderer
			glCheck(temp.renderer = (cstring)glGetString(GL_RENDERER));
			
			// vendor
			glCheck(temp.vendor = (cstring)glGetString(GL_VENDOR));
			
			// version
			glCheck(temp.version = (cstring)glGetString(GL_VERSION));

			// shading language version
			glCheck(temp.shading_language_version = (cstring)glGetString(GL_SHADING_LANGUAGE_VERSION));

			// major version
			if (glGetIntegerv(GL_MAJOR_VERSION, &temp.major_version); glGetError() == GL_INVALID_ENUM)
			{
				temp.major_version = std::invoke([&]() noexcept -> int32_t
				{
					return !temp.version.empty() ? temp.version[0] - '0' : 1;
				});
			}

			// minor version
			if (glGetIntegerv(GL_MINOR_VERSION, &temp.minor_version); glGetError() == GL_INVALID_ENUM)
			{
				temp.minor_version = std::invoke([]() noexcept -> int32_t
				{
					return !temp.version.empty() ? temp.version[2] - '0' : 1;
				});
			}

			// extensions
			{
				int32_t num{};
				glGetIntegerv(GL_NUM_EXTENSIONS, &num);
				temp.extensions.reserve(num);

				pmr::stringstream ss{ (cstring)glGetString(GL_EXTENSIONS) };
				pmr::string line{};
				while (std::getline(ss, line, ' '))
				{
					temp.extensions.push_back(line);
				}
			}

			// edge clamp available
#if defined(GL_EXT_texture_edge_clamp) \
|| defined(GLEW_EXT_texture_edge_clamp) \
|| defined(GL_SGIS_texture_edge_clamp)
			temp.texture_edge_clamp_available = true;
#endif
			// max texture slots
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (int32_t *)&temp.max_texture_slots);

			// shaders available
#if defined(GL_ARB_multitexture) \
|| defined(GL_ARB_shading_language_100) \
|| defined(GL_ARB_shader_objects) \
|| defined(GL_ARB_vertex_shader) \
|| defined(GL_ARB_fragment_shader)
			temp.shaders_available = true;

			// geometry shaders available
#	ifdef GL_ARB_geometry_shader4
			temp.geometry_shaders_available = true;
#	endif
#endif
			// shader binary formats
			{
				int32_t num{};
				glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &num);
				temp.shader_binary_formats.reserve((size_t)num);
				glGetIntegerv(GL_SHADER_BINARY_FORMATS, (int32_t *)temp.shader_binary_formats.data());
			}

		};
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_device::get_alpha_enabled() const
	{
		bool temp{};
		glCheck(glGetBooleanv(GL_ALPHA_TEST, (uint8_t *)&temp));
		return temp;
	}

	alpha_fn opengl_device::get_alpha_fn() const
	{
		return
		{
			std::invoke([]() -> uint32_t
			{
				uint32_t func{};
				glCheck(glGetIntegerv(GL_ALPHA_TEST_FUNC, (int32_t *)&func));
				return _predicate<to_user>(func);
			}),
			std::invoke([]() -> float_t
			{
				float_t ref{};
				glCheck(glGetFloatv(GL_ALPHA_TEST_REF, &ref));
				return ref;
			})
		};
	}

	bool opengl_device::get_blend_enabled() const
	{
		bool temp{};
		glCheck(glGetBooleanv(GL_BLEND, (uint8_t *)&temp));
		return temp;
	}

	color opengl_device::get_blend_color() const
	{
		color temp{};
		glCheck(glGetFloatv(GL_BLEND_COLOR, temp));
		return temp;
	}

	blend_eq opengl_device::get_blend_eq() const
	{
		blend_eq temp{};
		
		glCheck(glGetIntegerv(GL_BLEND_EQUATION_RGB, (int32_t *)&temp.modeRGB));
		temp.modeRGB = _function<to_user>(temp.modeRGB);
		
		glCheck(glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (int32_t *)&temp.modeAlpha));
		temp.modeAlpha = _function<to_user>(temp.modeAlpha);
		
		return temp;
	}

	blend_fn opengl_device::get_blend_fn() const
	{
		blend_fn temp{};

		glCheck(glGetIntegerv(GL_BLEND_SRC_RGB, (int32_t *)&temp.sfactorRGB));
		temp.sfactorRGB = _factor<to_user>(temp.sfactorRGB);

		glCheck(glGetIntegerv(GL_BLEND_SRC_ALPHA, (int32_t *)&temp.sfactorAlpha));
		temp.sfactorAlpha = _factor<to_user>(temp.sfactorAlpha);

		glCheck(glGetIntegerv(GL_BLEND_DST_RGB, (int32_t *)&temp.dfactorRGB));
		temp.dfactorRGB = _factor<to_user>(temp.dfactorRGB);

		glCheck(glGetIntegerv(GL_BLEND_DST_ALPHA, (int32_t *)&temp.dfactorAlpha));
		temp.dfactorAlpha = _factor<to_user>(temp.dfactorAlpha);

		return temp;
	}

	color opengl_device::get_clear_color() const
	{
		color temp{};
		glCheck(glGetFloatv(GL_COLOR_CLEAR_VALUE, temp));
		return temp;
	}

	bool opengl_device::get_cull_enabled() const
	{
		bool temp{};
		glCheck(glGetBooleanv(GL_CULL_FACE, (uint8_t *)&temp));
		return temp;
	}

	uint32_t opengl_device::get_cull_facet() const
	{
		uint32_t temp{};
		glCheck(glGetIntegerv(GL_CULL_FACE_MODE, (int32_t *)&temp));
		return _facet<to_user>(temp);
	}

	uint32_t opengl_device::get_cull_order() const
	{
		uint32_t temp{};
		glCheck(glGetIntegerv(GL_FRONT_FACE, (int32_t *)&temp));
		return _order<to_user>(temp);
	}

	bool opengl_device::get_depth_enabled() const
	{
		bool temp{};
		glCheck(glGetBooleanv(GL_DEPTH_TEST, (uint8_t *)&temp));
		return temp;
	}

	bool opengl_device::get_depth_mask() const
	{
		bool temp{};
		glCheck(glGetBooleanv(GL_DEPTH_WRITEMASK, (uint8_t *)&temp));
		return temp;
	}

	uint32_t opengl_device::get_depth_pr() const
	{
		uint32_t temp{};
		glCheck(glGetIntegerv(GL_DEPTH_FUNC, (int32_t *)&temp));
		return _predicate<to_user>(temp);
	}

	depth_range opengl_device::get_depth_range() const
	{
		depth_range temp{};
		glCheck(glGetFloatv(GL_DEPTH_RANGE, &temp.nearVal));
		return temp;
	}

	bool opengl_device::get_stencil_enabled() const
	{
		bool temp{};
		glCheck(glGetBooleanv(GL_STENCIL_TEST, (uint8_t *)&temp));
		return temp;
	}

	stencil_fn opengl_device::get_stencil_fn() const
	{
		stencil_fn temp{};

		glCheck(glGetIntegerv(GL_STENCIL_FUNC, (int32_t *)&temp.pred));
		temp.pred = _predicate<to_user>(temp.pred);

		glCheck(glGetIntegerv(GL_STENCIL_REF, &temp.ref));
		
		glCheck(glGetIntegerv(GL_STENCIL_VALUE_MASK, (int32_t *)&temp.mask));

		return temp;
	}

	int_rect opengl_device::get_viewport() const
	{
		int_rect temp{};
		glCheck(glGetIntegerv(GL_VIEWPORT, temp));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_device::set_alpha_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_ALPHA_TEST));
	}

	void opengl_device::set_alpha_fn(alpha_fn const & value)
	{
		glCheck(glAlphaFunc(_predicate<to_impl>(value.pred), value.ref));
	}

	void opengl_device::set_blend_color(color const & value)
	{
		glCheck(glBlendColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_device::set_blend_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_BLEND));
	}

	void opengl_device::set_blend_eq(blend_eq const & value)
	{
		glCheck(glBlendEquationSeparate(
			_function<to_impl>(value.modeRGB),
			_function<to_impl>(value.modeAlpha)));
	}

	void opengl_device::set_blend_fn(blend_fn const & value)
	{
		glCheck(glBlendFuncSeparate(
			_factor<to_impl>(value.sfactorRGB),
			_factor<to_impl>(value.dfactorRGB),
			_factor<to_impl>(value.sfactorAlpha),
			_factor<to_impl>(value.dfactorAlpha)));
	}

	void opengl_device::set_clear_color(color const & value)
	{
		glCheck(glClearColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_device::set_cull_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_CULL_FACE));
	}

	void opengl_device::set_cull_facet(uint32_t facet)
	{
		glCheck(glCullFace(_facet<to_impl>(facet)));
	}

	void opengl_device::set_cull_order(uint32_t order)
	{
		glCheck(glFrontFace(_order<to_impl>(order)));
	}

	void opengl_device::set_depth_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_DEPTH_TEST));
	}

	void opengl_device::set_depth_mask(bool enabled)
	{
		glCheck(glDepthMask(enabled));
	}

	void opengl_device::set_depth_pr(uint32_t predicate)
	{
		glCheck(glDepthFunc(_predicate<to_impl>(predicate)));
	}

	void opengl_device::set_depth_range(depth_range const & value)
	{
		glCheck(glDepthRangef(value.nearVal, value.farVal));
	}

	void opengl_device::set_stencil_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : glDisable)(GL_STENCIL_TEST));
	}

	void opengl_device::set_stencil_fn(stencil_fn const & value)
	{
		glCheck(glStencilFunc(
			_predicate<to_impl>(value.pred),
			value.ref,
			value.mask));
	}

	void opengl_device::set_viewport(int_rect const & bounds)
	{
		glCheck(glViewport(bounds[0], bounds[1], bounds[2], bounds[3]));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_device::clear(uint32_t flags)
	{
		glCheck(glClear(_buffer_bit<to_impl>(flags)));
	}

	void opengl_device::draw(shared<vertexarray> const & value)
	{
		if (!value) { return; }

		value->bind();

		if (value->get_ib())
		{
			value->get_ib()->bind();

			for (auto const & vb : value->get_vbs())
			{
				vb->bind();

				draw_indexed(value->get_ib()->get_count());
			}
		}
		else
		{
			for (auto const & vb : value->get_vbs())
			{
				vb->bind();

				draw_arrays(0, vb->get_count());
			}
		}
	}

	void opengl_device::draw_arrays(size_t first, size_t count)
	{
		glCheck(glDrawArrays(GL_TRIANGLES, (uint32_t)first, (uint32_t)count));
	}

	void opengl_device::draw_indexed(size_t count)
	{
		glCheck(glDrawElements(GL_TRIANGLES, (uint32_t)count, GL_UNSIGNED_INT, nullptr));
	}

	void opengl_device::flush()
	{
		glCheck(glFlush());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_device::upload(handle_t loc, int32_t const & value)
	{
		glCheck(glUniform1iARB((int32_t)(intptr_t)loc, value));
	}

	void opengl_device::upload(handle_t loc, float_t const & value)
	{
		glCheck(glUniform1fARB((int32_t)(intptr_t)loc, value));
	}

	void opengl_device::upload(handle_t loc, vec2 const & value)
	{
		glCheck(glUniform2fARB((int32_t)(intptr_t)loc, value[0], value[1]));
	}

	void opengl_device::upload(handle_t loc, vec3 const & value)
	{
		glCheck(glUniform3fARB((int32_t)(intptr_t)loc, value[0], value[1], value[2]));
	}

	void opengl_device::upload(handle_t loc, vec4 const & value)
	{
		glCheck(glUniform4fARB((int32_t)(intptr_t)loc, value[0], value[1], value[2], value[3]));
	}

	void opengl_device::upload(handle_t loc, mat2 const & value)
	{
		glCheck(glUniformMatrix2fvARB((int32_t)(intptr_t)loc, 2 * 2, false, value));
	}

	void opengl_device::upload(handle_t loc, mat3 const & value)
	{
		glCheck(glUniformMatrix3fvARB((int32_t)(intptr_t)loc, 3 * 3, false, value));
	}

	void opengl_device::upload(handle_t loc, mat4 const & value)
	{
		glCheck(glUniformMatrix4fvARB((int32_t)(intptr_t)loc, 4 * 4, false, value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gl
{
	vertexarray const & opengl_vertexarray::bind() const
	{
		glCheck(glBindVertexArray(m_handle));
		return (*this);
	}

	vertexarray const & opengl_vertexarray::unbind() const
	{
		glCheck(glBindVertexArray(NULL));
		return (*this);
	}

	vertexarray & opengl_vertexarray::generate()
	{
		if (!m_handle)
		{
			glCheck(glGenVertexArrays(1, &m_handle));
		}
		return (*this);
	}

	vertexarray & opengl_vertexarray::destroy()
	{
		if (m_handle)
		{
			glCheck(glDeleteVertexArrays(1, &m_handle));
			m_handle = NULL;
		}
		return (*this);
	}

	vertexarray & opengl_vertexarray::add_vb(shared<vertexbuffer> const & value) &
	{
		if (!value || !*value) { return (*this); }
		
		bind(); m_vertices.emplace_back(value)->bind();

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
				glCheck(glVertexAttribIPointer(
					(uint32_t)i,
					e.get_component_count(),
					type,
					layout.stride(),
					reinterpret_cast<buffer_t>(e.offset)));
			}
			else
			{
				glCheck(glVertexAttribPointer(
					(uint32_t)i,
					e.get_component_count(),
					type,
					e.normalized,
					layout.stride(),
					reinterpret_cast<buffer_t>(e.offset)));
			}
			glCheck(glEnableVertexAttribArray((uint32_t)i));
		}

		return (*this);
	}

	vertexarray & opengl_vertexarray::set_ib(shared<indexbuffer> const & value) &
	{
		bind();

		if (m_indices = value) { m_indices->bind(); }

		return (*this);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gl
{
	vertexbuffer const & opengl_vertexbuffer::bind() const
	{
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
		return (*this);
	}

	vertexbuffer const & opengl_vertexbuffer::unbind() const
	{
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, NULL));
		return (*this);
	}

	vertexbuffer & opengl_vertexbuffer::generate()
	{
		if (!m_handle)
		{
			glCheck(glGenBuffers(1, &m_handle));
		}
		return (*this);
	}

	vertexbuffer & opengl_vertexbuffer::destroy()
	{
		if (m_handle)
		{
			glCheck(glDeleteBuffers(1, &m_handle));
			m_handle = NULL;
		}
		return (*this);
	}

	vertexbuffer & opengl_vertexbuffer::set_data(buffer_t data, size_t count)
	{
		copy_buffer<float_t>(m_view, data, count);

		glCheck(glBufferData(
			GL_ARRAY_BUFFER,
			(uint32_t)(m_count = count) * sizeof(float_t),
			data,
			_usage<to_impl>(m_usage)));

		return (*this);
	}

	vertexbuffer & opengl_vertexbuffer::set_data(buffer_t data, size_t count, size_t offset)
	{
		copy_buffer<float_t>(m_view, data, count);

		glCheck(glBufferSubData(
			GL_ARRAY_BUFFER,
			offset,
			(uint32_t)(m_count = count) * sizeof(float_t),
			data));

		return (*this);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gl
{
	indexbuffer const & opengl_indexbuffer::bind() const
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle));
		return (*this);
	}

	indexbuffer const & opengl_indexbuffer::unbind() const
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));
		return (*this);
	}

	indexbuffer & opengl_indexbuffer::generate()
	{
		if (!m_handle)
		{
			glCheck(glGenBuffers(1, &m_handle));
		}
		return (*this);
	}

	indexbuffer & opengl_indexbuffer::destroy()
	{
		if (m_handle)
		{
			glCheck(glDeleteBuffers(1, &m_handle));
			m_handle = NULL;
		}
		return (*this);
	}

	indexbuffer & opengl_indexbuffer::set_data(buffer_t data, size_t count)
	{
		copy_buffer<uint32_t>(m_view, data, count);

		glCheck(glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			(uint32_t)(m_count = count) * sizeof(uint32_t),
			data,
			_usage<to_impl>(m_usage)));

		return (*this);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gl
{
	framebuffer const & opengl_framebuffer::bind() const
	{
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_handle));
		glCheck(glViewport(0, 0, m_size[0], m_size[1]));
		return (*this);
	}

	framebuffer const & opengl_framebuffer::unbind() const
	{
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
		return (*this);
	}

	framebuffer & opengl_framebuffer::generate()
	{
		if (!m_handle)
		{
			glCheck(glGenFramebuffers(1, &m_handle));
		}
		return (*this);
	}

	framebuffer & opengl_framebuffer::destroy()
	{
		if (m_handle)
		{
			glCheck(glDeleteFramebuffers(1, &m_handle));
			m_handle = NULL;
		}
		return (*this);
	}

	framebuffer const & opengl_framebuffer::bind_texture(uint32_t slot) const &
	{
		glCheck(glActiveTexture(GL_TEXTURE0 + slot));
		m_color->bind();
		return (*this);
	}

	framebuffer & opengl_framebuffer::resize(vec2i const & size) &
	{
		if (m_size == size) { return (*this); }
		else { m_size = size; }

		destroy().generate().bind();

		// color attachment
		if (m_color) { m_color->update(m_size); }
		else
		{
			m_color = make_texture2d(
				m_size,
				m_format,
				m_format,
				type_unsigned_byte);
		}
		glCheck(glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, (uint32_t)(intptr_t)m_color->get_handle(), 0));

		// depth attachment
		if (m_depth) { m_depth->update(m_size); }
		else
		{
			m_depth = make_texture2d(
				m_size,
				format_depth24_stencil8,
				format_depth_stencil,
				type_unsigned_int_24_8);
		}
		glCheck(glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			GL_TEXTURE_2D, (uint32_t)(intptr_t)m_depth->get_handle(), 0));

		// check status
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			debug::error("framebuffer is not complete");
		}

		return (*this);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gl
{
	texture2d const & opengl_texture2d::bind() const
	{
		glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
		return (*this);
	}

	texture2d const & opengl_texture2d::unbind() const
	{
		glCheck(glBindTexture(GL_TEXTURE_2D, NULL));
		return (*this);
	}

	texture2d & opengl_texture2d::generate()
	{
		if (!m_handle)
		{
			glCheck(glGenTextures(1, &m_handle));
		}
		return (*this);
	}

	texture2d & opengl_texture2d::destroy()
	{
		if (m_handle)
		{
			glCheck(glDeleteTextures(1, &m_handle));
			m_handle = NULL;
		}
		return (*this);
	}

	texture2d & opengl_texture2d::set_mipmapped(bool value) &
	{
		ML_flag_write(m_flags, texture_flags_mipmapped, value);

		auto const is_smooth{ ML_flag_read(m_flags, texture_flags_smooth) };

		if (is_smooth)
		{
			glCheck(glGenerateMipmap(GL_TEXTURE_2D));
		}

		glCheck(glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			value
			? is_smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR
			: is_smooth ? GL_LINEAR : GL_NEAREST));

		return (*this);
	}

	texture2d & opengl_texture2d::set_repeated(bool value) &
	{
		ML_flag_write(m_flags, texture_flags_repeated, value);

		static bool const edge_clamp_available
		{
			device::get()->get_info().texture_edge_clamp_available
		};

		glCheck(glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S,
			value
			? GL_REPEAT
			: edge_clamp_available ? GL_CLAMP_TO_EDGE : GL_CLAMP));

		glCheck(glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T,
			value
			? GL_REPEAT
			: edge_clamp_available ? GL_CLAMP_TO_EDGE : GL_CLAMP));

		return (*this);
	}

	texture2d & opengl_texture2d::set_smooth(bool value) &
	{
		ML_flag_write(m_flags, texture_flags_smooth, value);

		glCheck(glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			value ? GL_LINEAR : GL_NEAREST));

		glCheck(glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			ML_flag_read(m_flags, texture_flags_mipmapped)
			? value ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR
			: value ? GL_LINEAR : GL_NEAREST));

		return (*this);
	}

	texture2d & opengl_texture2d::update(vec2i const & size, buffer_t data) &
	{
		if (m_size == size) { return (*this); }
		else { m_size = size; }

		destroy().generate().bind();

		glCheck(glTexImage2D(
			GL_TEXTURE_2D, 0,
			_format<to_impl>(m_i_format), m_size[0], m_size[1], 0,
			_format<to_impl>(m_c_format),
			_type<to_impl>(m_p_type),
			data));

		set_repeated(ML_flag_read(m_flags, texture_flags_repeated));
		
		set_smooth(ML_flag_read(m_flags, texture_flags_smooth));
		
		set_mipmapped(ML_flag_read(m_flags, texture_flags_mipmapped));

		return (*this);
	}

	image opengl_texture2d::copy_to_image() const
	{
		image temp{ m_size, std::invoke([channels = m_i_format]() noexcept -> size_t
		{
			switch (channels)
			{
			case format_red		: return 1;
			case format_rgb		: return 3;
			case format_rgba	:
			default				: return 4;
			}
		}) };
		if (m_handle)
		{
			ML_bind_scope(*this);

			glCheck(glGetTexImage(
				GL_TEXTURE_2D,
				0,
				_format<to_impl>(m_i_format),
				_type<to_impl>(m_p_type),
				temp.data()));
		}
		return temp;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gl
{
	program & opengl_program::generate()
	{
		if (!m_handle)
		{
			glCheck(m_handle = glCreateShaderObjectARB(_shader<to_impl>(m_type)));
		}
		return (*this);
	}

	program & opengl_program::destroy()
	{
		if (m_handle)
		{
			glCheck(glDeleteObjectARB(m_handle));
			m_handle = NULL;
		}
		return (*this);
	}

	int32_t opengl_program::compile(size_t count, cstring const * src)
	{
		if (!count || !src || !*src) { return -1; } // no source

		// compile
		glCheck(m_handle = glCreateShaderObjectARB(_shader<to_impl>(m_type)));
		glCheck(glShaderSource(m_handle, (uint32_t)count, src, nullptr));
		glCheck(glCompileShaderARB(m_handle));

		// check errors
		glCheck(glGetObjectParameterivARB(m_handle, GL_OBJECT_COMPILE_STATUS_ARB, &m_status));
		if (m_status == GL_FALSE)
		{
			// error log
			int32_t log_len{};
			glCheck(glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &log_len));
			m_log.resize((size_t)log_len);
			glCheck(glGetInfoLogARB(m_handle, log_len, &log_len, m_log.data()));
			m_log.push_back(0);
			
			debug::error(m_log.data());

			// destroy
			destroy();
		}
		return m_status;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gl
{
	struct opengl_shader::uniform_binder final
	{
		int32_t location{ -1 }; uint32_t current{}, previous{};

		template <class Fn
		> uniform_binder(opengl_shader & self, cstring name, Fn && fn) noexcept
		{
			if (!(current = self.m_handle)) { return; }

			glCheck(previous = glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
			
			if (current != previous)
			{
				glCheck(glUseProgramObjectARB(current));
			}

			if (-1 < (location = self.m_uniforms.find_or_add_fn
			(util::hash(name, std::strlen(name)), [&]() noexcept
			{
				int32_t temp{};
				glCheck(temp = glGetUniformLocationARB(current, name));
				return temp;
			})))
			{
				std::invoke(ML_forward(fn), ML_addressof(location));
			}
		}

		~uniform_binder() noexcept
		{
			if (current && (current != previous))
			{
				glCheck(glUseProgramObjectARB(previous));
			}
		}
	};

	shader const & opengl_shader::bind() const
	{
		glCheck(glUseProgramObjectARB(m_handle));
		return (*this);
	}

	shader const & opengl_shader::unbind() const
	{
		glCheck(glUseProgramObjectARB(NULL));
		return (*this);
	}

	shader & opengl_shader::generate()
	{
		if (!m_handle)
		{
			glCheck(m_handle = glCreateProgramObjectARB());
		}
		return (*this);
	}

	shader & opengl_shader::destroy()
	{
		if (m_handle)
		{
			glCheck(glDeleteObjectARB(m_handle));
			m_handle = NULL;
			m_uniforms.clear();
			m_textures.clear();
		}
		return (*this);
	}

	int32_t opengl_shader::attach(shared<program> const & value)
	{
		if (!value || !*value) { return 0; }
		else
		{
			glCheck(glAttachObjectARB(m_handle, (uint32_t)(intptr_t)value->get_handle()));

			return value->get_status();
		}
	}

	int32_t opengl_shader::link()
	{
		if (!m_handle) { return 0; }

		glCheck(glLinkProgramARB(m_handle));

		int32_t status{};
		glCheck(glGetObjectParameterivARB(m_handle, GL_OBJECT_LINK_STATUS_ARB, &status));
		if (status == GL_FALSE)
		{
			destroy();
		}
		
		return status;
	}

	shader const & opengl_shader::bind_textures() const
	{
		int32_t index{};
		m_textures.for_each([&](handle_t loc, handle_t value)
		{
			device::get()->upload(loc, index);
			glCheck(glActiveTexture(GL_TEXTURE0 + index));
			glCheck(glBindTexture(GL_TEXTURE_2D, (uint32_t)(intptr_t)value));
			index++;
		});
		return (*this);
	}

	bool opengl_shader::bind_uniform(cstring name, std::function<void(handle_t)> const & callback)
	{
		return -1 < uniform_binder{ *this, name, callback }.location;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // ML_IMPL_RENDERER_OPENGL