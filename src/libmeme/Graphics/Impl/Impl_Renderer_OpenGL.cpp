#if defined(ML_IMPL_RENDERER_OPENGL)

#include "Impl_Renderer_OpenGL.hpp"

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

// check error macro
#if (!ML_is_debug)
#	define glCheck(expr) (expr)
#else
#	define glCheck(expr) \
	do { expr; _ML check_error(__FILE__, __LINE__, #expr); } while (0)
#endif

// errors
namespace ml
{
	static void check_error(cstring file, uint32_t line, cstring expr)
	{
		if (auto const code{ glGetError() }; code != GL_NO_ERROR)
		{
			pmr::string path{ file };
			path = path.substr(path.find_last_of("\\/") + 1);
			pmr::string err_name{ "No Error" };
			pmr::string err_desc{ "No Description" };
			switch (code)
			{
			case GL_INVALID_ENUM:
				err_name = "Invalid Enum";
				err_desc = "An unacceptable value has been specified for an enumerated argument";
				break;
			case GL_INVALID_VALUE:
				err_name = "Invalid Value";
				err_desc = "A numeric argument is out of range";
				break;
			case GL_INVALID_OPERATION:
				err_name = "Invalid Operation";
				err_desc = "The specified operation is not allowed in the current state";
				break;
			case GL_STACK_OVERFLOW:
				err_name = "Stack Overflow";
				err_desc = "This command would cause a stack overflow";
				break;
			case GL_STACK_UNDERFLOW:
				err_name = "Stack Underflow";
				err_desc = "This command would cause a stack underflow";
				break;
			case GL_OUT_OF_MEMORY:
				err_name = "Out of Memory";
				err_desc = "There is not enough memory left to execute the command";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				err_name = "Invalid Framebuffer Operation";
				err_desc = "The object bound to frame buffer binding is not \'frame buffer complete\'";
				break;
			case GL_CONTEXT_LOST:
				err_name = "Context Lost";
				err_desc = "The OpenGL context has been lost, due to a graphics card reset";
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl enums
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using namespace gl;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _buffer_bit(uint32_t value) noexcept
	{
		uint32_t temp{};
		if constexpr (Convt()) // to backend
		{
			ML_flag_map(temp, GL_COLOR_BUFFER_BIT	, value, color_buffer);
			ML_flag_map(temp, GL_DEPTH_BUFFER_BIT	, value, depth_buffer);
			ML_flag_map(temp, GL_STENCIL_BUFFER_BIT	, value, stencil_buffer);
		}
		else // to user
		{
			ML_flag_map(temp, color_buffer			, value, GL_COLOR_BUFFER_BIT);
			ML_flag_map(temp, depth_buffer			, value, GL_DEPTH_BUFFER_BIT);
			ML_flag_map(temp, stencil_buffer		, value, GL_STENCIL_BUFFER_BIT);
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _error(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to backend
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
		if constexpr (Convt()) // to backend
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

	template <class Convt> constexpr uint32_t _primitive(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to backend
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
		if constexpr (Convt()) // to backend
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
		if constexpr (Convt()) // to backend
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
		if constexpr (Convt()) // to backend
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

	template <class Convt> constexpr uint32_t _front_face(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to backend
		{
			switch (value)
			{
			default				: return value;
			case front_face_cw	: return GL_CW;
			case front_face_ccw	: return GL_CCW;
			}
		}
		else // to user
		{
			switch (value)
			{
			default		: return value;
			case GL_CW	: return front_face_cw;
			case GL_CCW	: return front_face_ccw;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _facet(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to backend
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
		if constexpr (Convt()) // to backend
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
		if constexpr (Convt()) // to backend
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
		using namespace gl;
		if constexpr (Convt()) // to backend
		{
			switch (value)
			{
			default						: return value;
			case texture_type_1d		: return GL_TEXTURE_1D;
			case texture_type_2d		: return GL_TEXTURE_2D;
			case texture_type_3d		: return GL_TEXTURE_3D;
			case texture_type_cube_map	: return GL_TEXTURE_CUBE_MAP;
			}
		}
		else // to user
		{
			switch (value)
			{
			default						: return value;
			case GL_TEXTURE_1D			: return texture_type_1d;
			case GL_TEXTURE_2D			: return texture_type_2d;
			case GL_TEXTURE_3D			: return texture_type_3d;
			case GL_TEXTURE_CUBE_MAP	: return texture_type_cube_map;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _shader_type(uint32_t value) noexcept
	{
		if constexpr (Convt()) // to backend
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

// opengl objects
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_vertexarray::opengl_vertexarray()
	{
		glCheck(glGenVertexArrays(1, &m_handle));
		this->bind();
	}

	opengl_vertexarray::~opengl_vertexarray()
	{
		glCheck(glDeleteVertexArrays(1, &m_handle));
	}

	void opengl_vertexarray::bind() const
	{
		glCheck(glBindVertexArray(m_handle));
	}

	void opengl_vertexarray::unbind() const
	{
		glCheck(glBindVertexArray(NULL));
	}

	void opengl_vertexarray::add_vbo(shared<vertexbuffer> const & value)
	{
		if (!value) { return; }

		this->bind(); value->bind();

		uint32_t const stride{ value->get_layout().get_stride() };

		value->get_layout().for_each([&](uint32_t i, buffer_element const & e) noexcept
		{
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
				glCheck(glVertexAttribIPointer(i,
					e.get_component_count(),
					type,
					stride,
					reinterpret_cast<gl::buffer>(e.offset)));
			}
			else
			{
				glCheck(glVertexAttribPointer(i,
					e.get_component_count(),
					type,
					e.normalized,
					stride,
					reinterpret_cast<gl::buffer>(e.offset)));
			}
			glCheck(glEnableVertexAttribArray(i));
		});

		m_vertices.push_back(value);
	}

	void opengl_vertexarray::set_ibo(shared<indexbuffer> const & value)
	{
		this->bind();

		if (m_indices = value) { m_indices->bind(); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_vertexbuffer::opengl_vertexbuffer(gl::buffer vertices, uint32_t size, uint32_t usage)
		: m_usage{ usage }, m_size{ size }
	{
		glCheck(glGenBuffers(1, &m_handle));
		this->bind();
		glCheck(glBufferData(GL_ARRAY_BUFFER, size * sizeof(float_t), vertices, _usage<gl::to_impl>(usage)));
	}

	opengl_vertexbuffer::opengl_vertexbuffer(uint32_t size, uint32_t usage)
		: opengl_vertexbuffer{ nullptr, size, usage }
	{
	}

	opengl_vertexbuffer::~opengl_vertexbuffer()
	{
		glCheck(glDeleteBuffers(1, &m_handle));
	}

	void opengl_vertexbuffer::bind() const
	{
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
	}

	void opengl_vertexbuffer::unbind() const
	{
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, NULL));
	}

	void opengl_vertexbuffer::set_data(gl::buffer vertices, uint32_t size, uint32_t offset)
	{
		m_size = size;
		glCheck(glBufferSubData(GL_ARRAY_BUFFER, offset, size * sizeof(float_t), vertices));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_indexbuffer::opengl_indexbuffer(gl::buffer indices, uint32_t count)
		: m_count{ count }
	{
		glCheck(glGenBuffers(1, &m_handle));
		this->bind();
		this->set_data(indices, count);
	}

	opengl_indexbuffer::~opengl_indexbuffer()
	{
		glCheck(glDeleteBuffers(1, &m_handle));
	}

	void opengl_indexbuffer::bind() const
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle));
	}

	void opengl_indexbuffer::unbind() const
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));
	}

	void opengl_indexbuffer::set_data(gl::buffer indices, uint32_t count)
	{
		m_count = count;
		glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_framebuffer::opengl_framebuffer::opengl_framebuffer(uint32_t format, vec2i const & size)
		: m_format{ format }, m_size{}
	{
		this->resize(size);
	}

	opengl_framebuffer::~opengl_framebuffer()
	{
		glCheck(glDeleteFramebuffers(1, &m_handle));
	}

	void opengl_framebuffer::bind() const
	{
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_handle));
		glCheck(glViewport(0, 0, m_size[0], m_size[1]));
	}

	void opengl_framebuffer::unbind() const
	{
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
	}

	void opengl_framebuffer::bind_texture(uint32_t slot) const
	{
		glCheck(glActiveTexture(GL_TEXTURE0 + slot));
		m_color->bind();
	}

	void opengl_framebuffer::resize(vec2i const & size)
	{
		if (m_size == size) { return; }
		else { m_size = size; }

		if (m_handle) { this->~opengl_framebuffer(); }
		
		glCheck(glGenFramebuffers(1, &m_handle));
		
		this->bind();

		// color attachment
		if (m_color) { m_color->update(m_size); }
		else
		{
			m_color = texture2d::create(
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
			m_depth = texture2d::create(
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
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_texture2d::opengl_texture2d(vec2i const & size, uint32_t iformat, uint32_t cformat, uint32_t ptype, int32_t flags, gl::buffer pixels)
		: m_i_format{ iformat }, m_c_format{ cformat }, m_p_type{ ptype }, m_flags{ flags }
	{
		this->update(size, pixels);
	}

	opengl_texture2d::~opengl_texture2d()
	{
		glCheck(glDeleteTextures(1, &m_handle));
	}

	void opengl_texture2d::bind() const
	{
		glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
	}

	void opengl_texture2d::unbind() const
	{
		glCheck(glBindTexture(GL_TEXTURE_2D, NULL));
	}

	void opengl_texture2d::update(vec2i const & size, gl::buffer pixels)
	{
		if (m_size == size) { return; }
		else { m_size = size; }

		if (m_handle) { this->~opengl_texture2d(); }

		glCheck(glGenTextures(1, &m_handle));

		this->bind();

		glCheck(glTexImage2D(
			GL_TEXTURE_2D, 0,
			_format<gl::to_impl>(m_i_format), m_size[0], m_size[1], 0,
			_format<gl::to_impl>(m_c_format),
			_type<gl::to_impl>(m_p_type),
			pixels));

		if (m_flags != texture_flags_none)
		{
			this->set_repeated(this->is_repeated());
			this->set_smooth(this->is_smooth());
			this->set_mipmapped(this->is_mipmapped());
		}
	}

	void opengl_texture2d::set_mipmapped(bool value)
	{
		if (!m_handle) { return; }

		ML_flag_write(m_flags, texture_flags_mipmapped, value);

		if (this->is_smooth())
		{
			glCheck(glGenerateMipmap(GL_TEXTURE_2D));
		}

		glCheck(glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			value
			? this->is_smooth() ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR
			: this->is_smooth() ? GL_LINEAR : GL_NEAREST));
	}

	void opengl_texture2d::set_repeated(bool value)
	{
		if (!m_handle) { return; }

		ML_flag_write(m_flags, texture_flags_repeated, value);

		constexpr bool edgeClampAvailable{
#if defined(GL_EXT_texture_edge_clamp) \
|| defined(GLEW_EXT_texture_edge_clamp) \
|| defined(GL_SGIS_texture_edge_clamp)
			true
#endif
		};

		glCheck(glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S,
			value
			? GL_REPEAT
			: edgeClampAvailable ? GL_CLAMP_TO_EDGE : GL_CLAMP));

		glCheck(glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T,
			value
			? GL_REPEAT
			: edgeClampAvailable ? GL_CLAMP_TO_EDGE : GL_CLAMP));
	}

	void opengl_texture2d::set_smooth(bool value)
	{
		if (!m_handle) { return; }

		ML_flag_write(m_flags, texture_flags_smooth, value);

		glCheck(glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			value ? GL_LINEAR : GL_NEAREST));

		glCheck(glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			this->is_mipmapped()
			? value ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR
			: value ? GL_LINEAR : GL_NEAREST));
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
				_format<gl::to_impl>(m_i_format),
				_type<gl::to_impl>(m_p_type),
				temp.data()));
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct opengl_shader_object::uniform_binder final
	{
		int32_t location{ -1 }; uint32_t current{}, previous{};

		operator bool() const noexcept { return (-1 < location); }

		template <class Fn
		> uniform_binder(opengl_shader_object & self, cstring name, Fn && fn) noexcept
		{
			if (current = self.m_handle)
			{
				glCheck(previous = glGetHandleARB(GL_PROGRAM_OBJECT_ARB));

				if (current != previous)
				{
					glCheck(glUseProgramObjectARB(current));
				}

				location = self.m_uniforms.find_or_add_fn(util::hash(name, std::strlen(name)), [&
				]() noexcept
				{
					int32_t temp{};
					glCheck(temp = glGetUniformLocationARB(current, name));
					return temp;
				});

				if (*this)
				{
					std::invoke(ML_forward(fn), location);
				}
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

	opengl_shader_object::opengl_shader_object(cstring v_src, cstring f_src, cstring g_src)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto _print_errors = [&](uint32_t obj)
		{
			char log[1024]{};
			glCheck(glGetInfoLogARB(obj, sizeof(log), 0, log));
			pmr::stringstream ss{ log };
			pmr::string line;
			while (std::getline(ss, line))
			{
				std::cout << line << '\n';
			}
		};

		auto _compile = [&_print_errors](uint32_t type, cstring src) -> uint32_t
		{
			uint32_t obj{};
			if (src && *src)
			{
				glCheck(obj = glCreateShaderObjectARB(type));
				glCheck(glShaderSource(obj, 1, &src, nullptr));
				glCheck(glCompileShaderARB(obj));

				int32_t err{};
				glCheck(glGetObjectParameterivARB(obj, GL_OBJECT_COMPILE_STATUS_ARB, &err));
				if (!err)
				{
					_print_errors(obj);
					glCheck(glDeleteObjectARB(obj));
					obj = NULL;
				}
			}
			return obj;
		};

		auto _link = [&_print_errors](uint32_t obj) -> bool
		{
			glCheck(glLinkProgramARB(obj));
			int32_t err{};
			glCheck(glGetObjectParameterivARB(obj, GL_OBJECT_LINK_STATUS_ARB, &err));
			return err;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		if (m_handle)
		{
			glCheck(glDeleteObjectARB(m_handle));
			m_uniforms.clear();
			m_textures.clear();
		}

		glCheck(m_handle = glCreateProgramObjectARB());

		if (uint32_t v{ _compile(GL_VERTEX_SHADER, v_src) })
		{
			glCheck(glAttachObjectARB(m_handle, v));
			glCheck(glDeleteObjectARB(v));
		}

		if (uint32_t g{ _compile(GL_GEOMETRY_SHADER, g_src) })
		{
			glCheck(glAttachObjectARB(m_handle, g));
			glCheck(glDeleteObjectARB(g));
		}

		if (uint32_t f{ _compile(GL_FRAGMENT_SHADER, f_src) })
		{
			glCheck(glAttachObjectARB(m_handle, f));
			glCheck(glDeleteObjectARB(f));
		}

		if (!_link(m_handle))
		{
			glCheck(glDeleteObjectARB(m_handle));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	opengl_shader_object::~opengl_shader_object()
	{
		glCheck(glDeleteObjectARB(m_handle));
	}

	void opengl_shader_object::bind(bool bind_textures) const
	{
		glCheck(glUseProgramObjectARB(m_handle));

		if (bind_textures)
		{
			uint32_t index{};
			m_textures.for_each([&](int32_t location, gl::handle tex)
			{
				glCheck(glUniform1i(location, index));
				glCheck(glActiveTexture(GL_TEXTURE0 + index));
				glCheck(glBindTexture(GL_TEXTURE_2D, (uint32_t)(intptr_t)tex));
				index++;
			});
		}
	}

	void opengl_shader_object::unbind() const
	{
		glCheck(glUseProgramObjectARB(NULL));
	}

	bool opengl_shader_object::set_uniform(cstring name, bool value)
	{
		return this->set_uniform(name, static_cast<int32_t>(value));
	}

	bool opengl_shader_object::set_uniform(cstring name, int32_t value)
	{
		return uniform_binder(*this, name, [&](int32_t location)
		{
			glCheck(glUniform1iARB(location, value));
		});
	}

	bool opengl_shader_object::set_uniform(cstring name, float_t value)
	{
		return uniform_binder(*this, name, [&](int32_t location)
		{
			glCheck(glUniform1fARB(location, value));
		});
	}

	bool opengl_shader_object::set_uniform(cstring name, vec2 const & value)
	{
		return uniform_binder(*this, name, [&](int32_t location)
		{
			glCheck(glUniform2fARB(location, value[0], value[1]));
		});
	}

	bool opengl_shader_object::set_uniform(cstring name, vec3 const & value)
	{
		return uniform_binder(*this, name, [&](int32_t location)
		{
			glCheck(glUniform3fARB(location, value[0], value[1], value[2]));
		});
	}

	bool opengl_shader_object::set_uniform(cstring name, vec4 const & value)
	{
		return uniform_binder(*this, name, [&](int32_t location)
		{
			glCheck(glUniform4fARB(location, value[0], value[1], value[2], value[3]));
		});
	}

	bool opengl_shader_object::set_uniform(cstring name, mat2 const & value)
	{
		return uniform_binder(*this, name, [&](int32_t location)
		{
			glCheck(glUniformMatrix2fvARB(location, (uint32_t)value.size(), false, value));
		});
	}

	bool opengl_shader_object::set_uniform(cstring name, mat3 const & value)
	{
		return uniform_binder(*this, name, [&](int32_t location)
		{
			glCheck(glUniformMatrix3fvARB(location, (uint32_t)value.size(), false, value));
		});
	}

	bool opengl_shader_object::set_uniform(cstring name, mat4 const & value)
	{
		return uniform_binder(*this, name, [&](int32_t location)
		{
			glCheck(glUniformMatrix4fvARB(location, (uint32_t)value.size(), false, value));
		});
	}

	bool opengl_shader_object::set_uniform(cstring name, shared<texture2d> const & value)
	{
		static auto const max_texture_units{ std::invoke([]() noexcept
		{
			uint32_t temp{};
			glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (int32_t *)&temp);
			return temp;
		}) };
		return uniform_binder(*this, name, [&](int32_t location)
		{
			if (auto const it{ m_textures.find(location) })
			{
				(*it->second) = value ? value->get_handle() : nullptr;
			}
			else if ((m_textures.size() + 1) < (size_t)max_texture_units)
			{
				m_textures.insert(location, value ? value->get_handle() : nullptr);
			}
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl api
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_render_api::do_initialize()
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

	void opengl_render_api::on_initialize()
	{
		glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_api_info const & opengl_render_api::get_info() const
	{
		static render_api_info temp{};
		static ML_scope // once
		{
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

			// renderer
			glCheck(temp.renderer = (cstring)glGetString(GL_RENDERER));
			
			// vendor
			glCheck(temp.vendor = (cstring)glGetString(GL_VENDOR));
			
			// version
			glCheck(temp.version = (cstring)glGetString(GL_VERSION));

			// shading language version
			glCheck(temp.shading_language_version = (cstring)glGetString(GL_SHADING_LANGUAGE_VERSION));

			// extensions
			{
				int32_t num_extensions{};
				glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
				temp.extensions.reserve(num_extensions);

				pmr::stringstream ss{ (cstring)glGetString(GL_EXTENSIONS) };
				pmr::string line{};
				while (std::getline(ss, line, ' '))
				{
					temp.extensions.push_back(line);
				}
			}
		};
		return temp;
	}

	uint32_t opengl_render_api::get_error() const
	{
		return _error<gl::to_user>(glGetError());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_render_api::get_alpha_enabled() const
	{
		return glIsEnabled(GL_ALPHA_TEST);
	}

	alpha_function opengl_render_api::get_alpha_function() const
	{
		return
		{
			std::invoke([]() -> uint32_t
			{
				uint32_t func{};
				glCheck(glGetIntegerv(GL_ALPHA_TEST_FUNC, (int32_t *)&func));
				return _predicate<gl::to_user>(func);
			}),
			std::invoke([]() -> float_t
			{
				float_t ref{};
				glCheck(glGetFloatv(GL_ALPHA_TEST_REF, &ref));
				return ref;
			})
		};
	}

	bool opengl_render_api::get_blend_enabled() const
	{
		return glIsEnabled(GL_BLEND);
	}

	color opengl_render_api::get_blend_color() const
	{
		color temp{};
		glCheck(glGetFloatv(GL_BLEND_COLOR, temp));
		return temp;
	}

	blend_equation opengl_render_api::get_blend_equation() const
	{
		auto helper = [](auto value) noexcept -> uint32_t
		{
			uint32_t temp{};
			glCheck(glGetIntegerv(value, (int32_t *)&temp));
			return _function<gl::to_user>(temp);
		};
		return {
			helper(GL_BLEND_EQUATION_RGB),
			helper(GL_BLEND_EQUATION_ALPHA)
		};
	}

	blend_function opengl_render_api::get_blend_function() const
	{
		auto helper = [](auto value) noexcept -> uint32_t
		{
			uint32_t temp{};
			glCheck(glGetIntegerv(value, (int32_t *)&temp));
			return _factor<gl::to_user>(temp);
		};
		return {
			helper(GL_BLEND_SRC_RGB), helper(GL_BLEND_SRC_ALPHA),
			helper(GL_BLEND_DST_RGB), helper(GL_BLEND_DST_ALPHA)
		};
	}

	color opengl_render_api::get_clear_color() const
	{
		color temp{};
		glCheck(glGetFloatv(GL_COLOR_CLEAR_VALUE, temp));
		return temp;
	}

	bool opengl_render_api::get_cull_enabled() const
	{
		return glIsEnabled(GL_CULL_FACE);
	}

	uint32_t opengl_render_api::get_cull_facet() const
	{
		uint32_t temp{};
		glCheck(glGetIntegerv(GL_CULL_FACE_MODE, (int32_t *)&temp));
		return _facet<gl::to_user>(temp);
	}

	uint32_t opengl_render_api::get_cull_order() const
	{
		uint32_t temp{};
		glCheck(glGetIntegerv(GL_FRONT_FACE, (int32_t *)&temp));
		return _front_face<gl::to_user>(temp);
	}

	bool opengl_render_api::get_depth_enabled() const
	{
		return glIsEnabled(GL_DEPTH_TEST);
	}

	uint32_t opengl_render_api::get_depth_function() const
	{
		uint32_t temp{};
		glCheck(glGetIntegerv(GL_DEPTH_FUNC, (int32_t *)&temp));
		return _predicate<gl::to_user>(temp);
	}

	bool opengl_render_api::get_depth_mask() const
	{
		return glIsEnabled(GL_DEPTH_WRITEMASK);
	}

	depth_range opengl_render_api::get_depth_range() const
	{
		depth_range temp{};
		glCheck(glGetFloatv(GL_DEPTH_RANGE, &temp.nearVal));
		return temp;
	}

	bool opengl_render_api::get_stencil_enabled() const
	{
		return glIsEnabled(GL_STENCIL_TEST);
	}

	int_rect opengl_render_api::get_viewport() const
	{
		int_rect temp{};
		glCheck(glGetIntegerv(GL_VIEWPORT, temp));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_render_api::set_alpha_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_ALPHA_TEST));
	}

	void opengl_render_api::set_alpha_function(alpha_function const & value)
	{
		glCheck(glAlphaFunc(_predicate<gl::to_impl>(value.func), value.ref));
	}

	void opengl_render_api::set_blend_color(color const & value)
	{
		glCheck(glBlendColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_render_api::set_blend_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_BLEND));
	}

	void opengl_render_api::set_blend_equation(blend_equation const & value)
	{
		glCheck(glBlendEquationSeparate(
			_function<gl::to_impl>(value.modeRGB),
			_function<gl::to_impl>(value.modeAlpha)));
	}

	void opengl_render_api::set_blend_function(blend_function const & value)
	{
		glCheck(glBlendFuncSeparate(
			_factor<gl::to_impl>(value.sfactorRGB),
			_factor<gl::to_impl>(value.dfactorRGB),
			_factor<gl::to_impl>(value.sfactorAlpha),
			_factor<gl::to_impl>(value.dfactorAlpha)));
	}

	void opengl_render_api::set_clear_color(color const & value)
	{
		glCheck(glClearColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_render_api::set_cull_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_CULL_FACE));
	}

	void opengl_render_api::set_cull_facet(uint32_t facet)
	{
		glCheck(glCullFace(_facet<gl::to_impl>(facet)));
	}

	void opengl_render_api::set_cull_order(uint32_t order)
	{
		glCheck(glFrontFace(_front_face<gl::to_impl>(order)));
	}

	void opengl_render_api::set_depth_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_DEPTH_TEST));
	}

	void opengl_render_api::set_depth_function(uint32_t predicate)
	{
		glCheck(glDepthFunc(_predicate<gl::to_impl>(predicate)));
	}

	void opengl_render_api::set_depth_mask(bool enabled)
	{
		glCheck(glDepthMask(enabled));
	}

	void opengl_render_api::set_depth_range(depth_range const & value)
	{
		glCheck(glDepthRangef(value.nearVal, value.farVal));
	}

	void opengl_render_api::set_stencil_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : glDisable)(GL_STENCIL_TEST));
	}

	void opengl_render_api::set_viewport(int_rect const & bounds)
	{
		glCheck(glViewport(bounds[0], bounds[1], bounds[2], bounds[3]));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_render_api::clear(uint32_t flags)
	{
		glCheck(glClear(_buffer_bit<gl::to_impl>(flags)));
	}

	void opengl_render_api::draw(shared<vertexarray> const & value)
	{
		if (!value) { return; }

		ML_bind_scope(value);

		if (value->get_ibo())
		{
			ML_bind_scope(value->get_ibo());

			for (auto const & vb : value->get_vbos())
			{
				ML_bind_scope(vb);

				this->draw_indexed(value->get_ibo()->get_count());
			}
		}
		else
		{
			for (auto const & vb : value->get_vbos())
			{
				ML_bind_scope(vb);

				this->draw_arrays(0, vb->get_size());
			}
		}
	}

	void opengl_render_api::draw_arrays(uint32_t first, uint32_t count)
	{
		glCheck(glDrawArrays(GL_TRIANGLES, first, count));
	}

	void opengl_render_api::draw_indexed(uint32_t count)
	{
		glCheck(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
	}

	void opengl_render_api::flush()
	{
		glCheck(glFlush());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // ML_IMPL_RENDERER_OPENGL