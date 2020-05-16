#if defined(ML_IMPL_RENDERER_OPENGL)
#ifndef _ML_IMPL_RENDERER_OPENGL_HPP_
#define _ML_IMPL_RENDERER_OPENGL_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Renderer/RendererAPI.hpp>

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

	template <class Convt> constexpr uint32_t _clear_flags(uint32_t value) noexcept
	{
		uint32_t temp{};
		if constexpr (Convt{})
		{
			ML_flag_map(temp, value, clear_flags_accum	, GL_ACCUM_BUFFER_BIT);
			ML_flag_map(temp, value, clear_flags_color	, GL_COLOR_BUFFER_BIT);
			ML_flag_map(temp, value, clear_flags_depth	, GL_DEPTH_BUFFER_BIT);
			ML_flag_map(temp, value, clear_flags_stencil, GL_STENCIL_BUFFER_BIT);
		}
		else
		{
			ML_flag_map(temp, value, GL_ACCUM_BUFFER_BIT	, clear_flags_accum);
			ML_flag_map(temp, value, GL_COLOR_BUFFER_BIT	, clear_flags_color);
			ML_flag_map(temp, value, GL_DEPTH_BUFFER_BIT	, clear_flags_depth);
			ML_flag_map(temp, value, GL_STENCIL_BUFFER_BIT	, clear_flags_stencil);
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _error(uint32_t value) noexcept
	{
		if constexpr (Convt{})
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
			}
		}
		else
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
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _usage(uint32_t value) noexcept
	{
		if constexpr (Convt{})
		{
			switch (value)
			{
			default					: return value;
			case usage_stream_draw	: return GL_STREAM_DRAW;
			case usage_static_draw	: return GL_STATIC_DRAW;
			case usage_dynamic_draw	: return GL_DYNAMIC_DRAW;
			}
		}
		else
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
		if constexpr (Convt{})
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
		else
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
		if constexpr (Convt{})
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
		else
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
		if constexpr (Convt{})
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
		else
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

	template <class Convt> constexpr uint32_t _function(uint32_t value) noexcept
	{
		if constexpr (Convt{})
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
		else
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

	template <class Convt> constexpr uint32_t _cull_order(uint32_t value) noexcept
	{
		if constexpr (Convt{})
		{
			switch (value)
			{
			default				: return value;
			case cull_order_cw	: return GL_CW;
			case cull_order_ccw	: return GL_CCW;
			}
		}
		else
		{
			switch (value)
			{
			default		: return value;
			case GL_CW	: return cull_order_cw;
			case GL_CCW	: return cull_order_ccw;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Convt> constexpr uint32_t _facet(uint32_t value) noexcept
	{
		if constexpr (Convt{})
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
		else
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
		if constexpr (Convt{})
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
		else
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
		if constexpr (Convt{})
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
		else
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

	template <class Convt> constexpr uint32_t _texture_type(uint32_t value) noexcept
	{
		if constexpr (Convt{})
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
		else
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
		if constexpr (Convt{})
		{
			switch (value)
			{
			default						: return value;
			case shader_type_vertex		: return GL_VERTEX_SHADER;
			case shader_type_fragment	: return GL_FRAGMENT_SHADER;
			case shader_type_geometry	: return GL_GEOMETRY_SHADER;
			}
		}
		else
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

	template <class Convt> constexpr uint32_t _attachment(uint32_t value) noexcept
	{
		if constexpr (Convt{})
		{
			switch (value)
			{
			default							: return GL_COLOR_ATTACHMENT0 + value;
			case depth_attchment			: return GL_DEPTH_ATTACHMENT;
			case stencil_attachment			: return GL_STENCIL_ATTACHMENT;
			case depth_stencil_attachment	: return GL_DEPTH_STENCIL_ATTACHMENT;
			}
		}
		else
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

	// opengl vertex array
	class opengl_vertex_array final : public vertex_array
	{
	public:
		using self_type = opengl_vertex_array;

		opengl_vertex_array(uint32_t mode);

		~opengl_vertex_array();

		void bind() const override;

		void unbind() const override;

		inline handle get_handle() const override { return ML_addressof(m_handle); }

		inline uint32_t get_mode() const override { return m_mode; }

		void add_vertices(shared<vertex_buffer> const & value) override;

		void set_indices(shared<index_buffer> const & value) override;

		inline vector<shared<vertex_buffer>> const & get_vertices() const override { return m_vbos; }

		inline shared<index_buffer> const & get_indices() const override { return m_ibo; }

	private:
		uint32_t m_handle	{}						; // handle
		uint32_t m_mode		{ primitive_triangles }	; // draw mode
		
		vector<shared<vertex_buffer>>	m_vbos	{}	; // vertex buffers
		shared<index_buffer>			m_ibo	{}	; // index buffer
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl vertex buffer
	class opengl_vertex_buffer final : public vertex_buffer
	{
	public:
		using self_type = opengl_vertex_buffer;

		opengl_vertex_buffer(buffer vertices, uint32_t size);

		opengl_vertex_buffer(buffer vertices, uint32_t size, uint32_t offset);

		~opengl_vertex_buffer();

		void bind() const override;

		void unbind() const override;

		inline handle get_handle() const override { return ML_addressof(m_handle); }

		inline uint32_t get_size() const override { return m_size; }

	private:
		uint32_t m_handle	{};
		uint32_t m_size		{};
		uint32_t m_usage	{};

	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl index buffer
	class opengl_index_buffer final : public index_buffer
	{
	public:
		using self_type = opengl_index_buffer;

		opengl_index_buffer(buffer indices, uint32_t count);

		~opengl_index_buffer();

		void bind() const override;

		void unbind() const override;

		inline handle get_handle() const override { return ML_addressof(m_handle); }

		inline uint32_t get_count() const override { return m_count; }

	private:
		uint32_t m_handle	{};
		uint32_t m_count	{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl frame buffer
	class opengl_frame_buffer final : public frame_buffer
	{
	public:
		using self_type = opengl_frame_buffer;

		opengl_frame_buffer();

		~opengl_frame_buffer();

		void bind() const override;

		void unbind() const override;

		inline uint32_t get_color_attachment() const override { return m_color_attachment; }

		inline uint32_t get_depth_attachment() const override { return m_depth_attachment; }

		inline vec2i get_size() const override { return m_size; }

		inline uint32_t get_format() const override { return m_format; }

		inline handle get_handle() const override { return ML_addressof(m_handle); }

	private:
		uint32_t	m_handle			{};
		uint32_t	m_format			{};
		vec2i		m_size				{};
		uint32_t	m_color_attachment	{};
		uint32_t	m_depth_attachment	{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl shader
	class opengl_shader_object final : public shader_object
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		opengl_shader_object();

		~opengl_shader_object();

		void bind() const override;

		void unbind() const override;

		inline handle get_handle() const override { return ML_addressof(m_handle); }

	private:
		uint32_t					m_handle	{};
		ds::map<hash_t, int32_t>	m_attribs	{};
		ds::map<hash_t, int32_t>	m_uniforms	{};
		ds::map<hash_t, handle>		m_textures	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl texture
	class opengl_texture_object final : public texture_object
	{
	public:
		opengl_texture_object();

		~opengl_texture_object();

		void bind() const override;

		void unbind() const override;

		inline handle get_handle() const override { return ML_addressof(m_handle); }

		inline uint32_t get_type() const override { return m_type; }

	private:
		uint32_t m_handle	{};
		uint32_t m_type		{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl api
namespace ml::gl
{
	class opengl_render_api final : public render_api
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		~opengl_render_api() noexcept = default;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool initialize() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t get_error() const override;
		
		cstring get_extensions() const override;
		
		int32_t get_major_version() const override;
		
		int32_t get_minor_version() const override;
		
		int32_t get_num_extensions() const override;
		
		cstring get_renderer() const override;
		
		cstring get_shading_language_version() const override;
		
		cstring get_vendor() const override;
		
		cstring get_version() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool get_alpha_enabled() const override;

		alpha_function get_alpha_function() const override;
		
		bool get_blend_enabled() const override;

		color get_blend_color() const override;
		
		blend_equation get_blend_equation() const override;
		
		blend_function get_blend_function() const override;
		
		color get_clear_color() const override;

		bool get_cull_enabled() const override;

		uint32_t get_cull_face() const override;

		uint32_t get_cull_order() const override;

		bool get_depth_enabled() const override;
		
		uint32_t get_depth_function() const override;
		
		bool get_depth_mask() const override;

		depth_range get_depth_range() const override;

		bool get_multisample_enabled() const override;
		
		int_rect get_viewport() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_alpha_enabled(bool enabled) override;

		void set_alpha_function(alpha_function const & value) override;
		
		void set_blend_color(color const & value) override;

		void set_blend_enabled(bool enabled) override;
		
		void set_blend_equation(blend_equation const & value) override;
		
		void set_blend_function(blend_function const & value) override;
		
		void set_clear_color(color const & value) override;
		
		void set_cull_enabled(bool enabled) override;

		void set_cull_face(uint32_t facet) override;

		void set_cull_order(uint32_t order) override;

		void set_depth_enabled(bool enabled) override;
		
		void set_depth_function(uint32_t predicate) override;
		
		void set_depth_mask(bool enabled) override;

		void set_depth_range(depth_range const & value) override;

		void set_multisample_enabled(bool enabled) override;

		void set_viewport(int_rect const & bounds) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear(uint32_t flags) override;
		
		void draw_arrays(uint32_t primitive, uint32_t first, uint32_t count) override;
		
		void draw_indexed(uint32_t primitive, int32_t first, uint32_t index_type, void const * indices) override;
		
		void flush() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMPL_RENDERER_OPENGL_HPP_
#endif // ML_IMPL_RENDERER_OPENGL