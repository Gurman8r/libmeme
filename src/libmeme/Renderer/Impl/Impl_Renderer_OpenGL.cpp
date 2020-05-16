#if defined(ML_IMPL_RENDERER_OPENGL)

#include "Impl_Renderer_OpenGL.hpp"

// opengl resources
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_vertex_array::opengl_vertex_array()
		: m_handle{}, m_mode{ primitive_triangles }
	{
		glCheck(glGenVertexArrays(1, &m_handle));
	}

	opengl_vertex_array::~opengl_vertex_array()
	{
		glCheck(glDeleteVertexArrays(1, &m_handle));
	}

	opengl_vertex_array::opengl_vertex_array(uint32_t mode)
		: self_type{}
	{
		m_mode = mode;
	}

	void opengl_vertex_array::bind(opengl_vertex_array const * value)
	{
		glCheck(glBindVertexArray(value ? value->m_handle : NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_vertex_buffer::opengl_vertex_buffer()
		: m_handle{}, m_size{}
	{
		glCheck(glGenBuffers(1, &m_handle));
	}

	opengl_vertex_buffer::~opengl_vertex_buffer()
	{
		glCheck(glDeleteBuffers(1, &m_handle));
	}

	opengl_vertex_buffer::opengl_vertex_buffer(float_t const * vertices, uint32_t size)
		: self_type{}
	{
		this->set_data(vertices, size);
	}

	opengl_vertex_buffer::opengl_vertex_buffer(float_t const * vertices, uint32_t size, uint32_t offset)
		: self_type{}
	{
		this->set_sub_data(vertices, size, offset);
	}

	void opengl_vertex_buffer::bind(opengl_vertex_buffer const * value)
	{
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, value ? value->m_handle : NULL));
	}

	void opengl_vertex_buffer::set_data(float_t const * vertices, uint32_t size)
	{
		glCheck(glBufferData(GL_ARRAY_BUFFER, (m_size = size), vertices, GL_STATIC_DRAW));
	}

	void opengl_vertex_buffer::set_sub_data(float_t const * vertices, uint32_t size, uint32_t offset)
	{
		glCheck(glBufferSubData(GL_ARRAY_BUFFER, offset, (m_size = size), vertices));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_index_buffer::opengl_index_buffer()
		: m_handle{}, m_count{}
	{
		glCheck(glGenBuffers(1, &m_handle));
	}

	opengl_index_buffer::~opengl_index_buffer()
	{
		glCheck(glDeleteBuffers(1, &m_handle));
	}

	opengl_index_buffer::opengl_index_buffer(uint32_t const * indices, uint32_t count)
		: self_type{}
	{
		this->set_data(indices, count);
	}

	void opengl_index_buffer::bind(opengl_index_buffer const * value)
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, value ? value->m_handle : NULL));
	}

	uint32_t opengl_index_buffer::get_count() const
	{
		return m_count;
	}

	void opengl_index_buffer::set_data(uint32_t const * indices, uint32_t count)
	{
		glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (m_count = count) * sizeof(uint32_t), indices, GL_UNSIGNED_INT));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_frame_buffer::opengl_frame_buffer::opengl_frame_buffer()
		: m_handle{}
	{
		glCheck(glGenFramebuffers(1, &m_handle));
	}

	opengl_frame_buffer::~opengl_frame_buffer()
	{
		glCheck(glDeleteFramebuffers(1, &m_handle));
	}

	void opengl_frame_buffer::bind(opengl_frame_buffer const * value)
	{
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, value ? value->m_handle : NULL));
	}

	void opengl_frame_buffer::set_render_buffer(void const * value, uint32_t attachment)
	{
		glCheck(glFramebufferRenderbuffer(
			GL_FRAMEBUFFER, (GL_COLOR_ATTACHMENT0 + attachment),
			GL_RENDERBUFFER, (uint32_t)(intptr_t)value));
	}

	void opengl_frame_buffer::set_texture2d(void const * value, uint32_t attachment, uint32_t level)
	{
		glCheck(glFramebufferTexture2D(
			GL_FRAMEBUFFER, (GL_COLOR_ATTACHMENT0 + attachment),
			GL_TEXTURE_2D, (uint32_t)(intptr_t)value, level));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_render_buffer::opengl_render_buffer()
		: m_handle{}
	{
		glCheck(glGenRenderbuffers(1, &m_handle));
	}

	opengl_render_buffer::~opengl_render_buffer()
	{
		glCheck(glDeleteRenderbuffers(1, &m_handle));
	}

	opengl_render_buffer::opengl_render_buffer(uint32_t format, vec2i const & size)
		: self_type{}
	{
		this->set_storage(format, size);
	}

	void opengl_render_buffer::bind(opengl_render_buffer const * value)
	{
		glCheck(glBindRenderbuffer(GL_RENDERBUFFER, value ? value->m_handle : NULL));
	}

	void opengl_render_buffer::set_storage(uint32_t format, vec2i const & size)
	{
		glCheck(glRenderbufferStorage(GL_RENDERBUFFER, _format<to_impl>(format), size[0], size[1]));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_shader::opengl_shader()
		: m_handle{}
	{
#ifdef GL_ARB_shader_objects
		glCheck(m_handle = glCreateProgramObjectARB());
#else
		glCheck(m_handle = glCreateProgram());
#endif
	}

	opengl_shader::~opengl_shader()
	{
#ifdef GL_ARB_shader_objects
		glCheck(glDeleteObjectARB(m_handle));
#else
		glCheck(glDeleteShader(m_handle));
#endif
	}

	void opengl_shader::bind(opengl_shader const * value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUseProgramObjectARB(value ? (uint32_t)(intptr_t)value->get_handle() : NULL));
#else
		glCheck(glUseProgram(value ? (uint32_t)(intptr_t)value->get_handle() : NULL));
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	opengl_texture::opengl_texture()
		: m_handle{}, m_type{}
	{
	}

	opengl_texture::~opengl_texture()
	{
	}

	void opengl_texture::bind(opengl_texture const * value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// opengl api
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool opengl_render_api::initialize()
	{
		static bool temp{};
		static ML_scope // once
		{
#if defined(ML_IMPL_OPENGL_LOADER_GLEW)
			glewExperimental = true;
			temp = (GLEW_OK == glewInit());

#elif defined(ML_IMPL_OPENGL_LOADER_GL3W)
			temp = gl3wInit();

#elif defined(ML_IMPL_OPENGL_LOADER_GLAD)
			temp = gladLoadGL();

#elif defined(ML_IMPL_OPENGL_LOADER_CUSTOM)
			temp = false;
#endif
		};
		return temp;
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
		return _cull_order<to_user>(temp);
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

	bool opengl_render_api::get_multisample_enabled() const
	{
		return glIsEnabled(GL_MULTISAMPLE);
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

	void opengl_render_api::set_cull_facet(uint32_t facet)
	{
		glCheck(glCullFace(_facet<to_impl>(facet)));
	}

	void opengl_render_api::set_cull_order(uint32_t order)
	{
		glCheck(glFrontFace(_cull_order<to_impl>(order)));
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

	void opengl_render_api::set_multisample_enabled(bool enabled)
	{
		glCheck((enabled ? &glEnable : &glDisable)(GL_MULTISAMPLE));
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

	void opengl_render_api::draw_arrays(uint32_t primitive, uint32_t first, uint32_t count)
	{
		glCheck(glDrawArrays(_primitive<to_impl>(primitive), first, count));
	}

	void opengl_render_api::draw_indexed(uint32_t primitive, int32_t first, uint32_t index_type, void const * indices)
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