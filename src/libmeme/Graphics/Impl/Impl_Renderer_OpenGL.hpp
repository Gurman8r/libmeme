#if defined(ML_IMPL_RENDERER_OPENGL)
#ifndef _ML_IMPL_RENDERER_OPENGL_HPP_
#define _ML_IMPL_RENDERER_OPENGL_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl objects
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl vertex array
	class opengl_vertexarray final : public vertexarray
	{
	public:
		opengl_vertexarray();

		~opengl_vertexarray();

		void bind() const override;

		void unbind() const override;

		void add_vbo(shared<vertexbuffer> const & value) override;

		void set_ibo(shared<indexbuffer> const & value) override;

		inline gl::handle get_handle() const override { return ML_addressof(m_handle); }

		inline shared<indexbuffer> const & get_ibo() const override { return m_indices; }

		inline pmr::vector<shared<vertexbuffer>> const & get_vbos() const override { return m_vertices; }

	private:
		uint32_t							m_handle	{}; // handle
		shared<indexbuffer>				m_indices	{}; // index buffer
		pmr::vector<shared<vertexbuffer>>	m_vertices	{}; // vertex buffers
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl vertex buffer
	class opengl_vertexbuffer final : public vertexbuffer
	{
	public:
		opengl_vertexbuffer(gl::buffer vertices, uint32_t count, uint32_t usage = gl::usage_static_draw);

		opengl_vertexbuffer(uint32_t count, uint32_t usage = gl::usage_dynamic_draw);

		~opengl_vertexbuffer();

		void bind() const override;

		void unbind() const override;

		void set_data(gl::buffer vertices, uint32_t count, uint32_t offset = 0) override;

		inline void set_layout(buffer_layout const & value) override { m_layout = value; }

		inline uint32_t get_count() const override { return m_count; }

		inline gl::handle get_handle() const override { return ML_addressof(m_handle); }

		inline buffer_layout const & get_layout() const override { return m_layout; }

	private:
		uint32_t		m_handle	{}; // handle
		uint32_t		m_count		{}; // element count
		uint32_t		m_usage		{}; // draw usage
		buffer_layout	m_layout	{}; // buffer layout
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl index buffer
	class opengl_indexbuffer final : public indexbuffer
	{
	public:
		opengl_indexbuffer(gl::buffer indices, uint32_t count);

		~opengl_indexbuffer();

		void bind() const override;

		void unbind() const override;

		void set_data(gl::buffer indices, uint32_t count) override;

		inline uint32_t get_count() const override { return m_count; }

		inline gl::handle get_handle() const override { return ML_addressof(m_handle); }

	private:
		uint32_t	m_handle	{}; // handle
		uint32_t	m_count		{}; // element count
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl frame buffer
	class opengl_framebuffer final : public framebuffer
	{
	public:
		opengl_framebuffer(uint32_t format, vec2i const & size);

		~opengl_framebuffer();

		void bind() const override;

		void unbind() const override;

		void bind_texture(uint32_t slot = 0) const override;

		void resize(vec2i const & value) override;

		inline shared<texture2d> const & get_color_attachment() const override { return m_color; }

		inline shared<texture2d> const & get_depth_attachment() const override { return m_depth; }

		inline uint32_t get_format() const override { return m_format; }

		inline gl::handle get_handle() const override { return ML_addressof(m_handle); }

		inline vec2i get_size() const override { return m_size; }

	private:
		uint32_t			m_handle	{}	; // handle
		uint32_t			m_format	{}	; // format
		vec2i				m_size		{}	; // size
		shared<texture2d>	m_color		{}	; // color attachment
		shared<texture2d>	m_depth		{}	; // depth attachment
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl texture
	class opengl_texture2d final : public texture2d
	{
	public:
		opengl_texture2d(vec2i const & size, uint32_t iformat, uint32_t cformat, uint32_t ptype, int32_t flags, gl::buffer pixels);

		~opengl_texture2d();

		void bind() const override;

		void unbind() const override;

		void update(vec2i const & size, gl::buffer pixels = nullptr) override;

		void set_mipmapped(bool value) override;

		void set_repeated(bool value) override;

		void set_smooth(bool value) override;

		image copy_to_image() const override;

		inline int32_t get_flags() const override { return m_flags; }

		inline gl::handle get_handle() const override { return ML_addressof(m_handle); }

		inline vec2i const & get_size() const override { return m_size; }

	private:
		uint32_t	m_handle	{}; // handle
		vec2i		m_size		{}; // size
		uint32_t	m_i_format	{}; // internal format
		uint32_t	m_c_format	{}; // color format
		uint32_t	m_p_type	{}; // pixel type
		int32_t		m_flags		{}; // flags ( repeated / smooth / mipmapped )
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl shader
	class opengl_shader_object final : public shader_object
	{
	public:
		opengl_shader_object(cstring v_src, cstring f_src, cstring g_src = nullptr);

		~opengl_shader_object();

		void bind(bool bind_textures = true) const override;

		void unbind() const override;

		bool set_uniform(cstring name, bool value) override;

		bool set_uniform(cstring name, int32_t value) override;

		bool set_uniform(cstring name, float_t value) override;

		bool set_uniform(cstring name, vec2 const & value) override;

		bool set_uniform(cstring name, vec3 const & value) override;

		bool set_uniform(cstring name, vec4 const & value) override;

		bool set_uniform(cstring name, mat2 const & value) override;

		bool set_uniform(cstring name, mat3 const & value) override;

		bool set_uniform(cstring name, mat4 const & value) override;

		bool set_uniform(cstring name, shared<texture2d> const & value) override;

		inline gl::handle get_handle() const override { return ML_addressof(m_handle); }

	private:
		uint32_t						m_handle	{}; // handle
		ds::map<hash_t, int32_t>		m_uniforms	{}; // uniform cache
		ds::map<int32_t, gl::handle>	m_textures	{}; // texture cache
		
		struct uniform_binder; // uniform binder
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl api
namespace ml
{
	class opengl_render_api final : public render_api
	{
	protected:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class render_api;

		~opengl_render_api() noexcept = default;

		bool do_initialize() override;

		void on_initialize() override;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t get_error() const override;

		render_api_info const & get_info() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool get_alpha_enabled() const override;

		alpha_function get_alpha_function() const override;
		
		bool get_blend_enabled() const override;

		color get_blend_color() const override;
		
		blend_equation get_blend_equation() const override;
		
		blend_function get_blend_function() const override;
		
		color get_clear_color() const override;

		bool get_cull_enabled() const override;

		uint32_t get_cull_facet() const override;

		uint32_t get_cull_order() const override;

		bool get_depth_enabled() const override;
		
		uint32_t get_depth_function() const override;
		
		bool get_depth_mask() const override;

		depth_range get_depth_range() const override;

		bool get_stencil_enabled() const override;

		stencil_function get_stencil_function() const override;

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

		void set_cull_facet(uint32_t facet) override;

		void set_cull_order(uint32_t order) override;

		void set_depth_enabled(bool enabled) override;
		
		void set_depth_function(uint32_t predicate) override;
		
		void set_depth_mask(bool enabled) override;

		void set_depth_range(depth_range const & value) override;

		void set_stencil_enabled(bool enabled) override;

		void set_stencil_function(stencil_function const & value) override;

		void set_viewport(int_rect const & bounds) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear(uint32_t flags) override;

		void draw(shared<vertexarray> const & value) override;
		
		void draw_arrays(uint32_t first, uint32_t count) override;
		
		void draw_indexed(uint32_t count) override;
		
		void flush() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMPL_RENDERER_OPENGL_HPP_
#endif // ML_IMPL_RENDERER_OPENGL