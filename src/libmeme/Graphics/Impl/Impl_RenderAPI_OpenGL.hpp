#if defined(ML_IMPL_RENDERER_OPENGL)
#ifndef _ML_IMPL_RENDERER_OPENGL_HPP_
#define _ML_IMPL_RENDERER_OPENGL_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device
namespace ml::gfx
{
	// opengl device
	class opengl_device final : public device
	{
	protected:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class device;

		friend class std::unique_ptr<device, default_delete<>>;

		~opengl_device() override = default;

		bool do_initialize(context_settings const & settings) override;

		void on_initialize() override;

		devinfo m_devinfo;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		devinfo const & get_devinfo() const noexcept override { return m_devinfo; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool get_alpha_enabled() const override;

		alpha_mode get_alpha_mode() const override;
		
		bool get_blend_enabled() const override;

		color get_blend_color() const override;
		
		blend_mode get_blend_mode() const override;
		
		color get_clear_color() const override;

		bool get_cull_enabled() const override;

		cull_mode get_cull_mode() const override;

		bool get_depth_enabled() const override;
		
		depth_mode get_depth_mode() const override;

		bool get_depth_write() const override;

		bool get_stencil_enabled() const override;

		stencil_mode get_stencil_mode() const override;

		int_rect get_viewport() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_alpha_enabled(bool enabled) override;

		void set_alpha_mode(alpha_mode const & value) override;
		
		void set_blend_color(color const & value) override;

		void set_blend_enabled(bool enabled) override;
		
		void set_blend_mode(blend_mode const & value) override;
		
		void set_clear_color(color const & value) override;
		
		void set_cull_enabled(bool enabled) override;

		void set_cull_mode(cull_mode const & value) override;

		void set_depth_enabled(bool enabled) override;
		
		void set_depth_mode(depth_mode const & value) override;

		void set_depth_write(bool enabled) override;

		void set_stencil_enabled(bool enabled) override;

		void set_stencil_mode(stencil_mode const & value) override;

		void set_viewport(int_rect const & bounds) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear(uint32_t flags) override;

		void draw(shared<vertexarray> const & value) override;
		
		void draw_arrays(uint32_t mode, size_t first, size_t count) override;
		
		void draw_indexed(uint32_t mode, size_t count) override;
		
		void flush() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void upload(uniform_id loc, bool value) override;

		void upload(uniform_id loc, int32_t value) override;

		void upload(uniform_id loc, float_t value) override;

		void upload(uniform_id loc, vec2 const & value) override;

		void upload(uniform_id loc, vec3 const & value) override;

		void upload(uniform_id loc, vec4 const & value) override;

		void upload(uniform_id loc, mat2 const & value) override;

		void upload(uniform_id loc, mat3 const & value) override;

		void upload(uniform_id loc, mat4 const & value) override;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gfx
{
	// opengl vertexbuffer
	class opengl_vertexbuffer final : public vertexbuffer
	{
		friend class vertexbuffer;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_vertexbuffer> };

		uint32_t		m_handle	{}; // handle
		uint32_t const	m_usage		{}; // draw usage
		buffer_layout	m_layout	{}; // buffer layout
		buffer_t		m_buffer	{}; // local data

	public:
		opengl_vertexbuffer(uint32_t usage, size_t count, address_t data);

		~opengl_vertexbuffer() override;

		bool invalidate() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	public:
		void set_data(size_t count, address_t data, size_t offset = 0) override;

		void set_layout(buffer_layout const & value) override { m_layout = value; }

		buffer_t const & get_buffer() const noexcept override { return m_buffer; }

		size_t get_count() const noexcept override { return m_buffer.size() / sizeof(float_t); }

		buffer_layout const & get_layout() const noexcept override { return m_layout; }

		uint32_t get_usage() const noexcept override { return m_usage; }

	protected:
		static void do_bind(opengl_vertexbuffer const * value);

		bool do_is_equal(device_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gfx
{
	// opengl indexbuffer
	class opengl_indexbuffer final : public indexbuffer
	{
		friend class indexbuffer;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_indexbuffer> };

		uint32_t		m_handle	{}; // handle
		uint32_t const	m_usage		{}; // usage
		buffer_t		m_buffer	{}; // local data

	public:
		opengl_indexbuffer(uint32_t usage, size_t count, address_t data);

		~opengl_indexbuffer() override;

		bool invalidate() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	public:
		void set_data(size_t count, address_t data, size_t offset = 0) override;

		buffer_t const & get_buffer() const noexcept override { return m_buffer; }

		size_t get_count() const noexcept override { return m_buffer.size() / sizeof(uint32_t); }

		uint32_t get_usage() const noexcept override { return m_usage; }

	protected:
		static void do_bind(opengl_indexbuffer const * value);

		bool do_is_equal(device_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gfx
{
	// opengl vertexarray
	class opengl_vertexarray final : public vertexarray
	{
		friend class vertexarray;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_vertexarray> };

		uint32_t							m_handle	{}; // handle
		uint32_t const						m_mode		{}; // primitive type
		shared<indexbuffer>					m_indices	{}; // index buffer
		pmr::vector<shared<vertexbuffer>>	m_vertices	{}; // vertex buffers

	public:
		opengl_vertexarray(uint32_t mode);

		~opengl_vertexarray() override;

		bool invalidate() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	public:
		void add_vertices(shared<vertexbuffer> const & value) override;

		void set_indices(shared<indexbuffer> const & value) override;

		shared<indexbuffer> const & get_indices() const noexcept override { return m_indices; }

		uint32_t get_mode() const noexcept override { return m_mode; }

		pmr::vector<shared<vertexbuffer>> const & get_vertices() const noexcept override { return m_vertices; }

	protected:
		static void do_bind(opengl_vertexarray const * value);

		bool do_is_equal(device_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gfx
{
	// opengl texture2d
	class opengl_texture2d final : public texture2d
	{
		friend class texture2d;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_texture2d> };

		uint32_t	m_handle	{}			; // handle
		texopts		m_opts		{}			; // texture options
		bool		m_lock		{ true }	; // locked

	public:
		opengl_texture2d(texopts const & opts, address_t data);

		~opengl_texture2d() override;

		bool invalidate() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	public:
		void lock() override;

		void unlock() override;

		void update(vec2i const & size, address_t data = nullptr) override;

		void update(vec2i const & pos, vec2i const & size, address_t data) override;

		void set_mipmapped(bool value) override;

		void set_repeated(bool value) override;

		void set_smooth(bool value) override;

		image copy_to_image() const override;

		texopts const & get_texopts() const noexcept { return m_opts; }

	protected:
		static void do_bind(opengl_texture2d const * value, uint32_t slot);

		bool do_is_equal(device_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texturecube
namespace ml::gfx
{
	// opengl texturecube
	class opengl_texturecube : public texturecube
	{
		friend class texturecube;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_texturecube> };

		uint32_t	m_handle	{}			; // handle
		texopts		m_opts		{}			; // texture options
		bool		m_lock		{ true }	; // locked

	public:
		opengl_texturecube(texopts const & opts);

		~opengl_texturecube() override;

		bool invalidate() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	public:
		void lock() override;

		void unlock() override;

		texopts const & get_texopts() const noexcept { return m_opts; }

	protected:
		static void do_bind(opengl_texturecube const * value, uint32_t slot);

		bool do_is_equal(device_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gfx
{
	// opengl framebuffer
	class opengl_framebuffer final : public framebuffer
	{
		friend class framebuffer;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_framebuffer> };

		uint32_t			m_handle		{}	; // handle
		texopts				m_opts			{}	; // texture options
		tex_buffer_t		m_attachments	{}	; // color attachments
		shared<texture2d>	m_depth			{}	; // depth attachment

		
	public:
		opengl_framebuffer(texopts const & opts);

		~opengl_framebuffer() override;

		bool invalidate() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	public:
		bool attach(shared<texture2d> const & value) override;

		bool detach(shared<texture2d> const & value) override;

		void resize(vec2i const & value) override;

		pmr::vector<shared<texture2d>> const & get_color_attachments() const noexcept override { return m_attachments; }

		shared<texture2d> const & get_depth_attachment() const noexcept override { return m_depth; }

		texopts const & get_texopts() const noexcept override { return m_opts; }

	protected:
		static void do_bind(opengl_framebuffer const * value);

		bool do_is_equal(device_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gfx
{
	// opengl shader
	class opengl_shader final : public shader
	{
		friend class shader;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_shader> };

		uint32_t		m_handle		{}; // handle
		pmr::string		m_error_log		{}; // error log
		uint32_t const	m_shader_type	{}; // type
		int32_t const	m_flags			{};
		shader_src_t	m_source		{}; // source

	public:
		opengl_shader(uint32_t type, int32_t flags = shader_flags_default);

		~opengl_shader() override;

		bool invalidate() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	public:
		bool compile(shader_src_t const & src) override;

		pmr::string const & get_error_log() const noexcept override { return m_error_log; }

		int32_t get_flags() const noexcept override { return m_flags; }

		uint32_t get_shader_type() const noexcept override { return m_shader_type; }

		pmr::vector<pmr::string> const & get_source() const noexcept override { return m_source; }

	protected:
		bool do_is_equal(device_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gfx
{
	// opengl program
	class opengl_program final : public program
	{
		friend class program;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_program> };

		uint32_t		m_handle		{}; // handle
		pmr::string		m_error_log		{}; // error log
		shader_map_t	m_shaders		{}; // shader cache
		tex_cache_t		m_textures		{}; // texture cache
		uni_cache_t		m_uniforms		{}; // uniform cache

		// uniform binder
		struct ML_NODISCARD opengl_uniform_binder final
		{
			uniform_id location{ ML_handle(uniform_id, -1) };

			uint32_t self{}, last{};

			operator bool() const noexcept { return -1 < ML_handle(int32_t, location); }

			opengl_uniform_binder(opengl_program & s, cstring name) noexcept;

			~opengl_uniform_binder() noexcept;
		};

	public:
		opengl_program();

		~opengl_program() override;

		bool invalidate() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	public:
		bool attach(shared<shader> const & value) override;

		bool detach(shared<shader> const & value) override;

		bool link() override;

		bool bind_uniform(cstring name, uni_binder_t const & fn) override
		{
			opengl_uniform_binder u{ *this, name };
			if (u) { std::invoke(fn, u.location); }
			return u;
		}

		pmr::string const & get_error_log() const noexcept override { return m_error_log; }

		shader_map_t const & get_shaders() const noexcept override { return m_shaders; }

		tex_cache_t const & get_textures() const noexcept override { return m_textures; }

		uni_cache_t const & get_uniforms() const noexcept override { return m_uniforms; }

	protected:
		static void do_bind(opengl_program const * value);

		bool do_is_equal(device_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}

		void do_cache_texture(uniform_id loc, shared<texture> const & value) noexcept override
		{
			static auto const max_texture_slots
			{
				(size_t)device::get_context()->get_devinfo().max_texture_slots
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