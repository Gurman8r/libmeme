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

		devinfo const & get_devinfo() const noexcept override;

		uint32_t get_error() const noexcept override;

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
		
		void draw_arrays(uint32_t mode, size_t first, size_t count) override;
		
		void draw_indexed(uint32_t mode, size_t count) override;
		
		void flush() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void upload(register_t loc, bool value) override;

		void upload(register_t loc, int32_t value) override;

		void upload(register_t loc, float_t value) override;

		void upload(register_t loc, vec2 const & value) override;

		void upload(register_t loc, vec3 const & value) override;

		void upload(register_t loc, vec4 const & value) override;

		void upload(register_t loc, mat2 const & value) override;

		void upload(register_t loc, mat3 const & value) override;

		void upload(register_t loc, mat4 const & value) override;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gl
{
	// opengl vertexarray object
	class opengl_vertexarray final : public vertexarray
	{
		friend class vertexarray;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_vertexarray> };

		uint32_t							m_handle	{}; // handle
		uint32_t const						m_mode		{}; // primitive type
		shared<indexbuffer>					m_indices	{}; // index buffer
		pmr::vector<shared<vertexbuffer>>	m_vertices	{}; // vertex buffers

	public:
		opengl_vertexarray(uint32_t mode) noexcept;

		~opengl_vertexarray() noexcept override;

		bool generate() override;

		bool destroy() override;

		void add_vertices(shared<vertexbuffer> const & value) override;

		void set_indices(shared<indexbuffer> const & value) override;

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		shared<indexbuffer> const & get_indices() const noexcept override { return m_indices; }

		uint32_t get_mode() const noexcept override { return m_mode; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

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

// vertexbuffer
namespace ml::gl
{
	// opengl vertexbuffer object
	class opengl_vertexbuffer final : public vertexbuffer
	{
		friend class vertexbuffer;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_vertexbuffer> };

		uint32_t		m_handle	{}; // handle
		uint32_t const	m_usage		{}; // draw usage
		buffer_layout			m_layout	{}; // buffer layout
		buffer_t		m_buffer	{}; // local data

	public:
		opengl_vertexbuffer(uint32_t usage, size_t count, address_t data) noexcept;

		~opengl_vertexbuffer() noexcept override;

		bool generate() override;

		bool destroy() override;

		void set_data(size_t count, address_t data, size_t offset = 0) override;

		void set_layout(buffer_layout const & value) override { m_layout = value; }

		buffer_t const & get_buffer() const noexcept override { return m_buffer; }

		size_t get_count() const noexcept override { return m_buffer.size() / sizeof(float_t); }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		buffer_layout const & get_layout() const noexcept override { return m_layout; }

		uint32_t get_usage() const noexcept override { return m_usage; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

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
namespace ml::gl
{
	// opengl indexbuffer object
	class opengl_indexbuffer final : public indexbuffer
	{
		friend class indexbuffer;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_indexbuffer> };

		uint32_t		m_handle	{}; // handle
		uint32_t const	m_usage		{}; // usage
		buffer_t		m_buffer	{}; // local data

	public:
		opengl_indexbuffer(uint32_t usage, size_t count, address_t data) noexcept;

		~opengl_indexbuffer() noexcept override;

		bool generate() override;

		bool destroy() override;

		void set_data(size_t count, address_t data, size_t offset = 0) override;

		buffer_t const & get_buffer() const noexcept override { return m_buffer; }

		size_t get_count() const noexcept override { return m_buffer.size() / sizeof(uint32_t); }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

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

// texture2d
namespace ml::gl
{
	// opengl texture2d object
	class opengl_texture2d final : public texture2d
	{
		friend class texture2d;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_texture2d> };

		uint32_t	m_handle	{}; // handle
		texopts		m_opts		{}; // texture options

	public:
		opengl_texture2d(texopts const & opts, address_t data) noexcept;

		~opengl_texture2d() noexcept override;

		bool generate() override;

		bool destroy() override;

		void update(vec2i const & size, address_t data = nullptr) override;

		void update(vec2i const & pos, vec2i const & size, address_t data) override;

		void set_mipmapped(bool value) override;

		void set_repeated(bool value) override;

		void set_smooth(bool value) override;

		image copy_to_image() const override;

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		texopts const & get_texture_options() const noexcept { return m_opts; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

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
namespace ml::gl
{
	// opengl texturecube object
	class opengl_texturecube : public texturecube
	{
		friend class texturecube;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_texturecube> };

		uint32_t	m_handle	{}; // handle
		texopts		m_opts		{}; // texture options

	public:
		opengl_texturecube(texopts const & opts) noexcept;

		~opengl_texturecube() noexcept override;

		bool generate() override;

		bool destroy() override;

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		texopts const & get_texture_options() const noexcept { return m_opts; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

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
namespace ml::gl
{
	// opengl framebuffer object
	class opengl_framebuffer final : public framebuffer
	{
		friend class framebuffer;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_framebuffer> };

		uint32_t			m_handle	{}	; // handle
		vec2i				m_size		{}	; // size
		uint32_t			m_format	{}	; // format
		shared<texture2d>	m_color		{}	; // color attachment
		shared<texture2d>	m_depth		{}	; // depth attachment

	public:
		opengl_framebuffer(vec2i const & size, uint32_t format) noexcept;

		~opengl_framebuffer() noexcept override;

		bool generate() override;

		bool destroy() override;

		void resize(vec2i const & value) override;

		shared<texture2d> const & get_color_attachment() const noexcept override { return m_color; }

		shared<texture2d> const & get_depth_attachment() const noexcept override { return m_depth; }

		uint32_t get_format() const noexcept override { return m_format; }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		vec2i const & get_size() const noexcept override { return m_size; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	protected:
		static void do_bind(opengl_framebuffer const * value);

		bool do_is_equal(device_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gl
{
	// opengl program object
	class opengl_program final : public program
	{
		friend class program;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_program> };

		uint32_t		m_handle		{}; // handle
		pmr::string		m_error_log		{}; // error log
		uint32_t const	m_shader_type	{}; // type
		source_t		m_source		{}; // source

	public:
		opengl_program(uint32_t type) noexcept;

		~opengl_program() noexcept override;

		bool generate() override;

		bool destroy() override;

		bool compile(source_t const & src) override;

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		pmr::string const & get_error_log() const noexcept override { return m_error_log; }

		uint32_t get_shader_type() const noexcept override { return m_shader_type; }

		source_t const & get_source() const noexcept override { return m_source; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

	protected:
		bool do_is_equal(device_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gl
{
	// opengl shader object
	class opengl_shader final : public shader
	{
		friend class shader;

		static constexpr typeof<> s_type_info{ typeof_v<opengl_shader> };

		uint32_t		m_handle		{}; // handle
		pmr::string		m_error_log		{}; // error log
		pgm_cache_t		m_programs		{}; // program cache
		tex_cache_t		m_textures		{}; // texture cache
		reg_cache_t		m_uniforms		{}; // uniform cache

		// uniform binder
		struct ML_NODISCARD opengl_uniform_binder final
		{
			register_t location{ ML_addressof(-1) };

			uint32_t self{}, last{};

			operator bool() const noexcept { return -1 < (int32_t)(intptr_t)location; }

			opengl_uniform_binder(opengl_shader & s, cstring name) noexcept;

			~opengl_uniform_binder() noexcept;
		};

	public:
		opengl_shader() noexcept;

		~opengl_shader() noexcept override;

		bool generate() override;

		bool destroy() override;

		bool attach(shared<program> const & value) override;

		bool detach(shared<program> const & value) override;

		bool link() override;

		bool bind_register(cstring name, reg_clbk_t const & fn) override
		{
			opengl_uniform_binder u{ *this, name };
			if (u) { std::invoke(fn, u.location); }
			return u;
		}

		pmr::string const & get_error_log() const noexcept { return m_error_log; }

		handle_t get_handle() const noexcept override { return ML_addressof(m_handle); }

		pgm_cache_t const & get_programs() const noexcept override { return m_programs; }

		tex_cache_t const & get_textures() const noexcept override { return m_textures; }

		typeof<> const & get_type_info() const noexcept override { return s_type_info; }

		reg_cache_t const & get_uniforms() const noexcept override { return m_uniforms; }

	protected:
		static void do_bind(opengl_shader const * value);

		bool do_is_equal(device_resource const & other) const noexcept override
		{
			return this == std::addressof(other); // TODO
		}

		void do_cache_texture(register_t loc, shared<texture> const & value) noexcept override
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