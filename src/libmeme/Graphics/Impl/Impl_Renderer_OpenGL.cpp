#if defined(ML_IMPL_RENDERER_OPENGL)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Impl_Renderer_OpenGL.hpp"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_OPENGL_ES2)
#	include <GLES2/gl2.h>
#elif defined(ML_IMPL_OPENGL_ES3)
#	if defined(ML_os_apple && (TARGET_OS_IOS || TARGET_OS_TV))
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
#elif defined(ML_IMPL_OPENGL_LOADER_CUSTOM)
#	include ML_OPENGL_LOADER_CUSTOM
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if (!ML_is_debug)
# 	define glCheck(expr) (expr)
#else
# 	define glCheck(expr) \
	do { expr; check_error(__FILE__, __LINE__, ML_to_string(expr)); } while (0)
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	decltype(renderer_impl::g_enum_table) renderer_impl::g_enum_table
	{
		GL_VENDOR,
		GL_RENDERER,
		GL_VERSION,
		GL_MAJOR_VERSION,
		GL_MINOR_VERSION,
		GL_EXTENSIONS,
		GL_NUM_EXTENSIONS,

		GL_NO_ERROR,
		GL_INVALID_ENUM,
		GL_INVALID_VALUE,
		GL_INVALID_OPERATION,
		GL_STACK_OVERFLOW,
		GL_STACK_UNDERFLOW,
		GL_OUT_OF_MEMORY,
		GL_INVALID_FRAMEBUFFER_OPERATION,

		GL_STREAM_DRAW,
		GL_STATIC_DRAW,
		GL_DYNAMIC_DRAW,

		GL_CULL_FACE,
		GL_DEPTH_TEST,
		GL_ALPHA_TEST,
		GL_BLEND,
		GL_MULTISAMPLE,
		GL_FRAMEBUFFER_SRGB,
		GL_SCISSOR_TEST,

		GL_TEXTURE_2D,
		GL_TEXTURE_3D,
		GL_TEXTURE_CUBE_MAP,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,

		GL_ARRAY_BUFFER,
		GL_ELEMENT_ARRAY_BUFFER,
		GL_ARRAY_BUFFER_BINDING,
		GL_FRAMEBUFFER,
		GL_RENDERBUFFER,
		GL_PROGRAM_OBJECT_ARB,
		GL_READ_FRAMEBUFFER,
		GL_DRAW_FRAMEBUFFER,
		GL_READ_FRAMEBUFFER_BINDING,
		GL_DRAW_FRAMEBUFFER_BINDING,
		GL_CURRENT_PROGRAM,
		GL_VERTEX_ARRAY_BINDING,
		GL_TEXTURE_BINDING_2D,
		GL_SAMPLER_BINDING,
		GL_ACTIVE_TEXTURE,
		GL_POLYGON_MODE,
		GL_VIEWPORT,
		GL_SCISSOR_BOX,

		GL_BLEND_EQUATION_RGB,
		GL_BLEND_EQUATION_ALPHA,
		GL_BLEND_DST_RGB,
		GL_BLEND_SRC_RGB,
		GL_BLEND_DST_ALPHA,
		GL_BLEND_SRC_ALPHA,

		GL_LOWER_LEFT,
		GL_UPPER_LEFT,
		GL_CLIP_ORIGIN,

		GL_OBJECT_DELETE_STATUS_ARB,
		GL_OBJECT_COMPILE_STATUS_ARB,
		GL_OBJECT_LINK_STATUS_ARB,
		GL_INFO_LOG_LENGTH,
		GL_FRAMEBUFFER_COMPLETE,

		GL_FRAGMENT_SHADER,
		GL_VERTEX_SHADER,
		GL_GEOMETRY_SHADER,
		GL_SHADING_LANGUAGE_VERSION,

		GL_POINTS,
		GL_LINES,
		GL_LINE_LOOP,
		GL_LINE_STRIP,
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
		GL_TRIANGLE_FAN,
		GL_FILL,

		GL_MULT,
		GL_ADD,
		GL_SUBTRACT,
		GL_FUNC_ADD,

		GL_NEVER,
		GL_LESS,
		GL_EQUAL,
		GL_LEQUAL,
		GL_GREATER,
		GL_NOTEQUAL,
		GL_GEQUAL,
		GL_ALWAYS,

		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA_SATURATE,

		GL_FRONT_LEFT,
		GL_FRONT_RIGHT,
		GL_BACK_LEFT,
		GL_BACK_RIGHT,
		GL_FRONT,
		GL_BACK,
		GL_LEFT,
		GL_RIGHT,
		GL_FRONT_AND_BACK,

		GL_BYTE,
		GL_UNSIGNED_BYTE,
		GL_SHORT,
		GL_UNSIGNED_SHORT,
		GL_INT,
		GL_UNSIGNED_INT,
		GL_FLOAT,
		GL_HALF_FLOAT,

		GL_RED,
		GL_GREEN,
		GL_BLUE,
		GL_ALPHA,
		GL_RGB,
		GL_RGBA,
		GL_LUMINANCE,
		GL_LUMINANCE_ALPHA,
		GL_SRGB,
		GL_SRGB8,
		GL_SRGB_ALPHA,
		GL_SRGB8_ALPHA8,
		GL_SLUMINANCE_ALPHA,
		GL_SLUMINANCE8_ALPHA8,
		GL_SLUMINANCE,
		GL_SLUMINANCE8,
		GL_DEPTH24_STENCIL8,

		GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING,
		GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE,
		GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
		GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
		GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
		GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
		GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
		GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
		GL_FRAMEBUFFER_DEFAULT,
		GL_FRAMEBUFFER_UNDEFINED,
		GL_DEPTH_STENCIL_ATTACHMENT,

		GL_NEAREST,
		GL_LINEAR,
		GL_NEAREST_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_TEXTURE_MAG_FILTER,
		GL_TEXTURE_MIN_FILTER,
		GL_TEXTURE_WRAP_S,
		GL_TEXTURE_WRAP_T,
		GL_TEXTURE_WRAP_R,
		GL_CLAMP,
		GL_REPEAT,
		GL_CLAMP_TO_EDGE,
		GL_TEXTURE_MIN_LOD,
		GL_TEXTURE_MAX_LOD,
		GL_TEXTURE_BASE_LEVEL,
		GL_TEXTURE_MAX_LEVEL,

		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,

		GL_UNPACK_SWAP_BYTES,
		GL_UNPACK_LSB_FIRST,
		GL_UNPACK_ROW_LENGTH,
		GL_UNPACK_SKIP_ROWS,
		GL_UNPACK_SKIP_PIXELS,
		GL_UNPACK_ALIGNMENT,

		GL_PACK_SWAP_BYTES,
		GL_PACK_LSB_FIRST,
		GL_PACK_ROW_LENGTH,
		GL_PACK_SKIP_ROWS,
		GL_PACK_SKIP_PIXELS,
		GL_PACK_ALIGNMENT,

		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6,
		GL_COLOR_ATTACHMENT7,
		GL_COLOR_ATTACHMENT8,
		GL_COLOR_ATTACHMENT9,
		GL_COLOR_ATTACHMENT10,
		GL_COLOR_ATTACHMENT11,
		GL_COLOR_ATTACHMENT12,
		GL_COLOR_ATTACHMENT13,
		GL_COLOR_ATTACHMENT14,
		GL_COLOR_ATTACHMENT15,

		GL_TEXTURE0,
		GL_TEXTURE1,
		GL_TEXTURE2,
		GL_TEXTURE3,
		GL_TEXTURE4,
		GL_TEXTURE5,
		GL_TEXTURE6,
		GL_TEXTURE7,
		GL_TEXTURE8,
		GL_TEXTURE9,
		GL_TEXTURE10,
		GL_TEXTURE11,
		GL_TEXTURE12,
		GL_TEXTURE13,
		GL_TEXTURE14,
		GL_TEXTURE15,
		GL_TEXTURE16,
		GL_TEXTURE17,
		GL_TEXTURE18,
		GL_TEXTURE19,
		GL_TEXTURE20,
		GL_TEXTURE21,
		GL_TEXTURE22,
		GL_TEXTURE23,
		GL_TEXTURE24,
		GL_TEXTURE25,
		GL_TEXTURE26,
		GL_TEXTURE27,
		GL_TEXTURE28,
		GL_TEXTURE29,
		GL_TEXTURE30,
		GL_TEXTURE31,

		GL_DRAW_BUFFER0,
		GL_DRAW_BUFFER1,
		GL_DRAW_BUFFER2,
		GL_DRAW_BUFFER3,
		GL_DRAW_BUFFER4,
		GL_DRAW_BUFFER5,
		GL_DRAW_BUFFER6,
		GL_DRAW_BUFFER7,
		GL_DRAW_BUFFER8,
		GL_DRAW_BUFFER9,
		GL_DRAW_BUFFER10,
		GL_DRAW_BUFFER11,
		GL_DRAW_BUFFER12,
		GL_DRAW_BUFFER13,
		GL_DRAW_BUFFER14,
		GL_DRAW_BUFFER15,

		GL_CONTEXT_FLAGS,
		GL_CONTEXT_PROFILE_MASK,
		GL_CONTEXT_CORE_PROFILE_BIT,
		GL_CONTEXT_COMPATIBILITY_PROFILE_BIT,
		GL_CONTEXT_FLAG_DEBUG_BIT,

		GL_DEPTH_BUFFER_BIT,
		GL_STENCIL_BUFFER_BIT,
		GL_COLOR_BUFFER_BIT,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool impl_renderer_opengl::initialize()
	{
#if defined(ML_IMPL_OPENGL_LOADER_GLEW)
		glewExperimental = true;
		return (GLEW_OK == glewInit());

#elif defined(ML_OPENGL_LOADER_GL3W)
		return gl3wInit();

#elif defined(ML_OPENGL_LOADER_GLAD)
		return gladLoadGL();

#elif defined(ML_OPENGL_LOADER_CUSTOM)
		return false;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t impl_renderer_opengl::get_error()
	{
		return glGetError();
	}

	void impl_renderer_opengl::check_error(cstring file, uint32_t line, cstring expr)
	{
		if (auto const code{ get_error() }; code != GL_NO_ERROR)
		{
			pmr::string path{ file };
			path = path.substr(path.find_last_of("\\/") + 1);
			pmr::string err_name{ "No Error" };
			pmr::string err_desc{ "No Description" };
			switch (code)
			{
			case gl_invalid_enum:
				err_name = "Invalid Enum";
				err_desc = "An unacceptable value has been specified for an enumerated argument";
				break;
			case gl_invalid_value:
				err_name = "Invalid Value";
				err_desc = "A numeric argument is out of range";
				break;
			case gl_invalid_operation:
				err_name = "Invalid Operation";
				err_desc = "The specified operation is not allowed in the current state";
				break;
			case gl_stack_overflow:
				err_name = "Stack Overflow";
				err_desc = "This command would cause a stack overflow";
				break;
			case gl_stack_underflow:
				err_name = "Stack Underflow";
				err_desc = "This command would cause a stack underflow";
				break;
			case gl_out_of_memory:
				err_name = "Out of Memory";
				err_desc = "There is not enough memory left to execute the command";
				break;
			case gl_invalid_framebuffer_operation:
				err_name = "Invalid Framebuffer Operation";
				err_desc = "The object bound to framebuffer binding is not \'framebuffer complete\'";
				break;
			}
			std::cout
				<< "OpenGL Error " << code << ":\n"
				<< " \'" << file << "\' (" << line << ")\n"
				<< "expr:\n"
				<< "\t" << expr << "\n"
				<< "desc:\n"
				<< "\t" << err_name << "\n"
				<< "\t" << err_desc << "\n"
				<< "\n\n";
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool impl_renderer_opengl::is_enabled(uint32_t value)
	{
		bool temp{};
		glCheck(glIsEnabled(get_enum(value)));
		return temp;
	}

	void impl_renderer_opengl::enable(uint32_t value)
	{
		glCheck(glEnable(get_enum(value)));
	}

	void impl_renderer_opengl::disable(uint32_t value)
	{
		glCheck(glDisable(get_enum(value)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool impl_renderer_opengl::get_bool(uint32_t value)
	{
		bool temp{};
		glCheck(glGetBooleanv(get_enum(value), reinterpret_cast<uint8_t *>(&temp)));
		return temp;
	}

	int32_t impl_renderer_opengl::get_int(uint32_t value)
	{
		int32_t temp{};
		glCheck(glGetIntegerv(get_enum(value), &temp));
		return temp;
	}

	float_t impl_renderer_opengl::get_float(uint32_t value)
	{
		float_t temp{};
		glCheck(glGetFloatv(get_enum(value), &temp));
		return temp;
	}

	float64_t impl_renderer_opengl::get_double(uint32_t value)
	{
		float64_t temp{};
		glCheck(glGetDoublev(get_enum(value), &temp));
		return temp;
	}

	cstring impl_renderer_opengl::get_string(uint32_t value)
	{
		cstring temp{};
		glCheck(temp = reinterpret_cast<cstring>(glGetString(get_enum(value))));
		return temp;
	}

	cstring impl_renderer_opengl::get_string(uint32_t value, uint32_t index)
	{
		cstring temp{};
		glCheck(temp = reinterpret_cast<cstring>(glGetStringi(get_enum(value), index)));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_IMPL_RENDERER_OPENGL