#if defined(ML_IMPL_RENDERER_OPENGL)
#ifndef _ML_IMPL_RENDERER_OPENGL_HPP_
#define _ML_IMPL_RENDERER_OPENGL_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// api
namespace ml::gl
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

		api_info const & get_info() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool get_alpha_enabled() const override;

		alpha_fn get_alpha_fn() const override;
		
		bool get_blend_enabled() const override;

		color get_blend_color() const override;
		
		blend_eq get_blend_eq() const override;
		
		blend_fn get_blend_fn() const override;
		
		color get_clear_color() const override;

		bool get_cull_enabled() const override;

		uint32_t get_cull_facet() const override;

		uint32_t get_cull_order() const override;

		bool get_depth_enabled() const override;
		
		uint32_t get_depth_pr() const override;
		
		bool get_depth_mask() const override;

		depth_range get_depth_range() const override;

		bool get_stencil_enabled() const override;

		stencil_fn get_stencil_fn() const override;

		int_rect get_viewport() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_alpha_enabled(bool enabled) override;

		void set_alpha_fn(alpha_fn const & value) override;
		
		void set_blend_color(color const & value) override;

		void set_blend_enabled(bool enabled) override;
		
		void set_blend_eq(blend_eq const & value) override;
		
		void set_blend_fn(blend_fn const & value) override;
		
		void set_clear_color(color const & value) override;
		
		void set_cull_enabled(bool enabled) override;

		void set_cull_facet(uint32_t facet) override;

		void set_cull_order(uint32_t order) override;

		void set_depth_enabled(bool enabled) override;
		
		void set_depth_pr(uint32_t predicate) override;
		
		void set_depth_mask(bool enabled) override;

		void set_depth_range(depth_range const & value) override;

		void set_stencil_enabled(bool enabled) override;

		void set_stencil_fn(stencil_fn const & value) override;

		void set_viewport(int_rect const & bounds) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear(uint32_t flags) override;

		void draw(shared<vertexarray> const & value) override;
		
		void draw_arrays(uint32_t first, uint32_t count) override;
		
		void draw_indexed(uint32_t count) override;
		
		void flush() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void upload(handle loc, int32_t const & value) override;

		void upload(handle loc, float_t const & value) override;

		void upload(handle loc, vec2 const & value) override;

		void upload(handle loc, vec3 const & value) override;

		void upload(handle loc, vec4 const & value) override;

		void upload(handle loc, mat2 const & value) override;

		void upload(handle loc, mat3 const & value) override;

		void upload(handle loc, mat4 const & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gl
{
	// opengl vertexarray
	class opengl_vertexarray final : public vertexarray
	{
	public:
		opengl_vertexarray() noexcept
		{
			this->generate();
			this->bind();
		}

		~opengl_vertexarray() noexcept override { this->release(); }

		bool generate() override;

		bool release() override;

		void bind() const override;

		void unbind() const override;

		void add_vbo(shared<vertexbuffer> const & value) override;

		void set_ibo(shared<indexbuffer> const & value) override;

		inline handle get_handle() const noexcept override { return ML_addressof(m_handle); }

		inline shared<indexbuffer> const & get_ibo() const noexcept override { return m_indices; }

		inline pmr::vector<shared<vertexbuffer>> const & get_vbos() const noexcept override { return m_vertices; }

		inline bool has_value() const noexcept override { return (bool)m_handle; }

	protected:
		inline bool do_is_equal(graphics_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}

	private:
		uint32_t							m_handle	{}; // handle
		shared<indexbuffer>					m_indices	{}; // index buffer
		pmr::vector<shared<vertexbuffer>>	m_vertices	{}; // vertex buffers
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gl
{
	// opengl vertexbuffer
	class opengl_vertexbuffer final : public vertexbuffer
	{
	public:
		opengl_vertexbuffer(buffer verts, uint32_t count) noexcept : m_usage{ usage_static_draw }
		{
			this->generate();
			this->bind();
			this->set_data(verts, count);
		}

		opengl_vertexbuffer(uint32_t count) noexcept : m_usage{ usage_dynamic_draw }
		{
			this->generate();
			this->bind();
			this->set_data(nullptr, count);
		}

		~opengl_vertexbuffer() noexcept override { this->release(); }

		bool generate() override;

		bool release() override;

		void bind() const override;

		void unbind() const override;

		void set_data(buffer verts, uint32_t count) override;

		void set_data(buffer verts, uint32_t count, uint32_t offset) override;

		inline void set_layout(buffer_layout const & value) override { m_layout = value; }

		inline uint32_t get_count() const noexcept override { return m_count; }

		inline handle get_handle() const noexcept override { return ML_addressof(m_handle); }

		inline buffer_layout const & get_layout() const noexcept override { return m_layout; }

		inline bool has_value() const noexcept override { return (bool)m_handle; }

	protected:
		inline bool do_is_equal(graphics_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}

	private:
		uint32_t		m_handle	{}; // handle
		uint32_t		m_count		{}; // element count
		uint32_t const	m_usage		{}; // draw usage
		buffer_layout	m_layout	{}; // buffer layout
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gl
{
	// opengl indexbuffer
	class opengl_indexbuffer final : public indexbuffer
	{
	public:
		opengl_indexbuffer(buffer inds, uint32_t count) noexcept
		{
			this->generate();
			this->bind();
			this->set_data(inds, count);
		}

		~opengl_indexbuffer() noexcept override { this->release(); }

		bool generate() override;

		bool release() override;

		void bind() const override;

		void unbind() const override;

		void set_data(buffer inds, uint32_t count) override;

		inline uint32_t get_count() const noexcept override { return m_count; }

		inline handle get_handle() const noexcept override { return ML_addressof(m_handle); }

		inline bool has_value() const noexcept override { return (bool)m_handle; }

	protected:
		inline bool do_is_equal(graphics_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}

	private:
		uint32_t	m_handle	{}; // handle
		uint32_t	m_count		{}; // element count
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gl
{
	// opengl framebuffer
	class opengl_framebuffer final : public framebuffer
	{
	public:
		opengl_framebuffer::opengl_framebuffer(uint32_t format, vec2i const & size) noexcept
			: m_format{ format }
		{
			this->resize(size);
		}

		~opengl_framebuffer() noexcept override { this->release(); }

		bool generate() override;

		bool release() override;

		void bind() const override;

		void unbind() const override;

		void bind_texture(uint32_t slot = 0) const override;

		void resize(vec2i const & value) override;

		inline shared<texture2d> const & get_color_attachment() const noexcept override { return m_color; }

		inline shared<texture2d> const & get_depth_attachment() const noexcept override { return m_depth; }

		inline uint32_t get_format() const noexcept override { return m_format; }

		inline handle get_handle() const noexcept override { return ML_addressof(m_handle); }

		inline vec2i get_size() const noexcept override { return m_size; }

		inline bool has_value() const noexcept override { return (bool)m_handle; }

	protected:
		inline bool do_is_equal(graphics_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}

	private:
		uint32_t			m_handle	{}	; // handle
		uint32_t const		m_format	{}	; // format
		vec2i				m_size		{}	; // size
		shared<texture2d>	m_color		{}	; // color attachment
		shared<texture2d>	m_depth		{}	; // depth attachment
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gl
{
	// opengl texture2d
	class opengl_texture2d final : public texture2d
	{
	public:
		opengl_texture2d(vec2i const & size, uint32_t iformat, uint32_t cformat, uint32_t ptype, int32_t flags, buffer pixels) noexcept
			: m_i_format{ iformat }, m_c_format{ cformat }, m_p_type{ ptype }, m_flags{ flags }
		{
			this->update(size, pixels);
		}

		~opengl_texture2d() noexcept override { this->release(); }

		bool generate() override;

		bool release() override;

		void bind() const override;

		void unbind() const override;

		void update(vec2i const & size, buffer pixels = nullptr) override;

		void set_mipmapped(bool value) override;

		void set_repeated(bool value) override;

		void set_smooth(bool value) override;

		image copy_to_image() const override;

		inline int32_t get_flags() const noexcept override { return m_flags; }

		inline handle get_handle() const noexcept override { return ML_addressof(m_handle); }

		inline vec2i const & get_size() const noexcept override { return m_size; }

		inline bool has_value() const noexcept override { return (bool)m_handle; }

	protected:
		inline bool do_is_equal(graphics_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}

	private:
		uint32_t		m_handle	{}; // handle
		vec2i			m_size		{}; // size
		uint32_t const	m_i_format	{}; // internal format
		uint32_t const	m_c_format	{}; // color format
		uint32_t const	m_p_type	{}; // pixel type
		int32_t			m_flags		{}; // flags ( repeated / smooth / mipmapped )
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gl
{
	// opengl shader
	class opengl_shader final : public shader
	{
	public:
		opengl_shader() noexcept { this->generate(); }

		~opengl_shader() noexcept override { this->release(); }

		bool generate() override;

		bool release() override;

		void bind() const override;

		void unbind() const override;

		int32_t attach(uint32_t type, uint32_t count, cstring const * src) override;

		int32_t link() override;

		void bind_textures() const override;

		bool bind_uniform(cstring name, std::function<void(handle)> const & callback) override;

		inline bool has_value() const noexcept override { return (bool)m_handle; }

		inline handle get_handle() const noexcept override { return ML_addressof(m_handle); }

	protected:
		inline bool do_is_equal(graphics_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}

		inline void do_cache_texture(handle loc, handle value) noexcept override
		{
			static auto const max_texture_slots
			{
				(size_t)get_api()->get_info().max_texture_slots
			};
			if (auto const it{ m_textures.find(loc) })
			{
				(*it->second) = value;
			}
			else if ((m_textures.size() + 1) < max_texture_slots)
			{
				m_textures.insert(loc, value);
			}
		}

	private:
		uint32_t					m_handle	{}; // handle
		ds::map<hash_t, int32_t>	m_uniforms	{}; // uniform cache
		ds::map<handle, handle>		m_textures	{}; // texture cache

		struct uniform_binder; // uniform binder
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMPL_RENDERER_OPENGL_HPP_
#endif // ML_IMPL_RENDERER_OPENGL