#if defined(ML_IMPL_RENDERER_OPENGL)

#include "Impl_Renderer_OpenGL.hpp"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_OPENGL_LOADER_CUSTOM
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
	do { expr; _ML gl::check_error(__FILE__, __LINE__, #expr); } while (0)
#endif

// errors
namespace ml::gl
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
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convert> constexpr uint32_t _clear_flags(uint32_t value) noexcept
	{
		uint32_t temp{};
		if constexpr (Convert()) // to_impl
		{
			ML_flag_map(temp, GL_ACCUM_BUFFER_BIT	, value, clear_flags_accum);
			ML_flag_map(temp, GL_COLOR_BUFFER_BIT	, value, clear_flags_color);
			ML_flag_map(temp, GL_DEPTH_BUFFER_BIT	, value, clear_flags_depth);
			ML_flag_map(temp, GL_STENCIL_BUFFER_BIT	, value, clear_flags_stencil);
		}
		else // to_user
		{
			ML_flag_map(temp, clear_flags_accum		, value, GL_ACCUM_BUFFER_BIT);
			ML_flag_map(temp, clear_flags_color		, value, GL_COLOR_BUFFER_BIT);
			ML_flag_map(temp, clear_flags_depth		, value, GL_DEPTH_BUFFER_BIT);
			ML_flag_map(temp, clear_flags_stencil	, value, GL_STENCIL_BUFFER_BIT);
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convert> constexpr uint32_t _error(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
		{
			switch (value)
			{
			default									: return value;
			case error_no_error						: return GL_NO_ERROR;
			case error_invalid_enum					: return GL_INVALID_ENUM;
			case error_invalid_value				: return GL_INVALID_VALUE;
			case error_invalid_operation			: return GL_INVALID_OPERATION;
			case error_stack_overflow				: return GL_STACK_OVERFLOW;
			case error_stack_underflow				: return GL_STACK_UNDERFLOW;
			case error_out_of_memory				: return GL_OUT_OF_MEMORY;
			case error_invalid_framebuffer_operation: return GL_INVALID_FRAMEBUFFER_OPERATION;
			}
		}
		else // to_user
		{
			switch (value)
			{
			default									: return value;
			case GL_NO_ERROR						: return error_no_error;
			case GL_INVALID_ENUM					: return error_invalid_enum;
			case GL_INVALID_VALUE					: return error_invalid_value;
			case GL_INVALID_OPERATION				: return error_invalid_operation;
			case GL_STACK_OVERFLOW					: return error_stack_overflow;
			case GL_STACK_UNDERFLOW					: return error_stack_underflow;
			case GL_OUT_OF_MEMORY					: return error_out_of_memory;
			case GL_INVALID_FRAMEBUFFER_OPERATION	: return error_invalid_framebuffer_operation;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convert> constexpr uint32_t _usage(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
		{
			switch (value)
			{
			default					: return value;
			case usage_stream	: return GL_STREAM_DRAW;
			case usage_static	: return GL_STATIC_DRAW;
			case usage_dynamic	: return GL_DYNAMIC_DRAW;
			}
		}
		else // to_user
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

	template <class Convert> constexpr uint32_t _primitive(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
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
		else // to_user
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

	template <class Convert> constexpr uint32_t _predicate(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
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
		else // to_user
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

	template <class Convert> constexpr uint32_t _type(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
		{
			switch (value)
			{
			default					: return value;
			case type_byte			: return GL_BYTE;
			case type_unsigned_byte	: return GL_UNSIGNED_BYTE;
			case type_short			: return GL_SHORT;
			case type_unsigned_short: return GL_UNSIGNED_SHORT;
			case type_int			: return GL_INT;
			case type_unsigned_int	: return GL_UNSIGNED_INT;
			case type_float			: return GL_FLOAT;
			case type_half_float	: return GL_HALF_FLOAT;
			}
		}
		else // to_user
		{
			switch (value)
			{
			default					: return value;
			case GL_BYTE			: return type_byte;
			case GL_UNSIGNED_BYTE	: return type_unsigned_byte;
			case GL_SHORT			: return type_short;
			case GL_UNSIGNED_SHORT	: return type_unsigned_short;
			case GL_INT				: return type_int;
			case GL_UNSIGNED_INT	: return type_unsigned_int;
			case GL_FLOAT			: return type_float;
			case GL_HALF_FLOAT		: return type_half_float;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convert> constexpr uint32_t _function(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
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
		else // to_user
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

	template <class Convert> constexpr uint32_t _order(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
		{
			switch (value)
			{
			default						: return value;
			case order_clockwise		: return GL_CW;
			case order_counter_clockwise: return GL_CCW;
			}
		}
		else // to_user
		{
			switch (value)
			{
			default		: return value;
			case GL_CW	: return order_clockwise;
			case GL_CCW	: return order_counter_clockwise;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convert> constexpr uint32_t _facet(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
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
		else // to_user
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

	template <class Convert> constexpr uint32_t _factor(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
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
		else // to_user
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

	template <class Convert> constexpr uint32_t _format(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
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
			case format_depth24_stencil8	: return GL_DEPTH24_STENCIL8;
			}
		}
		else // to_user
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
			case GL_DEPTH24_STENCIL8		: return format_depth24_stencil8;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convert> constexpr uint32_t _texture_type(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
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
		else // to_user
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

	template <class Convert> constexpr uint32_t _shader_type(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
		{
			switch (value)
			{
			default						: return value;
			case shader_type_vertex		: return GL_VERTEX_SHADER;
			case shader_type_fragment	: return GL_FRAGMENT_SHADER;
			case shader_type_geometry	: return GL_GEOMETRY_SHADER;
			}
		}
		else // to_user
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

	template <class Convert> constexpr uint32_t _attachment(uint32_t value) noexcept
	{
		if constexpr (Convert()) // to_impl
		{
			switch (value)
			{
			default							: return GL_COLOR_ATTACHMENT0 + value;
			case depth_attchment			: return GL_DEPTH_ATTACHMENT;
			case stencil_attachment			: return GL_STENCIL_ATTACHMENT;
			case depth_stencil_attachment	: return GL_DEPTH_STENCIL_ATTACHMENT;
			}
		}
		else // to_user
		{
			switch (value)
			{
			default							: return color_attachment_0 + std::distance(GL_COLOR_ATTACHMENT0, value);
			case GL_DEPTH_ATTACHMENT		: return depth_attchment;
			case GL_STENCIL_ATTACHMENT		: return stencil_attachment;
			case GL_DEPTH_STENCIL_ATTACHMENT: return depth_stencil_attachment;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl objects
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_vertex_array::opengl_vertex_array(uint32_t mode)
		: m_mode{ mode }
	{
		glCheck(glGenVertexArrays(1, &m_handle));
	}

	opengl_vertex_array::~opengl_vertex_array()
	{
		glCheck(glDeleteVertexArrays(1, &m_handle));
	}

	void opengl_vertex_array::bind() const
	{
		glCheck(glBindVertexArray(m_handle));
	}

	void opengl_vertex_array::unbind() const
	{
		glCheck(glBindVertexArray(NULL));
	}

	void opengl_vertex_array::add_vertices(shared<vertex_buffer> const & value)
	{
		this->bind();
		value->bind();
		auto const & layout{ value->get_layout() };
		for (auto const & e : layout.get_elements())
		{
			glCheck(glEnableVertexAttribArray(m_index));
			glCheck(glVertexAttribPointer
			(
				m_index++,
				e.get_component_count(),
				std::invoke([&]() noexcept -> uint32_t
				{
					switch (e.get_base_type())
					{
					default					: return 0;
					case hashof_v<bool>		: return GL_BOOL;
					case hashof_v<int32_t>	: return GL_INT;
					case hashof_v<float_t>	: return GL_FLOAT;
					}
				}),
				e.normalized,
				layout.get_stride(),
				(buffer)(intptr_t)e.offset
			));
		}
		m_vertices.push_back(value);
	}

	void opengl_vertex_array::set_indices(shared<index_buffer> const & value)
	{
		this->bind();
		value->bind();
		m_indices = value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_vertex_buffer::opengl_vertex_buffer(buffer vertices, uint32_t size, uint32_t usage)
		: m_usage{ usage }
	{
		glCheck(glGenBuffers(1, &m_handle));
		this->bind();
		glCheck(glBufferData(GL_ARRAY_BUFFER, size, vertices, _usage<to_impl>(m_usage)));
	}

	opengl_vertex_buffer::opengl_vertex_buffer(uint32_t size, uint32_t usage)
		: opengl_vertex_buffer{ nullptr, size, usage }
	{
	}

	opengl_vertex_buffer::~opengl_vertex_buffer()
	{
		glCheck(glDeleteBuffers(1, &m_handle));
	}

	void opengl_vertex_buffer::bind() const
	{
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
	}

	void opengl_vertex_buffer::unbind() const
	{
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, NULL));
	}

	void opengl_vertex_buffer::set_data(buffer vertices, uint32_t size, uint32_t offset)
	{
		m_size = size;
		glCheck(glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_index_buffer::opengl_index_buffer(buffer indices, uint32_t count)
	{
		glCheck(glGenBuffers(1, &m_handle));
		this->bind();
		this->set_data(indices, count);
	}

	opengl_index_buffer::~opengl_index_buffer()
	{
		glCheck(glDeleteBuffers(1, &m_handle));
	}

	void opengl_index_buffer::bind() const
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle));
	}

	void opengl_index_buffer::unbind() const
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));
	}

	void opengl_index_buffer::set_data(buffer indices, uint32_t count)
	{
		m_count = count;
		glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_frame_buffer::opengl_frame_buffer::opengl_frame_buffer(uint32_t format, vec2i const & size)
	{
		m_format = format;
		m_size = size;
		glCheck(glGenFramebuffers(1, &m_handle));
	}

	opengl_frame_buffer::~opengl_frame_buffer()
	{
		glCheck(glDeleteFramebuffers(1, &m_handle));
	}

	void opengl_frame_buffer::bind() const
	{
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_handle));
		glCheck(glViewport(0, 0, m_size[0], m_size[1]));
	}

	void opengl_frame_buffer::unbind() const
	{
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
	}

	void opengl_frame_buffer::resize(vec2i const & value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_shader_object::opengl_shader_object()
	{
		glCheck(m_handle = glCreateProgramObjectARB());
	}

	opengl_shader_object::~opengl_shader_object()
	{
		glCheck(glDeleteObjectARB(m_handle));
	}

	void opengl_shader_object::bind() const
	{
		glCheck(glUseProgramObjectARB(m_handle));
	}

	void opengl_shader_object::unbind() const
	{
		glCheck(glUseProgramObjectARB(NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_texture_object::opengl_texture_object()
	{
		glCheck(glGenTextures(1, &m_handle));
	}

	opengl_texture_object::~opengl_texture_object()
	{
		glCheck(glDeleteTextures(1, &m_handle));
	}

	void opengl_texture_object::bind() const
	{
		glCheck(glBindTexture(_texture_type<to_impl>(m_type), m_handle));
	}

	void opengl_texture_object::unbind() const
	{
		glCheck(glBindTexture(_texture_type<to_impl>(m_type), NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// opengl api
namespace ml::gl
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t opengl_render_api::get_error() const
	{
		return _error<to_user>(glGetError());
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
			return _function<to_user>(temp);
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
			return _factor<to_user>(temp);
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

	uint32_t opengl_render_api::get_cull_face() const
	{
		uint32_t temp{};
		glCheck(glGetIntegerv(GL_CULL_FACE_MODE, (int32_t *)&temp));
		return _facet<to_user>(temp);
	}

	uint32_t opengl_render_api::get_cull_order() const
	{
		uint32_t temp{};
		glCheck(glGetIntegerv(GL_FRONT_FACE, (int32_t *)&temp));
		return _order<to_user>(temp);
	}

	bool opengl_render_api::get_depth_enabled() const
	{
		return glIsEnabled(GL_DEPTH_TEST);
	}

	uint32_t opengl_render_api::get_depth_function() const
	{
		uint32_t temp{};
		glCheck(glGetIntegerv(GL_DEPTH_FUNC, (int32_t *)&temp));
		return _predicate<to_user>(temp);
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
		glCheck(glAlphaFunc(_predicate<to_impl>(value.func), value.ref));
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
			_function<to_impl>(value.modeRGB),
			_function<to_impl>(value.modeAlpha)));
	}

	void opengl_render_api::set_blend_function(blend_function const & value)
	{
		glCheck(glBlendFuncSeparate(
			_factor<to_impl>(value.sfactorRGB),
			_factor<to_impl>(value.dfactorRGB),
			_factor<to_impl>(value.sfactorAlpha),
			_factor<to_impl>(value.dfactorAlpha)));
	}

	void opengl_render_api::set_clear_color(color const & value)
	{
		glCheck(glClearColor(value[0], value[1], value[2], value[3]));
	}

	void opengl_render_api::set_cull_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_CULL_FACE));
	}

	void opengl_render_api::set_cull_face(uint32_t facet)
	{
		glCheck(glCullFace(_facet<to_impl>(facet)));
	}

	void opengl_render_api::set_cull_order(uint32_t order)
	{
		glCheck(glFrontFace(_order<to_impl>(order)));
	}

	void opengl_render_api::set_depth_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_DEPTH_TEST));
	}

	void opengl_render_api::set_depth_function(uint32_t predicate)
	{
		glCheck(glDepthFunc(_predicate<to_impl>(predicate)));
	}

	void opengl_render_api::set_depth_mask(bool enabled)
	{
		glCheck(glDepthMask(enabled));
	}

	void opengl_render_api::set_depth_range(depth_range const & value)
	{
		glCheck(glDepthRangef(value.nearVal, value.farVal));
	}

	void opengl_render_api::set_viewport(int_rect const & bounds)
	{
		glCheck(glViewport(bounds[0], bounds[1], bounds[2], bounds[3]));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void opengl_render_api::clear(uint32_t flags)
	{
		glCheck(glClear(_clear_flags<to_impl>(flags)));
	}

	void opengl_render_api::draw(shared<vertex_array> const & value)
	{
		value->bind();
		value->get_vertices().front()->bind();
		value->get_indices()->bind();
		glCheck(glDrawElements(
			GL_TRIANGLES,
			value->get_indices()->get_count(),
			GL_UNSIGNED_INT,
			nullptr));
	}

	void opengl_render_api::draw_arrays(uint32_t primitive, uint32_t first, uint32_t count)
	{
		glCheck(glDrawArrays(_primitive<to_impl>(primitive), first, count));
	}

	void opengl_render_api::draw_indexed(uint32_t primitive, int32_t first, uint32_t index_type, buffer indices)
	{
		glCheck(glDrawElements(_primitive<to_impl>(primitive), first, _type<to_impl>(index_type), indices));
	}

	void opengl_render_api::flush()
	{
		glCheck(glFlush());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // ML_IMPL_RENDERER_OPENGL