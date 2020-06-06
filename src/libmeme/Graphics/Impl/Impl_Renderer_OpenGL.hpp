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
	protected:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class device;

		friend class std::unique_ptr<device, default_delete>;

		~opengl_device() noexcept override = default;

		bool do_initialize() override;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t get_error() const noexcept override;

		device_info const & get_info() const noexcept override;

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

		void upload(handle_t loc, bool const & value) override;

		void upload(handle_t loc, int32_t const & value) override;

		void upload(handle_t loc, float_t const & value) override;

		void upload(handle_t loc, vec2 const & value) override;

		void upload(handle_t loc, vec3 const & value) override;

		void upload(handle_t loc, vec4 const & value) override;

		void upload(handle_t loc, mat2 const & value) override;

		void upload(handle_t loc, mat3 const & value) override;

		void upload(handle_t loc, mat4 const & value) override;
		
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
		static constexpr typeof<> s_type_info{ typeof_v<opengl_vertexarray> };

		uint32_t							m_handle	{}; // object handle
		shared<indexbuffer>					m_indices	{}; // index buffer
		pmr::vector<shared<vertexbuffer>>	m_vertices	{}; // vertex buffers

	public:
		opengl_vertexarray() noexcept;

		~opengl_vertexarray() noexcept override { destroy(); }

		bool generate() override;

		bool destroy() override;

		void bind() const override;

		void unbind() const override;

		void add_vertices(shared<vertexbuffer> const & value) override;

		void set_indices(shared<indexbuffer> const & value) override;

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		shared<indexbuffer> const & get_indices() const noexcept override { return m_indices; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

		pmr::vector<shared<vertexbuffer>> const & get_vertices() const noexcept override { return m_vertices; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gl
{
	// opengl vertexbuffer
	class opengl_vertexbuffer final : public vertexbuffer
	{
		static constexpr typeof<> s_type_info{ typeof_v<opengl_vertexbuffer> };

		uint32_t		m_handle	{}; // object handle
		size_t			m_count		{}; // element count
		uint32_t const	m_usage		{}; // draw usage
		layout			m_layout	{}; // buffer layout
		buffer_t		m_buffer	{}; // local data

	public:
		opengl_vertexbuffer(uint32_t usage, address_t data, size_t count) noexcept;

		opengl_vertexbuffer(address_t data, size_t count) noexcept
			: opengl_vertexbuffer{ usage_static_draw, data, count }
		{
		}

		opengl_vertexbuffer(size_t count) noexcept
			: opengl_vertexbuffer{ usage_dynamic_draw, nullptr, count }
		{
		}

		~opengl_vertexbuffer() noexcept override { destroy(); }

		bool generate() override;

		bool destroy() override;

		void bind() const override;

		void unbind() const override;

		void set_data(address_t data, size_t count, size_t offset = 0) override;

		void set_layout(layout const & value) override { m_layout = value; }

		buffer_t const & get_buffer() const noexcept override { return m_buffer; }

		size_t get_count() const noexcept override { return m_count; }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		layout const & get_layout() const noexcept override { return m_layout; }

		uint32_t get_usage() const noexcept override { return m_usage; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gl
{
	// opengl indexbuffer
	class opengl_indexbuffer final : public indexbuffer
	{
		static constexpr typeof<> s_type_info{ typeof_v<opengl_indexbuffer> };

		uint32_t		m_handle	{}; // object handle
		size_t			m_count		{}; // element count
		uint32_t const	m_usage		{}; // usage
		buffer_t		m_buffer	{}; // local data

	public:
		opengl_indexbuffer(address_t data, size_t count) noexcept;

		~opengl_indexbuffer() noexcept override { destroy(); }

		bool generate() override;

		bool destroy() override;

		void bind() const override;

		void unbind() const override;

		void set_data(address_t data, size_t count, size_t offset = 0) override;

		buffer_t const & get_buffer() const noexcept override { return m_buffer; }

		size_t get_count() const noexcept override { return m_count; }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

		uint32_t get_usage() const noexcept override { return m_usage; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gl
{
	// opengl framebuffer
	class opengl_framebuffer final : public framebuffer
	{
		static constexpr typeof<> s_type_info{ typeof_v<opengl_framebuffer> };

		uint32_t			m_handle	{}	; // object handle
		uint32_t const		m_format	{}	; // format
		vec2i				m_size		{}	; // size
		shared<texture2d>	m_color		{}	; // color attachment
		shared<texture2d>	m_depth		{}	; // depth attachment

	public:
		opengl_framebuffer(vec2i const & size, uint32_t format) noexcept
			: m_format{ format }
		{
			resize(size);
		}

		~opengl_framebuffer() noexcept override { destroy(); }

		bool generate() override;

		bool destroy() override;

		void bind() const override;

		void unbind() const override;

		void bind_texture(uint32_t slot = 0) const override;

		void resize(vec2i const & value) override;

		shared<texture2d> const & get_color_attachment() const noexcept override { return m_color; }

		shared<texture2d> const & get_depth_attachment() const noexcept override { return m_depth; }

		uint32_t get_format() const noexcept override { return m_format; }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		vec2i const & get_size() const noexcept override { return m_size; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gl
{
	// opengl texture2d
	class opengl_texture2d final : public texture2d
	{
		static constexpr typeof<> s_type_info{ typeof_v<opengl_texture2d> };

		uint32_t		m_handle	{}; // object handle
		vec2i			m_size		{}; // size
		uint32_t const	m_i_format	{}; // internal format
		uint32_t const	m_c_format	{}; // color format
		uint32_t const	m_p_type	{}; // pixel type
		int32_t			m_flags		{}; // flags ( repeated / smooth / mipmapped )

	public:
		opengl_texture2d(vec2i const & size, uint32_t iformat, uint32_t cformat, uint32_t ptype, int32_t flags, address_t data) noexcept
			: m_i_format{ iformat }, m_c_format{ cformat }, m_p_type{ ptype }, m_flags{ flags }
		{
			update(size, data);
		}

		~opengl_texture2d() noexcept override { destroy(); }

		bool generate() override;

		bool destroy() override;

		void bind() const override;

		void unbind() const override;

		void set_mipmapped(bool value) override;

		void set_repeated(bool value) override;

		void set_smooth(bool value) override;

		void update(vec2i const & size, address_t data = nullptr) override;

		image copy_to_image() const override;

		int32_t get_flags() const noexcept override { return m_flags; }

		uint32_t get_color_format() const noexcept override { return m_c_format; }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		uint32_t get_internal_format() const noexcept override { return m_i_format; }

		uint32_t get_pixel_type() const noexcept override { return m_p_type; }

		vec2i const & get_size() const noexcept override { return m_size; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gl
{
	// opengl shader
	class opengl_shader final : public shader
	{
		static constexpr typeof<> s_type_info{ typeof_v<opengl_shader> };

		uint32_t		m_handle		{}; // object handle
		uint32_t const	m_shader_type	{}; // shader type
		int32_t			m_status		{}; // status
		pmr::string		m_error_log		{}; // error log

	public:
		opengl_shader(uint32_t type) noexcept : m_shader_type{ type } { generate(); }

		~opengl_shader() noexcept override { destroy(); }

		bool generate() override;

		bool destroy() override;

		void bind() const override;

		void unbind() const override;

		int32_t compile(size_t count, cstring const * src) override;

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		pmr::string const & get_error_log() const noexcept override { return m_error_log; }

		uint32_t get_shader_type() const noexcept override { return m_shader_type; }

		int32_t get_status() const noexcept override { return m_status; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gl
{
	// opengl program
	class opengl_program final : public program
	{
		static constexpr typeof<> s_type_info{ typeof_v<opengl_program> };

		uint32_t							m_handle		{}; // object handle
		ds::set<shared<shader>>				m_shaders		{}; // shader objects
		ds::map<handle_t, shared<texture>>	m_textures		{}; // texture cache
		ds::map<hash_t, int32_t>			m_uniforms		{}; // uniform cache
		int32_t								m_status		{}; // status
		pmr::string							m_error_log		{}; // error log

		struct opengl_uniform_binder; // uniform binder

	public:
		opengl_program() noexcept { generate(); }

		~opengl_program() noexcept override { destroy(); }

		bool generate() override;

		bool destroy() override;

		void bind() const override;

		void unbind() const override;

		bool attach(shared<shader> const & value) override;

		int32_t link() override;

		void bind_textures() const override;

		bool bind_uniform(cstring name, std::function<void(handle_t)> const & callback) override;

		pmr::string const & get_error_log() const noexcept { return m_error_log; }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		ds::set<shared<shader>> const & get_shaders() const noexcept override { return m_shaders; }

		int32_t get_status() const noexcept override { return m_status; }

		ds::map<handle_t, shared<texture>> const & get_textures() const noexcept override { return m_textures; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	protected:
		bool do_is_equal(device_object const & other) const noexcept override
		{
			return this == std::addressof(other);
		}

		void do_cache_texture(handle_t loc, shared<texture> const & value) noexcept override
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
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMPL_RENDERER_OPENGL_HPP_
#endif // ML_IMPL_RENDERER_OPENGL