#if defined(ML_IMPL_RENDERER_OPENGL)
#ifndef _ML_IMPL_RENDERER_OPENGL_HPP_
#define _ML_IMPL_RENDERER_OPENGL_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device
namespace ml::gl
{
	// opengl device
	class opengl_device final : public device
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t get_error() const noexcept override;

		device_info const & get_info() const override;

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
		
		void draw_arrays(size_t first, size_t count) override;
		
		void draw_indexed(size_t count) override;
		
		void flush() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void upload(handle_t loc, bool const & value) override
		{
			return upload(loc, static_cast<int32_t>(value));
		}

		void upload(handle_t loc, int32_t const & value) override;

		void upload(handle_t loc, float_t const & value) override;

		void upload(handle_t loc, vec2 const & value) override;

		void upload(handle_t loc, vec3 const & value) override;

		void upload(handle_t loc, vec4 const & value) override;

		void upload(handle_t loc, mat2 const & value) override;

		void upload(handle_t loc, mat3 const & value) override;

		void upload(handle_t loc, mat4 const & value) override;

	protected:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class context;

		friend class device;

		~opengl_device() noexcept override = default;

		bool do_initialize() override;

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
		opengl_vertexarray() noexcept { generate().bind(); }

		~opengl_vertexarray() noexcept override { destroy(); }

		vertexarray const & bind() const override;

		vertexarray const & unbind() const override;

		vertexarray & generate() override;

		vertexarray & destroy() override;

		vertexarray & add_vb(shared<vertexbuffer> const & value) override;

		vertexarray & set_ib(shared<indexbuffer> const & value) override;

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		shared<indexbuffer> const & get_ib() const noexcept override { return m_indices; }

		typeof<> get_typeof() const noexcept override { return typeof_v<opengl_vertexarray>; }

		pmr::vector<shared<vertexbuffer>> const & get_vbs() const noexcept override { return m_vertices; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}

	private:
		uint32_t							m_handle	{}; // object handle
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
		opengl_vertexbuffer(buffer_t data, size_t count) noexcept : m_usage{ usage_static_draw }
		{
			generate().bind();
			set_data(data, count);
		}

		opengl_vertexbuffer(size_t count) noexcept : m_usage{ usage_dynamic_draw }
		{
			generate().bind();
			set_data(nullptr, count);
		}

		~opengl_vertexbuffer() noexcept override { destroy(); }

		vertexbuffer const & bind() const override;

		vertexbuffer const & unbind() const override;

		vertexbuffer & generate() override;

		vertexbuffer & destroy() override;

		vertexbuffer & set_data(buffer_t data, size_t count) override;

		vertexbuffer & set_data(buffer_t data, size_t count, size_t offset) override;

		vertexbuffer & set_layout(layout const & value) override
		{
			m_layout = value;
			return (*this);
		}

		size_t get_count() const noexcept override { return m_count; }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		layout const & get_layout() const noexcept override { return m_layout; }

		uint32_t get_usage() const noexcept override { return m_usage; }

		typeof<> get_typeof() const noexcept override { return typeof_v<opengl_vertexbuffer>; }

		view_t const & get_view() const noexcept override { return m_view; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}

	private:
		uint32_t		m_handle	{}; // object handle
		size_t			m_count		{}; // element count
		uint32_t const	m_usage		{}; // draw usage
		layout			m_layout	{}; // buffer layout
		view_t			m_view		{}; // local data
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
		opengl_indexbuffer(buffer_t data, size_t count) noexcept : m_usage{ usage_static_draw }
		{
			generate().bind();
			set_data(data, count);
		}

		~opengl_indexbuffer() noexcept override { destroy(); }

		indexbuffer const & bind() const override;

		indexbuffer const & unbind() const override;

		indexbuffer & generate() override;

		indexbuffer & destroy() override;

		indexbuffer & set_data(buffer_t data, size_t count) override;

		size_t get_count() const noexcept override { return m_count; }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		typeof<> get_typeof() const noexcept override { return typeof_v<opengl_indexbuffer>; }

		uint32_t get_usage() const noexcept override { return m_usage; }

		view_t const & get_view() const noexcept override { return m_view; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}

	private:
		uint32_t		m_handle	{}; // object handle
		size_t			m_count		{}; // element count
		uint32_t const	m_usage		{}; // usage
		view_t			m_view		{}; // local data
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
		opengl_framebuffer::opengl_framebuffer(vec2i const & size, uint32_t format) noexcept
			: m_format{ format }
		{
			resize(size);
		}

		~opengl_framebuffer() noexcept override { destroy(); }

		framebuffer const & bind() const override;

		framebuffer const & unbind() const override;

		framebuffer & generate() override;

		framebuffer & destroy() override;

		framebuffer const & bind_texture(uint32_t slot = 0) const & override;

		framebuffer & resize(vec2i const & value) & override;

		shared<texture2d> const & get_color_attachment() const noexcept override { return m_color; }

		shared<texture2d> const & get_depth_attachment() const noexcept override { return m_depth; }

		uint32_t get_format() const noexcept override { return m_format; }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		vec2i const & get_size() const noexcept override { return m_size; }

		typeof<> get_typeof() const noexcept override { return typeof_v<opengl_framebuffer>; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}

	private:
		uint32_t			m_handle	{}	; // object handle
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
		opengl_texture2d(vec2i const & size, uint32_t iformat, uint32_t cformat, uint32_t ptype, int32_t flags, buffer_t data) noexcept
			: m_i_format{ iformat }, m_c_format{ cformat }, m_p_type{ ptype }, m_flags{ flags }
		{
			update(size, data);
		}

		~opengl_texture2d() noexcept override { destroy(); }

		texture2d const & bind() const override;

		texture2d const & unbind() const override;

		texture2d & generate() override;

		texture2d & destroy() override;

		texture2d & set_mipmapped(bool value) & override;

		texture2d & set_repeated(bool value) & override;

		texture2d & set_smooth(bool value) & override;

		texture2d & update(vec2i const & size, buffer_t data = nullptr) & override;

		image copy_to_image() const override;

		int32_t get_flags() const noexcept override { return m_flags; }

		uint32_t get_color_format() const noexcept override { return m_c_format; }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		uint32_t get_internal_format() const noexcept override { return m_i_format; }

		uint32_t get_pixel_type() const noexcept override { return m_p_type; }

		vec2i const & get_size() const noexcept override { return m_size; }

		typeof<> get_typeof() const noexcept override { return typeof_v<opengl_texture2d>; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}

	private:
		uint32_t		m_handle	{}; // object handle
		vec2i			m_size		{}; // size
		uint32_t const	m_i_format	{}; // internal format
		uint32_t const	m_c_format	{}; // color format
		uint32_t const	m_p_type	{}; // pixel type
		int32_t			m_flags		{}; // flags ( repeated / smooth / mipmapped )
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gl
{
	// opengl program
	class opengl_program final : public program
	{
	public:
		opengl_program(uint32_t type) noexcept : m_type{ type } { generate(); }

		~opengl_program() noexcept override { destroy(); }

		program & generate() override;

		program & destroy() override;

		int32_t compile(size_t count, cstring const * src) override;

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		pmr::vector<char> const & get_log() const noexcept override { return m_log; }

		int32_t get_status() const noexcept override { return m_status; }

		uint32_t get_shader_type() const noexcept override { return m_type; }

		typeof<> get_typeof() const noexcept override { return typeof_v<opengl_program>; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}

	private:
		uint32_t			m_handle{};
		uint32_t const		m_type	{};
		int32_t				m_status{};
		pmr::vector<char>	m_log	{};
	};
}

// shader
namespace ml::gl
{
	// opengl shader
	class opengl_shader final : public shader
	{
	public:
		opengl_shader() noexcept { generate(); }

		~opengl_shader() noexcept override { destroy(); }

		shader const & bind() const override;

		shader const & unbind() const override;

		shader & generate() override;

		shader & destroy() override;

		int32_t attach(shared<program> const & value) override;

		int32_t link() override;

		shader const & bind_textures() const override;

		bool bind_uniform(cstring name, std::function<void(handle_t)> const & callback) override;

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		typeof<> get_typeof() const noexcept override { return typeof_v<opengl_shader>; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}

		void do_cache_texture(handle_t loc, handle_t value) noexcept override
		{
			static auto const max_texture_slots
			{
				(size_t)device::get_context()->get_info().max_texture_slots
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
		uint32_t					m_handle	{}; // object handle
		ds::map<hash_t, int32_t>	m_uniforms	{}; // uniform cache
		ds::map<handle_t, handle_t>	m_textures	{}; // texture cache

		struct uniform_binder; // uniform binder
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMPL_RENDERER_OPENGL_HPP_
#endif // ML_IMPL_RENDERER_OPENGL