#if defined(ML_IMPL_RENDERER_OPENGL)
#ifndef _ML_IMPL_RENDERER_OPENGL_HPP_
#define _ML_IMPL_RENDERER_OPENGL_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Graphics/RenderAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device
namespace ml::gfx
{
	class opengl_device final : public device
	{
	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class device;

		static constexpr typeof<> s_typeof{ typeof_v<opengl_device> };

		shared<context>	m_devctx	{}; // 
		devinfo			m_devinfo	{}; // 

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		opengl_device();

		~opengl_device() override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, this); }

		void set_context(shared<context> const & value) noexcept override { m_devctx = value; }

		shared<context> const & get_context() const noexcept override { return m_devctx; }

		devinfo const & get_info() const noexcept override { return m_devinfo; }

		typeof<> const & get_typeof() const noexcept override { return s_typeof; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device context
namespace ml::gfx
{
	// opengl device context
	class opengl_context final : public context
	{
	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class context;

		static constexpr typeof<> s_typeof{ typeof_v<opengl_context> };

		context_settings m_settings; // context settings

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		opengl_context(device * parent, context_settings const & cs);

		~opengl_context() override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, this); }

		context_settings const & get_settings() const noexcept override { return m_settings; }

		typeof<> const & get_typeof() const noexcept override { return s_typeof; }

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

		void set_stencil_enabled(bool enabled) override;

		void set_stencil_mode(stencil_mode const & value) override;

		void set_viewport(int_rect const & bounds) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear(uint32_t mask) override;

		void draw(shared<vertexarray> const & value) override;
		
		void draw_arrays(uint32_t prim, size_t first, size_t count) override;
		
		void draw_indexed(uint32_t prim, size_t count) override;
		
		void flush() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void upload(uniform_id loc, bool value) override;

		void upload(uniform_id loc, int32_t value) override;

		void upload(uniform_id loc, float_t value) override;

		void upload(uniform_id loc, vec2f const & value) override;

		void upload(uniform_id loc, vec3f const & value) override;

		void upload(uniform_id loc, vec4f const & value) override;

		void upload(uniform_id loc, mat2f const & value) override;

		void upload(uniform_id loc, mat3f const & value)  override;

		void upload(uniform_id loc, mat4f const & value) override;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gfx
{
	// opengl vertexarray
	class opengl_vertexarray final : public vertexarray
	{
	private:
		friend class vertexarray;

		static constexpr typeof<> s_typeof{ typeof_v<opengl_vertexarray> };

		uint32_t							m_handle	{}; // handle
		uint32_t const						m_primitive	{}; // prim type
		shared<indexbuffer>					m_indices	{}; // index buffer
		pmr::vector<shared<vertexbuffer>>	m_vertices	{}; // vertex buffers

	public:
		opengl_vertexarray(device * parent, uint32_t prim);

		~opengl_vertexarray() override;

		bool revalue() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_typeof() const noexcept override { return s_typeof; }

	public:
		void add_vertices(shared<vertexbuffer> const & value) override;

		void set_indices(shared<indexbuffer> const & value) override;

		shared<indexbuffer> const & get_indices() const noexcept override { return m_indices; }

		uint32_t get_primitive() const noexcept override { return m_primitive; }

		pmr::vector<shared<vertexbuffer>> const & get_vertices() const noexcept override { return m_vertices; }

	protected:
		static void do_bind(opengl_vertexarray const * value);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gfx
{
	// opengl vertexbuffer
	class opengl_vertexbuffer final : public vertexbuffer
	{
	private:
		friend class vertexbuffer;

		static constexpr typeof<> s_typeof{ typeof_v<opengl_vertexbuffer> };

		uint32_t		m_handle	{}; // handle
		uint32_t const	m_usage		{}; // draw usage
		buffer_layout	m_layout	{}; // buffer layout
		buffer_t			m_buffer	{}; // local data

	public:
		opengl_vertexbuffer(device * parent, uint32_t usage, size_t count, address_t data);

		~opengl_vertexbuffer() override;

		bool revalue() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_typeof() const noexcept override { return s_typeof; }

	public:
		void set_data(size_t count, address_t data, size_t offset = 0) override;

		void set_layout(buffer_layout const & value) override { m_layout = value; }

		buffer_t const & get_buffer() const noexcept override { return m_buffer; }

		size_t get_count() const noexcept override { return m_buffer.size() / sizeof(float_t); }

		buffer_layout const & get_layout() const noexcept override { return m_layout; }

		uint32_t get_usage() const noexcept override { return m_usage; }

	protected:
		static void do_bind(opengl_vertexbuffer const * value);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gfx
{
	// opengl indexbuffer
	class opengl_indexbuffer final : public indexbuffer
	{
	private:
		friend class indexbuffer;

		static constexpr typeof<> s_typeof{ typeof_v<opengl_indexbuffer> };

		uint32_t		m_handle	{}; // handle
		uint32_t const	m_usage		{}; // usage
		buffer_t		m_buffer	{}; // local data

	public:
		opengl_indexbuffer(device * parent, uint32_t usage, size_t count, address_t data);

		~opengl_indexbuffer() override;

		bool revalue() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_typeof() const noexcept override { return s_typeof; }

	public:
		void set_data(size_t count, address_t data, size_t offset = 0) override;

		buffer_t const & get_buffer() const noexcept override { return m_buffer; }

		size_t get_count() const noexcept override { return m_buffer.size() / sizeof(uint32_t); }

		uint32_t get_usage() const noexcept override { return m_usage; }

	protected:
		static void do_bind(opengl_indexbuffer const * value);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gfx
{
	// opengl texture2d
	class opengl_texture2d final : public texture2d
	{
	private:
		friend class texture2d;

		static constexpr typeof<> s_typeof{ typeof_v<opengl_texture2d> };

		uint32_t	m_handle	{}			; // handle
		texopts		m_opts		{}			; // texture options
		bool		m_lock		{ true }	; // locked

	public:
		opengl_texture2d(device * parent, texopts const & opts, address_t data);

		~opengl_texture2d() override;

		bool revalue() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_typeof() const noexcept override { return s_typeof; }

	public:
		void lock() override;

		void unlock() override;

		void update(vec2i const & size, address_t data = {}) override;

		void update(vec2i const & pos, vec2i const & size, address_t data) override;

		void set_mipmapped(bool value) override;

		void set_repeated(bool value) override;

		void set_smooth(bool value) override;

		image copy_to_image() const override;

		texopts const & get_texture_options() const noexcept { return m_opts; }

	protected:
		static void do_bind(opengl_texture2d const * value, uint32_t slot);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texturecube
namespace ml::gfx
{
	// opengl texturecube
	class opengl_texturecube : public texturecube
	{
	private:
		friend class texturecube;

		static constexpr typeof<> s_typeof{ typeof_v<opengl_texturecube> };

		uint32_t	m_handle	{}			; // handle
		texopts		m_opts		{}			; // texture options
		bool		m_lock		{ true }	; // locked

	public:
		opengl_texturecube(device * parent, texopts const & opts);

		~opengl_texturecube() override;

		bool revalue() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_typeof() const noexcept override { return s_typeof; }

	public:
		void lock() override;

		void unlock() override;

		texopts const & get_texture_options() const noexcept { return m_opts; }

	protected:
		static void do_bind(opengl_texturecube const * value, uint32_t slot);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gfx
{
	// opengl framebuffer
	class opengl_framebuffer final : public framebuffer
	{
	private:
		friend class framebuffer;

		static constexpr typeof<> s_typeof{ typeof_v<opengl_framebuffer> };

		uint32_t						m_handle		{}	; // handle
		texopts							m_opts			{}	; // texture options
		pmr::vector<shared<texture2d>>	m_attachments	{}	; // color attachments
		shared<texture2d>				m_depth			{}	; // depth attachment

		
	public:
		opengl_framebuffer(device * parent, texopts const & opts);

		~opengl_framebuffer() override;

		bool revalue() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_typeof() const noexcept override { return s_typeof; }

	public:
		bool attach(shared<texture2d> const & value) override;

		bool detach(shared<texture2d> const & value) override;

		void resize(vec2i const & value) override;

		pmr::vector<shared<texture2d>> const & get_color_attachments() const noexcept override { return m_attachments; }

		shared<texture2d> const & get_depth_attachment() const noexcept override { return m_depth; }

		texopts const & get_texture_options() const noexcept override { return m_opts; }

	protected:
		static void do_bind(opengl_framebuffer const * value);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gfx
{
	// opengl shader
	class opengl_shader final : public shader
	{
	private:
		friend class shader;

		static constexpr typeof<> s_typeof{ typeof_v<opengl_shader> };

		uint32_t					m_handle		{}; // handle
		pmr::string					m_error_log		{}; // error log
		int32_t const				m_flags			{}; // shader flags
		uint32_t const				m_shader_type	{}; // type
		pmr::vector<pmr::string>	m_source		{}; // source

	public:
		opengl_shader(device * parent, uint32_t type, int32_t flags = shader_flags_default);

		~opengl_shader() override;

		bool revalue() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_typeof() const noexcept override { return s_typeof; }

	public:
		bool compile(pmr::vector<pmr::string> const & src) override;

		pmr::string const & get_error_log() const noexcept override { return m_error_log; }

		int32_t get_flags() const noexcept override { return m_flags; }

		uint32_t get_shader_type() const noexcept override { return m_shader_type; }

		pmr::vector<pmr::string> const & get_source() const noexcept override { return m_source; }

	protected:
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gfx
{
	// opengl program
	class opengl_program final : public program
	{
	private:
		friend class program;

		static constexpr typeof<> s_typeof{ typeof_v<opengl_program> };

		uint32_t								m_handle		{}; // handle
		pmr::string								m_error_log		{}; // error log
		int32_t const							m_flags			{}; // program flags
		ds::map<uint32_t, shared<shader>>		m_shaders		{}; // shader cache
		ds::map<uniform_id, shared<texture>>	m_textures		{}; // texture cache
		ds::map<hash_t, uniform_id>				m_uniforms		{}; // uniform cache

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
		opengl_program(device * parent, int32_t flags = program_flags_default);

		~opengl_program() override;

		bool revalue() override;

		resource_id get_handle() const noexcept override { return ML_handle(resource_id, m_handle); }

		typeof<> const & get_typeof() const noexcept override { return s_typeof; }

	public:
		bool attach(shared<shader> const & value) override;

		bool detach(shared<shader> const & value) override;

		bool link() override;

		bool validate() override;

		bool bind_uniform(cstring name, std::function<void(uniform_id)> const & fn) override
		{
			opengl_uniform_binder u{ *this, name };
			if (u) { std::invoke(fn, u.location); }
			return u;
		}

		pmr::string const & get_error_log() const noexcept override { return m_error_log; }

		int32_t get_flags() const noexcept override { return m_flags; }

		ds::map<uint32_t, shared<shader>> const & get_shaders() const noexcept override { return m_shaders; }

		ds::map<uniform_id, shared<texture>> const & get_textures() const noexcept override { return m_textures; }

		ds::map<hash_t, uniform_id> const & get_uniforms() const noexcept override { return m_uniforms; }

	protected:
		static void do_bind(opengl_program const * value);

		void do_cache_texture(uniform_id loc, shared<texture> const & value) noexcept override
		{
			static auto const max_texture_slots
			{
				(size_t)device::get_default()->get_info().max_texture_slots
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
