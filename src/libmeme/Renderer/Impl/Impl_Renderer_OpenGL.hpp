#if defined(ML_IMPL_RENDERER_OPENGL)
#ifndef _ML_IMPL_RENDERER_OPENGL_HPP_
#define _ML_IMPL_RENDERER_OPENGL_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Renderer/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// opengl objects
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl vertex array
	class opengl_vertex_array final : public vertex_array
	{
	public:
		opengl_vertex_array();

		~opengl_vertex_array();

		void bind() const override;

		void unbind() const override;

		void add_vbo(shared<vertex_buffer> const & value) override;

		void set_ibo(shared<index_buffer> const & value) override;

		inline handle_t get_handle() const override { return ML_addressof(m_handle); }

		inline shared<index_buffer> const & get_ibo() const override { return m_indices; }

		inline pmr::vector<shared<vertex_buffer>> const & get_vbos() const override { return m_vertices; }

	private:
		uint32_t m_handle{}; // handle
		
		shared<index_buffer> m_indices{}; // index buffer
		
		pmr::vector<shared<vertex_buffer>> m_vertices{}; // vertex buffers
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl vertex buffer
	class opengl_vertex_buffer final : public vertex_buffer
	{
	public:
		opengl_vertex_buffer(buffer_t vertices, uint32_t size, uint32_t usage = usage_static);

		opengl_vertex_buffer(uint32_t size, uint32_t usage = usage_dynamic);

		~opengl_vertex_buffer();

		void bind() const override;

		void unbind() const override;

		void set_data(buffer_t vertices, uint32_t size, uint32_t offset = 0) override;

		inline void set_layout(buffer_layout const & value) override { m_layout = value; }

		inline handle_t get_handle() const override { return ML_addressof(m_handle); }

		inline buffer_layout const & get_layout() const override { return m_layout; }

		inline uint32_t get_size() const override { return m_size; }

	private:
		uint32_t		m_handle	{}; // handle
		uint32_t		m_size		{}; // data size in bytes
		uint32_t		m_usage		{}; // draw usage
		buffer_layout	m_layout	{}; // buffer layout
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl index buffer
	class opengl_index_buffer final : public index_buffer
	{
	public:
		opengl_index_buffer(buffer_t indices, uint32_t count);

		~opengl_index_buffer();

		void bind() const override;

		void unbind() const override;

		void set_data(buffer_t indices, uint32_t count) override;

		inline uint32_t get_count() const override { return m_count; }

		inline handle_t get_handle() const override { return ML_addressof(m_handle); }

	private:
		uint32_t	m_handle	{}; // handle
		uint32_t	m_count		{}; // index count
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl frame buffer
	class opengl_frame_buffer final : public frame_buffer
	{
	public:
		opengl_frame_buffer(uint32_t format, vec2i const & size);

		~opengl_frame_buffer();

		void bind() const override;

		void unbind() const override;

		void bind_texture(uint32_t slot = 0) const override;

		void resize(vec2i const & value) override;

		inline handle_t get_color_attachment() const override { return ML_addressof(m_color_attachment); }

		inline handle_t get_depth_attachment() const override { return ML_addressof(m_depth_attachment); }

		inline uint32_t get_format() const override { return m_format; }

		inline handle_t get_handle() const override { return ML_addressof(m_handle); }

		inline vec2i get_size() const override { return m_size; }

	private:
		uint32_t	m_handle			{}; // handle
		uint32_t	m_format			{}; // format
		vec2i		m_size				{}; // size
		uint32_t	m_color_attachment	{}; // color attachment
		uint32_t	m_depth_attachment	{}; // depth attachment
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// opengl shader
	class opengl_shader_object final : public shader_object
	{
	public:
		opengl_shader_object(cstring v_src, cstring f_src);

		opengl_shader_object(cstring v_src, cstring g_src, cstring f_src);

		~opengl_shader_object();

		void bind(bool bind_textures = true) const override;

		void unbind() const override;

		inline handle_t get_handle() const override { return ML_addressof(m_handle); }

		bool set_uniform(cstring name, int32_t value) override;

		bool set_uniform(cstring name, float32_t value) override;

		bool set_uniform(cstring name, vec2 const & value) override;

		bool set_uniform(cstring name, vec3 const & value) override;

		bool set_uniform(cstring name, vec4 const & value) override;

		bool set_uniform(cstring name, mat2 const & value) override;

		bool set_uniform(cstring name, mat3 const & value) override;

		bool set_uniform(cstring name, mat4 const & value) override;

		bool set_uniform(cstring name, handle_t value) override;

	private:
		uint32_t					m_handle	{}; // handle
		ds::map<hash_t, int32_t>	m_uniforms	{}; // uniform cache
		ds::map<int32_t, handle_t>	m_textures	{}; // texture cache
		
		struct uniform_binder;

		void compile(cstring v_src, cstring g_src, cstring f_src);
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

		inline handle_t get_handle() const override { return ML_addressof(m_handle); }

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
	protected:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class render_api;

		~opengl_render_api() noexcept = default;

		bool do_initialize() override;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		api_capabilities const & get_capabilities() const override;

		uint32_t get_error() const override;

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

		void set_viewport(int_rect const & bounds) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear(uint32_t flags) override;

		void draw(shared<vertex_array> const & value) override;
		
		void draw_arrays(uint32_t first, uint32_t count) override;
		
		void draw_indexed(uint32_t count) override;
		
		void flush() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMPL_RENDERER_OPENGL_HPP_
#endif // ML_IMPL_RENDERER_OPENGL