#if defined(ML_IMPL_RENDERER_OPENGL)

#include "Impl_RenderAPI_OpenGL.hpp"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl loader
#ifdef ML_OPENGL_LOADER_CUSTOM
#	if defined(__has_include) && __has_include(ML_OPENGL_LOADER_CUSTOM)
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

// enums
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _buffer_bit(uint32_t value) noexcept
	{
		uint32_t temp{};
		if constexpr (Convt()) // to impl
		{
			ML_flag_map(temp, GL_COLOR_BUFFER_BIT	, value, buffer_bit_color);
			ML_flag_map(temp, GL_DEPTH_BUFFER_BIT	, value, buffer_bit_depth);
			ML_flag_map(temp, GL_STENCIL_BUFFER_BIT	, value, buffer_bit_stencil);
		}
		else // to user
		{
			ML_flag_map(temp, buffer_bit_color		, value, GL_COLOR_BUFFER_BIT);
			ML_flag_map(temp, buffer_bit_depth		, value, GL_DEPTH_BUFFER_BIT);
			ML_flag_map(temp, buffer_bit_stencil	, value, GL_STENCIL_BUFFER_BIT);
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// check error macro
#if ML_is_debug
#	define glCheck(expr) do{ (expr); _ML_GFX check_error(#expr, __FILE__, __LINE__); } while(0)
#else
#	define glCheck(expr) (expr)
#endif

namespace ml::gfx
{
	// check error
	static void check_error(cstring expr, cstring file, size_t line) noexcept
	{
		if (auto const code{ _error_type<to_user>(glGetError()) })
		{
			std::cout
				<< "An internal graphics error occurred ( " << code << " )\n"
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_device::opengl_device(context_settings const & cs)
	{
		// INIT
		static ML_scope{ ML_assert(ML_IMPL_OPENGL_INIT()); };
		{
			glCheck((cs.multisample ? &glEnable : &glDisable)(GL_MULTISAMPLE));
			
			glCheck((cs.srgb_capable ? &glEnable : glDisable)(GL_FRAMEBUFFER_SRGB));
			
			glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		}

		// VERSION
		{
			// renderer
			glCheck(m_devinfo.renderer = (cstring)glGetString(GL_RENDERER));

			// vendor
			glCheck(m_devinfo.vendor = (cstring)glGetString(GL_VENDOR));

			// version
			glCheck(m_devinfo.version = (cstring)glGetString(GL_VERSION));

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
				glCheck(glGetIntegerv(GL_NUM_EXTENSIONS, &num));
				m_devinfo.extensions.reserve(num);

				pmr::stringstream ss{};
				glCheck(ss.str((cstring)glGetString(GL_EXTENSIONS)));

				pmr::string line{};
				while (std::getline(ss, line, ' '))
				{
					m_devinfo.extensions.push_back(line);
				}
			}
		}

		// TEXTURES
		{
			// edge clamp available
#if defined(GL_EXT_texture_edge_clamp) \
|| defined(GLEW_EXT_texture_edge_clamp) \
|| defined(GL_SGIS_texture_edge_clamp)
			m_devinfo.texture_edge_clamp_available = true;
#endif
			// max texture slots
			glCheck(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (int32_t *)&m_devinfo.max_texture_slots));
		}

		// FRAMEBUFFERS
		{
			// max color attachments
			glCheck(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, (int32_t *)&m_devinfo.max_color_attachments));

			// max samples
			glCheck(glGetIntegerv(GL_MAX_SAMPLES, (int32_t *)&m_devinfo.max_samples));
		}
		
		// SHADERS
		{
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

			// separate shader objects available
#	ifdef GL_ARB_separate_shader_objects
			m_devinfo.separate_shaders_available = true;
#	endif

#endif
			// shading language version
			glCheck(m_devinfo.shading_language_version = (cstring)glGetString(GL_SHADING_LANGUAGE_VERSION));
		}

		// FUNCTIONS
		{
			// get error
			m_devinfo.get_error = []() noexcept { return _error_type<to_user>(glGetError()); };
		}
	}

	opengl_device::~opengl_device()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_device::get_alpha_enabled() const
	{
		bool temp{};
		glCheck(glGetBooleanv(GL_ALPHA_TEST, (uint8_t *)&temp));
		return temp;
	}

	alpha_mode opengl_device::get_alpha_mode() const
	{
		alpha_mode temp{};

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

	blend_mode opengl_device::get_blend_mode() const
	{
		blend_mode temp{};

		glCheck(glGetIntegerv(GL_BLEND_EQUATION_RGB, (int32_t *)&temp.color_equation));
		temp.color_equation = _function<to_user>(temp.color_equation);

		glCheck(glGetIntegerv(GL_BLEND_SRC_RGB, (int32_t *)&temp.color_sfactor));
		temp.color_sfactor = _factor<to_user>(temp.color_sfactor);

		glCheck(glGetIntegerv(GL_BLEND_DST_RGB, (int32_t *)&temp.color_dfactor));
		temp.color_dfactor = _factor<to_user>(temp.color_dfactor);

		glCheck(glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (int32_t *)&temp.alpha_equation));
		temp.alpha_equation = _function<to_user>(temp.alpha_equation);

		glCheck(glGetIntegerv(GL_BLEND_SRC_ALPHA, (int32_t *)&temp.alpha_sfactor));
		temp.alpha_sfactor = _factor<to_user>(temp.alpha_sfactor);

		glCheck(glGetIntegerv(GL_BLEND_DST_ALPHA, (int32_t *)&temp.alpha_dfactor));
		temp.alpha_dfactor = _factor<to_user>(temp.alpha_dfactor);

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

	cull_mode opengl_device::get_cull_mode() const
	{
		cull_mode temp{};

		glCheck(glGetIntegerv(GL_CULL_FACE_MODE, (int32_t *)&temp.facet));
		temp.facet = _facet<to_user>(temp.facet);

		glCheck(glGetIntegerv(GL_FRONT_FACE, (int32_t *)&temp.order));
		temp.order = _order<to_user>(temp.order);

		return temp;
	}

	bool opengl_device::get_depth_enabled() const
	{
		bool temp{};
		glCheck(glGetBooleanv(GL_DEPTH_TEST, (uint8_t *)&temp));
		return temp;
	}

	bool opengl_device::get_depth_write() const
	{
		bool temp{};
		glCheck(glGetBooleanv(GL_DEPTH_WRITEMASK, (uint8_t *)&temp));
		return temp;
	}

	depth_mode opengl_device::get_depth_mode() const
	{
		depth_mode temp{};
		
		glCheck(glGetIntegerv(GL_DEPTH_FUNC, (int32_t *)&temp.pred));
		temp.pred = _predicate<to_user>(temp.pred);
		
		glCheck(glGetFloatv(GL_DEPTH_RANGE, temp.range));
		
		return temp;
	}

	bool opengl_device::get_stencil_enabled() const
	{
		bool temp{};
		glCheck(glGetBooleanv(GL_STENCIL_TEST, (uint8_t *)&temp));
		return temp;
	}

	stencil_mode opengl_device::get_stencil_mode() const
	{
		stencil_mode temp{};

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

	void opengl_device::set_alpha_mode(alpha_mode const & value)
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

	void opengl_device::set_blend_mode(blend_mode const & value)
	{
		glCheck(glBlendFuncSeparate(
			_factor<to_impl>(value.color_sfactor),
			_factor<to_impl>(value.color_dfactor),
			_factor<to_impl>(value.alpha_sfactor),
			_factor<to_impl>(value.alpha_dfactor)));

		glCheck(glBlendEquationSeparate(
			_function<to_impl>(value.color_equation),
			_function<to_impl>(value.alpha_equation)));
	}

	void opengl_device::set_clear_color(color const & value)
	{
		glCheck(glClearColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_device::set_cull_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_CULL_FACE));
	}

	void opengl_device::set_cull_mode(cull_mode const & value)
	{
		glCheck(glCullFace(_facet<to_impl>(value.facet)));
		
		glCheck(glFrontFace(_order<to_impl>(value.order)));
	}

	void opengl_device::set_depth_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_DEPTH_TEST));
	}

	void opengl_device::set_depth_mode(depth_mode const & value)
	{
		glCheck(glDepthFunc(_predicate<to_impl>(value.pred)));

		glCheck(glDepthRangef(value.range[0], value.range[1]));
	}

	void opengl_device::set_depth_write(bool enabled)
	{
		glCheck(glDepthMask(enabled));
	}

	void opengl_device::set_stencil_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : glDisable)(GL_STENCIL_TEST));
	}

	void opengl_device::set_stencil_mode(stencil_mode const & value)
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
		vertexarray::bind(value);

		if (value->get_indices())
		{
			indexbuffer::bind(value->get_indices());

			for (auto const & vb : value->get_vertices())
			{
				vertexbuffer::bind(vb);

				draw_indexed(value->get_mode(), value->get_indices()->get_count());
			}
		}
		else
		{
			for (auto const & vb : value->get_vertices())
			{
				vertexbuffer::bind(vb);

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
		: m_usage{ usage }, m_buffer{ bufcpy<float_t>(count, data) }
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

	bool opengl_vertexbuffer::invalidate()
	{
		if (m_handle) { glCheck(glDeleteBuffers(1, &m_handle)); }

		glCheck(glGenBuffers(1, &m_handle));

		m_buffer.clear();

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
		: m_usage{ usage }, m_buffer{ bufcpy<uint32_t>(count, data) }
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

	bool opengl_indexbuffer::invalidate()
	{
		if (m_handle) { glCheck(glDeleteBuffers(1, &m_handle)); }
		
		glCheck(glGenBuffers(1, &m_handle));

		m_buffer.clear();
		
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

	bool opengl_vertexarray::invalidate()
	{
		if (m_handle) { glCheck(glDeleteVertexArrays(1, &m_handle)); }
		
		glCheck(glGenVertexArrays(1, &m_handle));

		m_vertices.clear(); m_indices.reset();

		return m_handle;
	}

	void opengl_vertexarray::add_vertices(shared<vertexbuffer> const & value)
	{
		if (!m_handle || !value || !*value) { return; }
		
		bind();
		
		m_vertices.emplace_back(value)->bind();

		auto const & layout{ value->get_layout() };

		for (size_t i = 0; i < layout.elements().size(); ++i)
		{
			auto const & e{ layout.elements()[i] };

			if (uint32_t const type{ std::invoke([&]() noexcept -> uint32_t
			{
				switch (get_element_base_type(e.type))
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
					get_element_component_count(e.type),
					type,
					layout.stride(),
					reinterpret_cast<address_t>(e.offset)));
			}
			else
			{
				glCheck(glVertexAttribPointer(
					(uint32_t)i,
					get_element_component_count(e.type),
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
			_type<to_impl>(m_opts.format.type),
			data));
		set_repeated(m_opts.flags & texture_flags_repeated);
		set_smooth(m_opts.flags & texture_flags_smooth);
		set_mipmapped(m_opts.flags & texture_flags_mipmapped);
	}

	opengl_texture2d::~opengl_texture2d()
	{
		glCheck(glDeleteTextures(1, &m_handle));
	}

	bool opengl_texture2d::invalidate()
	{
		if (!m_lock) { return debug::error("texture2d is not locked"); }

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
		if (!m_lock) { return (void)debug::error("texture2d is not locked"); }

		if (m_handle && (m_opts.size == size)) { return; }
		else { m_opts.size = size; }

		invalidate(); bind();
		
		glCheck(glTexImage2D(
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

		invalidate(); bind();

		glCheck(glTexSubImage2D(
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

		if (smooth) { glCheck(glGenerateMipmap(GL_TEXTURE_2D)); }

		glCheck(glTexParameteri(GL_TEXTURE_2D,
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
			device::get_current_context()->get_devinfo().texture_edge_clamp_available
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
		if (!m_lock) { return (void)debug::error("texture2d is not locked"); }

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
		image temp{ m_opts.size, get_bits_per_pixel(m_opts.format.color) };
		if (!m_lock) { debug::error("texture2d is not locked"); return temp; }
		if (m_handle)
		{
			ML_bind_scope(*this);

			glCheck(glGetTexImage(GL_TEXTURE_2D, 0,
				_format<to_impl>(m_opts.format.color),
				_type<to_impl>(m_opts.format.type),
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

	bool opengl_texturecube::invalidate()
	{
		if (!m_lock) { return debug::error("texturecube is not locked"); }

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

	bool opengl_framebuffer::invalidate()
	{
		if (m_handle) { glCheck(glDeleteFramebuffers(1, &m_handle)); }
		
		glCheck(glGenFramebuffers(1, &m_handle));
		
		m_attachments.clear(); m_depth.reset();

		return m_handle;
	}

	bool opengl_framebuffer::attach(shared<texture2d> const & value)
	{
		static auto const max_color_attachments
		{
			(size_t)device::get_current_context()->get_devinfo().max_color_attachments
		};
		if (value
			&& (m_attachments.size() < max_color_attachments)
			&& !std::binary_search(m_attachments.begin(), m_attachments.end(), value))
		{
			m_attachments.push_back(value);

			return true;
		}
		else
		{
			return false;
		}
	}

	bool opengl_framebuffer::detach(shared<texture2d> const & value)
	{
		if (auto const it{ std::find(m_attachments.begin(), m_attachments.end(), value) }
		; it != m_attachments.end())
		{
			m_attachments.erase(it);

			return true;
		}
		else
		{
			return false;
		}
	}

	void opengl_framebuffer::resize(vec2i const & size)
	{
		if (m_handle && (m_opts.size == size)) { return; }
		else { m_opts.size = size; }

		invalidate(); bind();

		// color attachments
		if (m_attachments.empty()) { m_attachments.push_back(texture2d::create(m_opts)); }
		for (size_t i = 0, imax = m_attachments.size(); i < imax; ++i)
		{
			m_attachments[i]->update(m_opts.size);

			glCheck(glFramebufferTexture2D(
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
			m_depth = texture2d::create({
				m_opts.size, {
				format_depth24_stencil8,
				format_depth_stencil,
				type_unsigned_int_24_8 },
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

// shader
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_shader::opengl_shader(uint32_t type, int32_t flags)
		: m_shader_type{ type }, m_flags{ flags }
	{
		glCheck(m_handle = glCreateShaderObjectARB(_shader_type<to_impl>(m_shader_type)));
	}

	opengl_shader::~opengl_shader()
	{
		glCheck(glDeleteObjectARB(m_handle));
	}

	bool opengl_shader::invalidate()
	{
		if (m_handle) { glCheck(glDeleteObjectARB(m_handle)); }
		
		glCheck(m_handle = glCreateShaderObjectARB(_shader_type<to_impl>(m_shader_type)));
		
		return m_handle;
	}

	bool opengl_shader::compile(shader_src_t const & src)
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_program::opengl_uniform_binder::opengl_uniform_binder(opengl_program & s, cstring name) noexcept
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

	opengl_program::opengl_uniform_binder::~opengl_uniform_binder() noexcept
	{
		if (self && (self != last))
		{
			glCheck(glUseProgramObjectARB(last));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_program::opengl_program()
	{
		glCheck(m_handle = glCreateProgramObjectARB());
	}

	opengl_program::~opengl_program()
	{
		glCheck(glDeleteObjectARB(m_handle));
	}

	bool opengl_program::invalidate()
	{
		if (m_handle) { glCheck(glDeleteObjectARB(m_handle)); }
		
		glCheck(m_handle = glCreateProgramObjectARB());
		
		m_uniforms.clear(); m_textures.clear(); m_shaders.clear();
		
		return m_handle;
	}

	bool opengl_program::attach(shared<shader> const & value)
	{
		if (m_shaders.try_emplace(value->get_shader_type(), value).second)
		{
			if (m_handle && value && *value)
			{
				glCheck(glAttachObjectARB(m_handle, ML_handle(uint32_t, value->get_handle())));
			}

			return true;
		}
		return false;
	}

	bool opengl_program::detach(shared<shader> const & value)
	{
		if (auto const it{ m_shaders.find(value->get_shader_type()) })
		{
			if (m_handle && value && *value)
			{
				glCheck(glDetachObjectARB(m_handle, ML_handle(uint32_t, value->get_handle())));
			}

			m_shaders.erase(it->first);

			return true;
		}
		return false;
	}

	bool opengl_program::link()
	{
		// check empty
		if (m_shaders.empty()) { return false; }

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

	void opengl_program::do_bind(opengl_program const * value)
	{
		glCheck(glUseProgramObjectARB(value ? value->m_handle : NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // ML_IMPL_RENDERER_OPENGL