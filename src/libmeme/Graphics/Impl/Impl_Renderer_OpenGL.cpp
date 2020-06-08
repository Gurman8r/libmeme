#if defined(ML_IMPL_RENDERER_OPENGL)

#include "Impl_Renderer_OpenGL.hpp"

#ifdef ML_OPENGL_LOADER_CUSTOM
#	if (defined(__has_include) && __has_include(ML_OPENGL_LOADER_CUSTOM))
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
#	error "Unknown or invalid opengl loader specified."
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::gfx
{
	// check error
	static void check_error(cstring file, size_t line, cstring expr) noexcept
	{
		if (auto const code{ device::get_context()->get_error() }; code != error_none)
		{
			std::cout
				<< "An internal OpenGL error occurred ( " << code << " )\n"
				<< "|  " << fs::path{ file }.filename() << " (" << line << ")\n"
				<< "| expression:\n"
				<< "|  " << expr << "\n"
				<< "| description:\n"
				<< "|  " << error_names[code] << "\n"
				<< "|  " << error_descriptions[code] << "\n"
				<< "\n";
		}
	}
}

// check error macro
#if ML_is_debug
#	define glCheck(expr) do{ (expr); _ML gfx::check_error(__FILE__, __LINE__, #expr); } while(0)
#else
#	define glCheck(expr) (expr)
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// enums
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _buffer_bit(uint32_t value) noexcept
	{
		uint32_t temp{};
		if constexpr (Convt()) // to impl
		{
			ML_flag_map(temp, GL_COLOR_BUFFER_BIT	, value, color_buffer_bit);
			ML_flag_map(temp, GL_DEPTH_BUFFER_BIT	, value, depth_buffer_bit);
			ML_flag_map(temp, GL_STENCIL_BUFFER_BIT	, value, stencil_buffer_bit);
		}
		else // to user
		{
			ML_flag_map(temp, color_buffer_bit		, value, GL_COLOR_BUFFER_BIT);
			ML_flag_map(temp, depth_buffer_bit		, value, GL_DEPTH_BUFFER_BIT);
			ML_flag_map(temp, stencil_buffer_bit	, value, GL_STENCIL_BUFFER_BIT);
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _error_type(uint32_t value) noexcept
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
			case stream_draw	: return GL_STREAM_DRAW;
			case static_draw	: return GL_STATIC_DRAW;
			case dynamic_draw	: return GL_DYNAMIC_DRAW;
			}
		}
		else // to user
		{
			switch (value)
			{
			default					: return value;
			case GL_STREAM_DRAW		: return stream_draw;
			case GL_STATIC_DRAW		: return static_draw;
			case GL_DYNAMIC_DRAW	: return dynamic_draw;
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

	template <class Convt> constexpr uint32_t _texture_type(uint32_t value) noexcept
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

	template <class Convt> constexpr uint32_t _shader_type(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to impl
		{
			switch (value)
			{
			default						: return value;
			case vertex_shader		: return GL_VERTEX_SHADER;
			case fragment_shader	: return GL_FRAGMENT_SHADER;
			case geometry_shader	: return GL_GEOMETRY_SHADER;
			}
		}
		else // to user
		{
			switch (value)
			{
			default					: return value;
			case GL_VERTEX_SHADER	: return vertex_shader;
			case GL_FRAGMENT_SHADER	: return fragment_shader;
			case GL_GEOMETRY_SHADER	: return geometry_shader;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_device::do_initialize()
	{
#if defined(ML_IMPL_OPENGL_LOADER_GLEW)
		// glew
		glewExperimental = true;
		return glewInit() == GLEW_OK;

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

	devinfo const & opengl_device::get_devinfo() const noexcept
	{
		static devinfo temp{};
		static ML_scope // once
		{
			// renderer
			glCheck(temp.renderer = (cstring)glGetString(GL_RENDERER));
			
			// vendor
			glCheck(temp.vendor = (cstring)glGetString(GL_VENDOR));
			
			// version
			glCheck(temp.version = (cstring)glGetString(GL_VERSION));

			// major version
			if (glGetIntegerv(GL_MAJOR_VERSION, &temp.major_version); glGetError() == GL_INVALID_ENUM)
			{
				temp.major_version = !temp.version.empty() ? temp.version[0] - '0' : 1;
			}

			// minor version
			if (glGetIntegerv(GL_MINOR_VERSION, &temp.minor_version); glGetError() == GL_INVALID_ENUM)
			{
				temp.minor_version = !temp.version.empty() ? temp.version[2] - '0' : 1;
			}

			// extensions
			{
				int32_t num{};
				glGetIntegerv(GL_NUM_EXTENSIONS, &num);
				temp.extensions.reserve(num);
				
				pmr::stringstream ss{};
				glCheck(ss.str((cstring)glGetString(GL_EXTENSIONS)));

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
#if defined(GL_ARB_shading_language_100) \
|| defined(GL_ARB_shader_objects) \
|| defined(GL_ARB_vertex_shader) \
|| defined(GL_ARB_fragment_shader)
			temp.shaders_available = true;

			// geometry shaders available
#	ifdef GL_ARB_geometry_shader4
			temp.geometry_shaders_available = true;
#	endif

			// separate shader objects available
#	ifdef GL_EXT_separate_shader_objects
			temp.separate_shaders_available = true;
#	endif

#endif
			// shading language version
			glCheck(temp.shading_language_version = (cstring)glGetString(GL_SHADING_LANGUAGE_VERSION));

			// shader binary formats
			{
				int32_t num{};
				glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &num);
				temp.shader_binary_formats.resize((size_t)num);
				glGetIntegerv(GL_SHADER_BINARY_FORMATS, (int32_t *)temp.shader_binary_formats.data());
			}
		};
		return temp;
	}

	uint32_t opengl_device::get_error() const noexcept
	{
		return _error_type<to_user>(glGetError());
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
		alpha_fn temp{};

		glCheck(glGetIntegerv(GL_ALPHA_TEST_FUNC, (int32_t *)&temp.pred));
		temp.pred = _predicate<to_user>(temp.pred);

		glCheck(glGetFloatv(GL_ALPHA_TEST_REF, &temp.ref));

		return temp;
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
		value->bind();

		if (value->get_indices())
		{
			value->get_indices()->bind();

			for (auto const & vb : value->get_vertices())
			{
				vb->bind();

				draw_indexed(value->get_mode(), value->get_indices()->get_count());
			}
		}
		else
		{
			for (auto const & vb : value->get_vertices())
			{
				vb->bind();

				draw_arrays(value->get_mode(), 0, vb->get_count());
			}
		}
	}

	void opengl_device::draw_arrays(uint32_t mode, size_t first, size_t count)
	{
		glCheck(glDrawArrays(_primitive<to_impl>(mode), (uint32_t)first, (uint32_t)count));
	}

	void opengl_device::draw_indexed(uint32_t mode, size_t count)
	{
		glCheck(glDrawElements(_primitive<to_impl>(mode), (uint32_t)count, GL_UNSIGNED_INT, nullptr));
	}

	void opengl_device::flush()
	{
		glCheck(glFlush());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_device::upload(uniform_id loc, bool value)
	{
		glCheck(glUniform1iARB(ML_handle(int32_t, loc), (int32_t)value));
	}

	void opengl_device::upload(uniform_id loc, int32_t value)
	{
		glCheck(glUniform1iARB(ML_handle(int32_t, loc), value));
	}

	void opengl_device::upload(uniform_id loc, float_t value)
	{
		glCheck(glUniform1fARB(ML_handle(int32_t, loc), value));
	}

	void opengl_device::upload(uniform_id loc, vec2 const & value)
	{
		glCheck(glUniform2fARB(ML_handle(int32_t, loc), value[0], value[1]));
	}

	void opengl_device::upload(uniform_id loc, vec3 const & value)
	{
		glCheck(glUniform3fARB(ML_handle(int32_t, loc), value[0], value[1], value[2]));
	}

	void opengl_device::upload(uniform_id loc, vec4 const & value)
	{
		glCheck(glUniform4fARB(ML_handle(int32_t, loc), value[0], value[1], value[2], value[3]));
	}

	void opengl_device::upload(uniform_id loc, mat2 const & value)
	{
		glCheck(glUniformMatrix2fvARB(ML_handle(int32_t, loc), 2 * 2, false, value));
	}

	void opengl_device::upload(uniform_id loc, mat3 const & value)
	{
		glCheck(glUniformMatrix3fvARB(ML_handle(int32_t, loc), 3 * 3, false, value));
	}

	void opengl_device::upload(uniform_id loc, mat4 const & value)
	{
		glCheck(glUniformMatrix4fvARB(ML_handle(int32_t, loc), 4 * 4, false, value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_vertexbuffer::opengl_vertexbuffer(uint32_t usage, size_t count, address_t data)
		: m_usage{ usage }
		, m_buffer{ bufcpy<float_t>(count, data) }
	{
		glCheck(glGenBuffers(1, &m_handle));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
		glCheck(glBufferData(
			GL_ARRAY_BUFFER,
			(uint32_t)m_buffer.size(),
			(address_t)m_buffer.data(),
			_usage<to_impl>(m_usage)));
	}

	opengl_vertexbuffer::~opengl_vertexbuffer()
	{
		glCheck(glDeleteBuffers(1, &m_handle));
	}

	bool opengl_vertexbuffer::generate()
	{
		if (m_handle) { return false; }
		glCheck(glGenBuffers(1, &m_handle));
		return m_handle;
	}

	bool opengl_vertexbuffer::destroy()
	{
		if (!m_handle) { return false; }
		glCheck(glDeleteBuffers(1, &m_handle));
		return !(m_handle = NULL);
	}

	bool opengl_vertexbuffer::revalue()
	{
		if (m_handle) { glCheck(glDeleteBuffers(1, &m_handle));}
		glCheck(glGenBuffers(1, &m_handle));
		return m_handle;
	}

	void opengl_vertexbuffer::set_data(size_t count, address_t data, size_t offset)
	{
		m_buffer = bufcpy<float_t>(count, data);
		glCheck(glBufferSubData(
			GL_ARRAY_BUFFER,
			offset,
			(uint32_t)m_buffer.size(),
			(address_t)m_buffer.data()));
	}

	void opengl_vertexbuffer::do_bind(opengl_vertexbuffer const * value)
	{
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, value ? value->m_handle : NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_indexbuffer::opengl_indexbuffer(uint32_t usage, size_t count, address_t data)
		: m_usage{ usage }
		, m_buffer{ bufcpy<uint32_t>(count, data) }
	{
		glCheck(glGenBuffers(1, &m_handle));
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle));
		glCheck(glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			(uint32_t)m_buffer.size(),
			(address_t)m_buffer.data(),
			_usage<to_impl>(m_usage)));
	}

	opengl_indexbuffer::~opengl_indexbuffer()
	{
		glCheck(glDeleteBuffers(1, &m_handle));
	}

	bool opengl_indexbuffer::generate()
	{
		if (m_handle) { return false; }
		
		glCheck(glGenBuffers(1, &m_handle));
		
		return m_handle;
	}

	bool opengl_indexbuffer::destroy()
	{
		if (!m_handle) { return false; }
		
		glCheck(glDeleteBuffers(1, &m_handle));
		
		return !(m_handle = NULL);
	}

	bool opengl_indexbuffer::revalue()
	{
		if (m_handle) { glCheck(glDeleteBuffers(1, &m_handle)); }
		
		glCheck(glGenBuffers(1, &m_handle));
		
		return m_handle;
	}

	void opengl_indexbuffer::set_data(size_t count, address_t data, size_t offset)
	{
		m_buffer = bufcpy<uint32_t>(count, data);
		glCheck(glBufferSubData(
			GL_ELEMENT_ARRAY_BUFFER,
			offset,
			(uint32_t)m_buffer.size(),
			(address_t)m_buffer.data()));
	}

	void opengl_indexbuffer::do_bind(opengl_indexbuffer const * value)
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, value ? value->m_handle : NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_vertexarray::opengl_vertexarray(uint32_t mode)
		: m_mode{ mode }
	{
		glCheck(glGenVertexArrays(1, &m_handle));
		glCheck(glBindVertexArray(m_handle));
	}

	opengl_vertexarray::~opengl_vertexarray()
	{
		glCheck(glDeleteVertexArrays(1, &m_handle));
	}

	bool opengl_vertexarray::generate()
	{
		if (m_handle) { return false; }
		
		glCheck(glGenVertexArrays(1, &m_handle));
		
		return m_handle;
	}

	bool opengl_vertexarray::destroy()
	{
		if (!m_handle) { return false; }
		
		glCheck(glDeleteVertexArrays(1, &m_handle));
		
		m_vertices.clear(); m_indices.reset();
		
		return !(m_handle = NULL);
	}

	bool opengl_vertexarray::revalue()
	{
		if (m_handle) { glCheck(glDeleteVertexArrays(1, &m_handle)); }
		
		glCheck(glGenVertexArrays(1, &m_handle));

		m_vertices.clear(); m_indices.reset();

		return m_handle;
	}

	void opengl_vertexarray::add_vertices(shared<vertexbuffer> const & value)
	{
		if (!m_handle || !value || !*value) { return; }
		
		bind(); m_vertices.emplace_back(value)->bind();

		auto const & layout{ value->get_layout() };

		for (size_t i = 0; i < layout.elements().size(); ++i)
		{
			auto const & e{ layout.elements()[i] };

			if (uint32_t const type{ std::invoke([&]() noexcept -> uint32_t
			{
				switch (element_base_type(e.type))
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
					element_component_count(e.type),
					type,
					layout.stride(),
					reinterpret_cast<address_t>(e.offset)));
			}
			else
			{
				glCheck(glVertexAttribPointer(
					(uint32_t)i,
					element_component_count(e.type),
					type,
					e.normalized,
					layout.stride(),
					reinterpret_cast<address_t>(e.offset)));
			}
			glCheck(glEnableVertexAttribArray((uint32_t)i));
		}
	}

	void opengl_vertexarray::set_indices(shared<indexbuffer> const & value)
	{
		bind();

		if (m_indices = value) { m_indices->bind(); }
	}

	void opengl_vertexarray::do_bind(opengl_vertexarray const * value)
	{
		glCheck(glBindVertexArray(value ? value->m_handle : NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_texture2d::opengl_texture2d(texopts const & opts, address_t data)
		: m_opts{ opts }
	{
		glCheck(glGenTextures(1, &m_handle));
		glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
		glCheck(glTexImage2D(
			GL_TEXTURE_2D, 0,
			_format<to_impl>(m_opts.format.color),
			m_opts.size[0],
			m_opts.size[1],
			0,
			_format<to_impl>(m_opts.format.pixel),
			_type<to_impl>(m_opts.pixel_type),
			data));
		set_repeated(m_opts.flags & texture_flags_repeated);
		set_smooth(m_opts.flags & texture_flags_smooth);
		set_mipmapped(m_opts.flags & texture_flags_mipmapped);
	}

	opengl_texture2d::~opengl_texture2d()
	{
		glCheck(glDeleteTextures(1, &m_handle));
	}

	bool opengl_texture2d::generate()
	{
		if (m_handle) { return false; }
		
		glCheck(glGenTextures(1, &m_handle));
		
		return m_handle;
	}

	bool opengl_texture2d::destroy()
	{
		if (!m_handle) { return false; }
		
		glCheck(glDeleteTextures(1, &m_handle));
		
		return !(m_handle = NULL);
	}

	bool opengl_texture2d::revalue()
	{
		if (m_handle) { glCheck(glDeleteTextures(1, &m_handle)); }
		
		glCheck(glGenTextures(1, &m_handle));

		return m_handle;
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
		if (!m_lock) { debug::error("texture2d is not locked"); return; }

		if (m_handle && (m_opts.size == size)) { return; }
		else { m_opts.size = size; }

		revalue(); bind();
		
		glCheck(glTexImage2D(
			GL_TEXTURE_2D, 0,
			_format<to_impl>(m_opts.format.color),
			m_opts.size[0],
			m_opts.size[1],
			0,
			_format<to_impl>(m_opts.format.pixel),
			_type<to_impl>(m_opts.pixel_type),
			data));
		
		set_repeated(m_opts.flags & texture_flags_repeated);
		set_smooth(m_opts.flags & texture_flags_smooth);
		set_mipmapped(m_opts.flags & texture_flags_mipmapped);
	}

	void opengl_texture2d::update(vec2i const & pos, vec2i const & size, address_t data)
	{
		if (!m_lock) { debug::error("texture2d is not locked"); return; }

		if (m_handle && (m_opts.size == size)) { return; }
		else { m_opts.size = size; }

		revalue(); bind();

		glCheck(glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			pos[0],
			pos[1],
			m_opts.size[0],
			m_opts.size[1],
			_format<to_impl>(m_opts.format.color),
			_type<to_impl>(m_opts.pixel_type),
			data));
		
		set_repeated(m_opts.flags & texture_flags_repeated);
		set_smooth(m_opts.flags & texture_flags_smooth);
		set_mipmapped(m_opts.flags & texture_flags_mipmapped);
	}

	void opengl_texture2d::set_mipmapped(bool value)
	{
		if (!m_lock) { debug::error("texture2d is not locked"); return; }

		ML_flag_write(m_opts.flags, texture_flags_mipmapped, value);

		bool const smooth{ ML_flag_read(m_opts.flags, texture_flags_smooth) };

		if (smooth) { glCheck(glGenerateMipmap(GL_TEXTURE_2D)); }

		glCheck(glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			value
			? smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR
			: smooth ? GL_LINEAR : GL_NEAREST));
	}

	void opengl_texture2d::set_repeated(bool value)
	{
		if (!m_lock) { debug::error("texture2d is not locked"); return; }

		ML_flag_write(m_opts.flags, texture_flags_repeated, value);

		static bool const edge_clamp_available
		{
			device::get_context()->get_devinfo().texture_edge_clamp_available
		};

		glCheck(glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S,
			value
			? GL_REPEAT
			: edge_clamp_available ? GL_CLAMP_TO_EDGE : GL_CLAMP));

		glCheck(glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T,
			value
			? GL_REPEAT
			: edge_clamp_available ? GL_CLAMP_TO_EDGE : GL_CLAMP));
	}

	void opengl_texture2d::set_smooth(bool value)
	{
		if (!m_lock) { debug::error("texture2d is not locked"); return; }

		ML_flag_write(m_opts.flags, texture_flags_smooth, value);

		glCheck(glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			value ? GL_LINEAR : GL_NEAREST));

		glCheck(glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			ML_flag_read(m_opts.flags, texture_flags_mipmapped)
			? value ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR
			: value ? GL_LINEAR : GL_NEAREST));
	}

	image opengl_texture2d::copy_to_image() const
	{
		image temp{ m_opts.size, bits_per_pixel(m_opts.format.color) };
		if (!m_lock) { debug::error("texture2d is not locked"); return temp; }
		if (m_handle)
		{
			ML_bind_scope(*this);

			glCheck(glGetTexImage(GL_TEXTURE_2D, 0,
				_format<to_impl>(m_opts.format.color),
				_type<to_impl>(m_opts.pixel_type),
				temp.data()));
		}
		return temp;
	}

	void opengl_texture2d::do_bind(opengl_texture2d const * value, uint32_t slot)
	{
		glCheck(glBindTextureUnit(slot, value ? value->m_handle : NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texturecube
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_texturecube::opengl_texturecube(texopts const & opts) : m_opts{ opts }
	{
		glCheck(glGenTextures(1, &m_handle));
	}

	opengl_texturecube::~opengl_texturecube()
	{
		glCheck(glDeleteTextures(1, &m_handle));
	}

	bool opengl_texturecube::generate()
	{
		if (m_handle) { return false; }
		
		glCheck(glGenTextures(1, &m_handle));
		
		return m_handle;
	}

	bool opengl_texturecube::destroy()
	{
		if (!m_handle) { return false; }
		
		glCheck(glDeleteTextures(1, &m_handle));
		
		return !(m_handle = NULL);
	}

	bool opengl_texturecube::revalue()
	{
		if (m_handle) { glCheck(glDeleteTextures(1, &m_handle)); }
		
		glCheck(glGenTextures(1, &m_handle));
		
		return m_handle;
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

	void opengl_texturecube::do_bind(opengl_texturecube const * value, uint32_t slot)
	{
		glCheck(glBindTextureUnit(slot, value ? value->m_handle : NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_framebuffer::opengl_framebuffer(texopts const & opts) : m_opts{ opts }
	{
		resize(m_opts.size);
	}

	opengl_framebuffer::~opengl_framebuffer()
	{
		glCheck(glDeleteFramebuffers(1, &m_handle));
	}

	bool opengl_framebuffer::generate()
	{
		if (m_handle) { return false; }
		
		glCheck(glGenFramebuffers(1, &m_handle));
		
		return m_handle;
	}

	bool opengl_framebuffer::destroy()
	{
		if (!m_handle) { return false; }

		glCheck(glDeleteFramebuffers(1, &m_handle));

		m_color.reset(); m_depth.reset();

		return !(m_handle = NULL);
	}

	bool opengl_framebuffer::revalue()
	{
		if (m_handle) { glCheck(glDeleteFramebuffers(1, &m_handle)); }
		
		glCheck(glGenFramebuffers(1, &m_handle));
		
		m_color.reset(); m_depth.reset();

		return m_handle;
	}

	void opengl_framebuffer::resize(vec2i const & size)
	{
		if (m_handle && (m_opts.size == size)) { return; }
		else { m_opts.size = size; }

		revalue(); bind();

		// color attachment
		if (m_color) { m_color->update(m_opts.size); }
		else
		{
			m_color = texture2d::allocate(m_opts);
		}
		glCheck(glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			ML_handle(uint32_t, m_color->get_handle()),
			0));

		// depth attachment
		if (m_depth) { m_depth->update(m_opts.size); }
		else
		{
			m_depth = texture2d::allocate({
				m_opts.size,
				{ format_depth24_stencil8, format_depth_stencil },
				type_unsigned_int_24_8,
				m_opts.flags });
		}
		glCheck(glFramebufferTexture2D(
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

	void opengl_framebuffer::do_bind(opengl_framebuffer const * value)
	{
		if (value)
		{
			glCheck(glBindFramebuffer(GL_FRAMEBUFFER, value->m_handle));
			glCheck(glViewport(0, 0, value->m_opts.size[0], value->m_opts.size[1]));
		}
		else
		{
			glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_program::opengl_program(uint32_t type)
		: m_shader_type{ type }
	{
		glCheck(m_handle = glCreateShaderObjectARB(_shader_type<to_impl>(m_shader_type)));
	}

	opengl_program::~opengl_program()
	{
		glCheck(glDeleteObjectARB(m_handle));
	}

	bool opengl_program::generate()
	{
		if (m_handle) { return false; }
		
		glCheck(m_handle = glCreateShaderObjectARB(_shader_type<to_impl>(m_shader_type)));
		
		return m_handle;
	}

	bool opengl_program::destroy()
	{
		if (!m_handle) { return false; }
		
		glCheck(glDeleteObjectARB(m_handle));
		
		return !(m_handle = NULL);
	}

	bool opengl_program::revalue()
	{
		if (m_handle) { glCheck(glDeleteObjectARB(m_handle)); }
		
		glCheck(m_handle = glCreateShaderObjectARB(_shader_type<to_impl>(m_shader_type)));
		
		return m_handle;
	}

	bool opengl_program::compile(pgm_src_t const & src)
	{
		// check empty
		if ((m_source = src).empty()) { return false; }

		// compile
		cstring src_addr{ m_source.front().c_str() };
		glCheck(glShaderSourceARB(m_handle, (uint32_t)m_source.size(), &src_addr, nullptr));
		glCheck(glCompileShaderARB(m_handle));

		// check errors
		m_error_log.clear();
		int32_t success{};
		glCheck(glGetObjectParameterivARB(m_handle, GL_OBJECT_COMPILE_STATUS_ARB, &success));
		if (!success)
		{
			int32_t log_len{};
			glCheck(glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &log_len));
			m_error_log.resize((size_t)log_len);
			glCheck(glGetInfoLogARB(m_handle, log_len, &log_len, m_error_log.data()));
			m_error_log.push_back(0);
		}
		return success;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// shader
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_shader::opengl_uniform_binder::opengl_uniform_binder(opengl_shader & s, cstring name)
	{
		if (!name || !*name || !(self = s.m_handle)) { return; }
		
		glCheck(last = glGetHandleARB(GL_PROGRAM_OBJECT_ARB));

		if (self != last) { glCheck(glUseProgramObjectARB(self)); }

		location = s.m_uniforms.find_or_add_fn(util::hash(name, util::strlen(name)), [&
		]() noexcept
		{
			int32_t temp{};
			glCheck(temp = glGetUniformLocationARB(self, name));
			return ML_handle(uniform_id, temp);
		});
	}

	opengl_shader::opengl_uniform_binder::~opengl_uniform_binder()
	{
		if (self && (self != last))
		{
			glCheck(glUseProgramObjectARB(last));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_shader::opengl_shader()
	{
		glCheck(m_handle = glCreateProgramObjectARB());
	}

	opengl_shader::~opengl_shader()
	{
		glCheck(glDeleteObjectARB(m_handle));
	}

	bool opengl_shader::generate()
	{
		if (m_handle) { return false; }
		
		glCheck(m_handle = glCreateProgramObjectARB());
		
		return m_handle;
	}

	bool opengl_shader::destroy()
	{
		if (!m_handle) { return false; }
		
		glCheck(glDeleteObjectARB(m_handle));
		
		m_programs.clear(); m_uniforms.clear(); m_textures.clear();
		
		return !(m_handle = NULL);
	}

	bool opengl_shader::revalue()
	{
		if (m_handle) { glCheck(glDeleteObjectARB(m_handle)); }
		
		glCheck(m_handle = glCreateProgramObjectARB());
		
		m_uniforms.clear(); m_textures.clear(); m_programs.clear();
		
		return link();
	}

	bool opengl_shader::attach(shared<program> const & value)
	{
		if (!m_handle || !value || !*value) { return false; }
		else if (m_programs.try_emplace(value->get_shader_type(), value).second)
		{
			glCheck(glAttachObjectARB(m_handle, ML_handle(uint32_t, value->get_handle())));

			return true;
		}
		else { return false; }
	}

	bool opengl_shader::detach(shared<program> const & value)
	{
		if (!m_handle || !value || !*value) { return false; }
		else if (auto const it{ m_programs.find(value->get_shader_type()) })
		{
			glCheck(glDetachObjectARB(m_handle, ML_handle(uint32_t, value->get_handle())));

			m_programs.erase(it->first);

			return true;
		}
		else { return false; }
	}

	bool opengl_shader::link()
	{
		// check empty
		if (m_programs.empty()) { return false; }

		// link
		glCheck(glLinkProgramARB(m_handle));

		// check errors
		m_error_log.clear();
		int32_t success{};
		glCheck(glGetObjectParameterivARB(m_handle, GL_OBJECT_LINK_STATUS_ARB, &success));
		if (!success)
		{
			// error log
			int32_t log_len{};
			glCheck(glGetShaderiv(m_handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &log_len));
			m_error_log.resize((size_t)log_len);
			glCheck(glGetProgramInfoLog(m_handle, log_len, &log_len, m_error_log.data()));
			m_error_log.push_back(0);
		}
		return success;
	}

	void opengl_shader::do_bind(opengl_shader const * value)
	{
		glCheck(glUseProgramObjectARB(value ? value->m_handle : NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // ML_IMPL_RENDERER_OPENGL