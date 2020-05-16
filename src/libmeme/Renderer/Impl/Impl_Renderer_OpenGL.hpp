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

#if (!ML_is_debug)
#	define glCheck(expr) (expr)
#else
#	define glCheck(expr) \
	do { expr; _ML gl_check_error(__FILE__, __LINE__, #expr); } while (0)
#endif

// errors
namespace ml
{
	static void gl_check_error(cstring file, uint32_t line, cstring expr)
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// enum mappings
namespace ml::gl
{
	constexpr uint32_t _capability(uint32_t value) noexcept
	{
		switch (value)
		{
		default										: return value;
		case capability_alpha_test					: return GL_ALPHA_TEST;
		case capability_blend						: return GL_BLEND;
		case capability_cull_face					: return GL_CULL_FACE;
		case capability_depth_clamp					: return GL_DEPTH_CLAMP;
		case capability_depth_test					: return GL_DEPTH_TEST;
		case capability_dither						: return GL_DITHER;
		case capability_framebuffer_srgb			: return GL_FRAMEBUFFER_SRGB;
		case capability_line_smooth					: return GL_LINE_SMOOTH;
		case capability_multisample					: return GL_MULTISAMPLE;
		case capability_polygon_smooth				: return GL_POLYGON_SMOOTH;
		case capability_sample_mask					: return GL_SAMPLE_MASK;
		case capability_scissor_test				: return GL_SCISSOR_TEST;
		case capability_stencil_test				: return GL_STENCIL_TEST;
		case capability_texture_1d					: return GL_TEXTURE_1D;
		case capability_texture_2d					: return GL_TEXTURE_2D;
		case capability_texture_3d					: return GL_TEXTURE_2D;
		case capability_texture_cube_map			: return GL_TEXTURE_CUBE_MAP;
		case capability_texture_cube_map_seamless	: return GL_TEXTURE_CUBE_MAP_SEAMLESS;
		}
	}

	constexpr uint32_t _error_type(uint32_t value) noexcept
	{
		switch (value)
		{
		default											: value;
		case error_type_none							: return GL_NO_ERROR;
		case error_type_invalid_enum					: return GL_INVALID_ENUM;
		case error_type_invalid_value					: return GL_INVALID_VALUE;
		case error_type_invalid_operation				: return GL_INVALID_OPERATION;
		case error_type_stack_overflow					: return GL_STACK_OVERFLOW;
		case error_type_stack_underflow					: return GL_STACK_UNDERFLOW;
		case error_type_out_of_memory					: return GL_OUT_OF_MEMORY;
		case error_type_invalid_framebuffer_operation	: return GL_INVALID_FRAMEBUFFER_OPERATION;
		}
	}

	constexpr uint32_t _clear_flags(uint32_t value) noexcept
	{
		switch (value)
		{
		default					: return value;
		case clear_flags_accum	: return GL_ACCUM_BUFFER_BIT;
		case clear_flags_color	: return GL_COLOR_BUFFER_BIT;
		case clear_flags_depth	: return GL_DEPTH_BUFFER_BIT;
		case clear_flags_stencil: return GL_STENCIL_BUFFER_BIT;
		}
	}

	constexpr uint32_t _usage(uint32_t value) noexcept
	{
		switch (value)
		{
		default					: return value;
		case usage_stream_draw	: return GL_STREAM_DRAW;
		case usage_static_draw	: return GL_STATIC_DRAW;
		case usage_dynamic_draw	: return GL_DYNAMIC_DRAW;
		}
	}

	constexpr uint32_t _primitive(uint32_t value) noexcept
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

	constexpr uint32_t _predicate(uint32_t value) noexcept
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

	constexpr uint32_t _type(uint32_t value) noexcept
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

	constexpr uint32_t _function(uint32_t value) noexcept
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

	constexpr uint32_t _front_face(uint32_t value) noexcept
	{
		switch (value)
		{
		default				: return value;
		case front_face_cw	: return GL_CW;
		case front_face_ccw	: return GL_CCW;
		}
	}

	constexpr uint32_t _facet(uint32_t value) noexcept
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

	constexpr uint32_t _factor(uint32_t value) noexcept
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

	constexpr uint32_t _format(uint32_t value) noexcept
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

	constexpr uint32_t _texture_type(uint32_t value) noexcept
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

	constexpr uint32_t _shader_type(uint32_t value) noexcept
	{
		switch (value)
		{
		default						: return value;
		case shader_type_vertex		: return GL_VERTEX_SHADER;
		case shader_type_fragment	: return GL_FRAGMENT_SHADER;
		case shader_type_geometry	: return GL_GEOMETRY_SHADER;
		}
	}

	constexpr uint32_t _color_attachment(uint32_t value) noexcept
	{
		switch (value)
		{
		default					: return value;
		case color_attachment_0	: return GL_COLOR_ATTACHMENT0;
		case color_attachment_1	: return GL_COLOR_ATTACHMENT1;
		case color_attachment_2	: return GL_COLOR_ATTACHMENT2;
		case color_attachment_3	: return GL_COLOR_ATTACHMENT3;
		case color_attachment_4	: return GL_COLOR_ATTACHMENT4;
		case color_attachment_5	: return GL_COLOR_ATTACHMENT5;
		case color_attachment_6	: return GL_COLOR_ATTACHMENT6;
		case color_attachment_7	: return GL_COLOR_ATTACHMENT7;
		case color_attachment_8	: return GL_COLOR_ATTACHMENT8;
		case color_attachment_9	: return GL_COLOR_ATTACHMENT9;
		case color_attachment_10: return GL_COLOR_ATTACHMENT10;
		case color_attachment_11: return GL_COLOR_ATTACHMENT11;
		case color_attachment_12: return GL_COLOR_ATTACHMENT12;
		case color_attachment_13: return GL_COLOR_ATTACHMENT13;
		case color_attachment_14: return GL_COLOR_ATTACHMENT14;
		case color_attachment_15: return GL_COLOR_ATTACHMENT15;
		}
	}

	constexpr uint32_t _draw_buffer(uint32_t value) noexcept
	{
		switch (value)
		{
		default				: return value;
		case draw_buffer_0	: return GL_DRAW_BUFFER0;
		case draw_buffer_1	: return GL_DRAW_BUFFER1;
		case draw_buffer_2	: return GL_DRAW_BUFFER2;
		case draw_buffer_3	: return GL_DRAW_BUFFER3;
		case draw_buffer_4	: return GL_DRAW_BUFFER4;
		case draw_buffer_5	: return GL_DRAW_BUFFER5;
		case draw_buffer_6	: return GL_DRAW_BUFFER6;
		case draw_buffer_7	: return GL_DRAW_BUFFER7;
		case draw_buffer_8	: return GL_DRAW_BUFFER8;
		case draw_buffer_9	: return GL_DRAW_BUFFER9;
		case draw_buffer_10	: return GL_DRAW_BUFFER10;
		case draw_buffer_11	: return GL_DRAW_BUFFER11;
		case draw_buffer_12	: return GL_DRAW_BUFFER12;
		case draw_buffer_13	: return GL_DRAW_BUFFER13;
		case draw_buffer_14	: return GL_DRAW_BUFFER14;
		case draw_buffer_15	: return GL_DRAW_BUFFER15;
		}
	}

	constexpr uint32_t _texture_attachment(uint32_t value) noexcept
	{
		switch (value)
		{
		default						: return value;
		case texture_attachment_0	: return GL_TEXTURE0;
		case texture_attachment_1	: return GL_TEXTURE1;
		case texture_attachment_2	: return GL_TEXTURE2;
		case texture_attachment_3	: return GL_TEXTURE3;
		case texture_attachment_4	: return GL_TEXTURE4;
		case texture_attachment_5	: return GL_TEXTURE5;
		case texture_attachment_6	: return GL_TEXTURE6;
		case texture_attachment_7	: return GL_TEXTURE7;
		case texture_attachment_8	: return GL_TEXTURE8;
		case texture_attachment_9	: return GL_TEXTURE9;
		case texture_attachment_10	: return GL_TEXTURE10;
		case texture_attachment_11	: return GL_TEXTURE11;
		case texture_attachment_12	: return GL_TEXTURE12;
		case texture_attachment_13	: return GL_TEXTURE13;
		case texture_attachment_14	: return GL_TEXTURE14;
		case texture_attachment_15	: return GL_TEXTURE15;
		case texture_attachment_16	: return GL_TEXTURE16;
		case texture_attachment_17	: return GL_TEXTURE17;
		case texture_attachment_18	: return GL_TEXTURE18;
		case texture_attachment_19	: return GL_TEXTURE19;
		case texture_attachment_20	: return GL_TEXTURE20;
		case texture_attachment_21	: return GL_TEXTURE21;
		case texture_attachment_22	: return GL_TEXTURE22;
		case texture_attachment_23	: return GL_TEXTURE23;
		case texture_attachment_24	: return GL_TEXTURE24;
		case texture_attachment_25	: return GL_TEXTURE25;
		case texture_attachment_26	: return GL_TEXTURE26;
		case texture_attachment_27	: return GL_TEXTURE27;
		case texture_attachment_28	: return GL_TEXTURE28;
		case texture_attachment_29	: return GL_TEXTURE29;
		case texture_attachment_30	: return GL_TEXTURE30;
		case texture_attachment_31	: return GL_TEXTURE31;
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl buffers
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl vertex array
	struct ML_RENDERER_API opengl_vertex_array final : vertex_array
	{
		using self_type = opengl_vertex_array;

		opengl_vertex_array() { glCheck(glGenVertexArrays(1, &m_handle)); }

		~opengl_vertex_array() { glCheck(glDeleteVertexArrays(1, &m_handle)); }

		opengl_vertex_array(uint32_t mode) : self_type{}
		{
			m_mode = mode;
		}

		static void bind(opengl_vertex_array const * value)
		{
			glCheck(glBindVertexArray(value ? value->m_handle : NULL));
		}

		void * get_handle() const noexcept override { return ML_addressof(m_handle); }

		uint32_t get_mode() const noexcept override { return m_mode; }

	private:
		uint32_t m_handle	{}						; // handle
		uint32_t m_mode		{ GL_TRIANGLES }		; // primitive type
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl vertex buffer
	struct ML_RENDERER_API opengl_vertex_buffer final : vertex_buffer
	{
		using self_type = opengl_vertex_buffer;

		opengl_vertex_buffer() { glCheck(glGenBuffers(1, &m_handle)); }

		~opengl_vertex_buffer() { glCheck(glDeleteBuffers(1, &m_handle)); }

		opengl_vertex_buffer(float_t const * vertices, uint32_t size) : self_type{}
		{
			this->set_data(vertices, size);
		}

		opengl_vertex_buffer(float_t const * vertices, uint32_t size, uint32_t offset) : self_type{}
		{
			this->set_sub_data(vertices, size, offset);
		}

		static void bind(opengl_vertex_buffer const * value)
		{
			glCheck(glBindBuffer(GL_ARRAY_BUFFER, value ? value->m_handle : NULL));
		}

		void set_data(float_t const * vertices, uint32_t size) override
		{
			glCheck(glBufferData(GL_ARRAY_BUFFER, (m_size = size), vertices, GL_STATIC_DRAW));
		}

		void set_sub_data(float_t const * vertices, uint32_t size, uint32_t offset) override
		{
			glCheck(glBufferSubData(GL_ARRAY_BUFFER, offset, (m_size = size), vertices));
		}

		void * get_handle() const noexcept override { return ML_addressof(m_handle); }

		uint32_t get_size() const noexcept override { return m_size; }

	private:
		uint32_t m_handle	{}; // handle
		uint32_t m_size		{}; // size
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl index buffer
	struct ML_RENDERER_API opengl_index_buffer final : index_buffer
	{
		using self_type = opengl_index_buffer;

		opengl_index_buffer() { glCheck(glGenBuffers(1, &m_handle)); }

		~opengl_index_buffer() { glCheck(glDeleteBuffers(1, &m_handle)); }

		opengl_index_buffer(uint32_t const * indices, uint32_t count) : self_type{}
		{
			this->set_data(indices, count);
		}

		static void bind(opengl_index_buffer const * value)
		{
			glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, value ? value->m_handle : NULL));
		}

		void set_data(uint32_t const * indices, uint32_t count) override
		{
			glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (m_count = count) * sizeof(uint32_t), indices, GL_UNSIGNED_INT));
		}

		void * get_handle() const noexcept override { return ML_addressof(m_handle); }

		uint32_t get_count() const noexcept override { return m_count; }

	private:
		uint32_t m_handle	{}; // handle
		uint32_t m_count	{}; // count
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl frame buffer
	struct ML_RENDERER_API opengl_frame_buffer final : frame_buffer
	{
		using self_type = opengl_frame_buffer;

		opengl_frame_buffer() { glCheck(glGenFramebuffers(1, &m_handle)); }

		~opengl_frame_buffer() { glCheck(glDeleteFramebuffers(1, &m_handle)); }

		static void bind(opengl_frame_buffer const * value)
		{
			glCheck(glBindFramebuffer(GL_FRAMEBUFFER, value ? value->m_handle : NULL));
		}

		void set_render_buffer(void const * value, uint32_t attachment) override
		{
			glCheck(glFramebufferRenderbuffer(
				GL_FRAMEBUFFER, attachment,
				GL_RENDERBUFFER, *reinterpret_cast<uint32_t const *>(value)));
		}

		void set_texture2d(void const * value, uint32_t attachment, uint32_t level) override
		{
			glCheck(glFramebufferTexture2D(
				GL_FRAMEBUFFER, attachment,
				GL_TEXTURE_2D, *reinterpret_cast<uint32_t const *>(value), level));
		}

		void * get_handle() const noexcept override { return ML_addressof(m_handle); }

	private:
		uint32_t m_handle{}; // handle
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl render buffer
	struct ML_RENDERER_API opengl_render_buffer final : render_buffer
	{
		using self_type = opengl_render_buffer;

		opengl_render_buffer() { glCheck(glGenRenderbuffers(1, &m_handle)); }

		~opengl_render_buffer() { glCheck(glDeleteRenderbuffers(1, &m_handle)); }

		opengl_render_buffer(uint32_t format, int32_t width, int32_t height) : self_type{}
		{
			this->set_storage(format, width, height);
		}

		static void bind(opengl_render_buffer const * value)
		{
			glCheck(glBindRenderbuffer(GL_RENDERBUFFER, value ? value->m_handle : NULL));
		}

		void set_storage(uint32_t format, int32_t width, int32_t height) override
		{
			glCheck(glRenderbufferStorage(GL_RENDERBUFFER, gl::_format(format), width, height));
		}

		void * get_handle() const noexcept override { return ML_addressof(m_handle); }

	private:
		uint32_t m_handle{}; // handle
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl render api
namespace ml
{
	struct ML_RENDERER_API opengl_render_api final : render_api
	{
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

		gl::alpha_function get_alpha_function() const override;
		
		color get_blend_color() const override;
		
		gl::blend_equation	get_blend_equation() const override;
		
		gl::blend_function get_blend_function() const override;
		
		color get_clear_color() const override;
		
		uint32_t get_depth_function() const override;
		
		bool get_depth_mask() const override;

		gl::depth_range get_depth_range() const override;
		
		bool get_enabled(uint32_t capability) const override;
		
		uint32_t get_front_face() const override;
		
		int_rect get_viewport() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_alpha_function(gl::alpha_function const & value) override;
		
		void set_blend_color(color const & value) override;
		
		void set_blend_equation(gl::blend_equation const & value) override;
		
		void set_blend_function(gl::blend_function const & value) override;
		
		void set_clear_color(color const & value) override;
		
		void set_cull_mode(uint32_t facet) override;
		
		void set_depth_function(uint32_t predicate) override;
		
		void set_depth_mask(bool enabled) override;

		void set_depth_range(gl::depth_range const & value) override;
		
		void set_enabled(uint32_t capability, bool enabled) override;
		
		void set_front_face(uint32_t front_face) override;
		
		void set_viewport(int_rect const & bounds) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear(uint32_t flags) override;
		
		void draw_arrays(uint32_t primitive, uint32_t first, uint32_t count) override;
		
		void draw_indexed(uint32_t primitive, int32_t first, uint32_t type, void const * indices) override;
		
		void flush() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMPL_RENDERER_OPENGL_HPP_
#endif // ML_IMPL_RENDERER_OPENGL