#ifndef _ML_RENDERER_API_HPP_
#define _ML_RENDERER_API_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Color.hpp>
#include <libmeme/Core/Rect.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// types
namespace ml::gl
{
	// classes
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class render_api;
	class vertex_array;
	class vertex_buffer;
	class index_buffer;
	class frame_buffer;
	class shader_object;
	class texture_object;

	// aliases
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias			allocator	= pmr::polymorphic_allocator<byte_t>; // allocator type
	ML_alias			buffer		= void const *			; // generic data buffer type
	ML_alias			command		= std::function<void()>	; // render command type
	ML_alias			handle		= void *				; // generic handle type
	ML_alias_T(class T)	shared		= std::shared_ptr<T>	; // object pointer type

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// enums
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// conversion helpers

	struct to_user : std::false_type {}; // ( false type ) | backend enum -> user enum

	struct to_impl : std::true_type {}; // ( true type ) | user enum -> backend enum

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum clear_flags_ : uint32_t
	{
		clear_flags_none,
		clear_flags_accum	= 1 << 0,
		clear_flags_color	= 1 << 1,
		clear_flags_depth	= 1 << 2,
		clear_flags_stencil = 1 << 3,
	};

	enum error_ : uint32_t
	{
		error_no_error,
		error_invalid_enum,
		error_invalid_value,
		error_invalid_operation,
		error_stack_overflow,
		error_stack_underflow,
		error_out_of_memory,
		error_invalid_framebuffer_operation,
	};

	enum draw_usage_ : uint32_t
	{
		usage_stream,
		usage_static,
		usage_dynamic,
	};

	enum primitive_ : uint32_t
	{
		primitive_points,
		primitive_lines,
		primitive_line_loop,
		primitive_line_strip,
		primitive_triangles,
		primitive_triangle_strip,
		primitive_triangle_fan,
		primitive_fill,
	};

	enum predicate_ : uint32_t
	{
		predicate_never,
		predicate_less,
		predicate_equal,
		predicate_lequal,
		predicate_greater,
		predicate_notequal,
		predicate_gequal,
		predicate_always,
	};

	enum type_ : uint32_t
	{
		type_byte,
		type_unsigned_byte,
		type_short,
		type_unsigned_short,
		type_int,
		type_unsigned_int,
		type_float,
		type_half_float,
	};

	enum function_ : uint32_t
	{
		function_add,
		function_subtract,
		function_reverse_subtract,
		function_min,
		function_max,
	};

	enum order_
	{
		order_clockwise,
		order_counter_clockwise,
	};

	enum facet_ : uint32_t
	{
		facet_front_left,
		facet_front_right,
		facet_back_left,
		facet_back_right,
		facet_front,
		facet_back,
		facet_left,
		facet_right,
		facet_front_and_back,
	};

	enum factor_ : uint32_t
	{
		factor_zero,
		factor_one,
		factor_src_color,
		factor_one_minus_src_color,
		factor_src_alpha,
		factor_one_minus_src_alpha,
		factor_dst_alpha,
		factor_one_minus_dst_alpha,
		factor_dst_color,
		factor_one_minus_dst_color,
		factor_src_alpha_saturate,
	};

	enum format_ : uint32_t
	{
		format_red,
		format_green,
		format_blue,
		format_alpha,
		format_rgb,
		format_rgba,
		format_luminance,
		format_luminance_alpha,
		format_srgb,
		format_srgb8,
		format_srgb_alpha,
		format_srgb8_alpha8,
		format_sluminance_alpha,
		format_sluminance8_alpha8,
		format_sluminance,
		format_sluminance8,
		format_depth24_stencil8,
	};

	enum texture_type_ : uint32_t
	{
		texture_type_1d,
		texture_type_2d,
		texture_type_3d,
		texture_type_cube_map,
	};

	enum shader_type_ : uint32_t
	{
		shader_type_vertex,
		shader_type_fragment,
		shader_type_geometry,
	};

	enum attachment_ : uint32_t
	{
		color_attachment_0,
		color_attachment_1,
		color_attachment_2,
		color_attachment_3,
		color_attachment_4,
		color_attachment_5,
		color_attachment_6,
		color_attachment_7,
		color_attachment_8,
		color_attachment_9,
		color_attachment_10,
		color_attachment_11,
		color_attachment_12,
		color_attachment_13,
		color_attachment_14,
		color_attachment_15,
		color_attachment_max,

		depth_attchment,
		stencil_attachment,
		depth_stencil_attachment,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// layout
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vertex buffer layout element
	struct ML_NODISCARD buffer_element final
	{
		pmr::string name		{};
		hash_t		type		{};
		uint32_t	size		{};
		uint32_t	offset		{};
		bool		normalized	{};

		hash_t get_base_type() const noexcept
		{
			switch (type)
			{
			default					: return 0;
			case hashof_v<bool>		: return hashof_v<bool>;
			case hashof_v<int32_t>	:
			case hashof_v<vec2i>	:
			case hashof_v<vec3i>	:
			case hashof_v<vec4i>	:
			case hashof_v<mat2i>	:
			case hashof_v<mat3i>	:
			case hashof_v<mat4i>	: return hashof_v<int32_t>;
			case hashof_v<float_t>	:
			case hashof_v<vec2f>	:
			case hashof_v<vec3f>	:
			case hashof_v<vec4f>	:
			case hashof_v<mat2f>	:
			case hashof_v<mat3f>	:
			case hashof_v<mat4f>	: return hashof_v<float_t>;
			}
		}

		uint32_t get_component_count() const noexcept
		{
			switch (type)
			{
			default					: return 0;
			case hashof_v<bool>		:
			case hashof_v<int32_t>	:
			case hashof_v<float_t>	: return 1;
			case hashof_v<vec2i>	:
			case hashof_v<vec2f>	: return 2 * 1;
			case hashof_v<vec3i>	:
			case hashof_v<vec3f>	: return 3 * 1;
			case hashof_v<vec4i>	:
			case hashof_v<vec4f>	: return 4 * 1;
			case hashof_v<mat2i>	:
			case hashof_v<mat2f>	: return 2 * 2;
			case hashof_v<mat3i>	:
			case hashof_v<mat3f>	: return 3 * 3;
			case hashof_v<mat4i>	:
			case hashof_v<mat4f>	: return 4 * 4;
			}
		}
	};

	template <class T
	> ML_NODISCARD auto layout_element(pmr::string const & name, bool normalized = 0) noexcept
	{
		static_assert(util::is_any_of_v<T,
			bool,
			int32_t, vec2i, vec3i, vec4i, mat2i, mat3i, mat4i,
			float_t, vec2f, vec3f, vec4f, mat2f, mat3f, mat4f
		>);
		return buffer_element{ name, hashof_v<T>, sizeof(T), 0, normalized };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vertex buffer layout
	struct ML_NODISCARD buffer_layout final
	{
		template <class ... Args
		> buffer_layout(Args && ... args) noexcept : m_elements{ ML_forward(args)... }
		{
			uint32_t offset{};
			for (auto & e : m_elements)
			{
				e.offset = offset;
				offset += e.size;
				m_stride += e.size;
			}
		}

		auto const & get_elements() const & noexcept { return m_elements; }

		uint32_t get_stride() const noexcept { return m_stride; }

	private:
		pmr::vector<buffer_element> m_elements{};
		
		uint32_t m_stride{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// objects
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vertex array
	class ML_RENDERER_API vertex_array : public trackable, public non_copyable
	{
	public:
		virtual ~vertex_array() = default;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual void add_vertices(shared<vertex_buffer> const & value) = 0;

		virtual void set_indices(shared<index_buffer> const & value) = 0;

		ML_NODISCARD virtual handle get_handle() const = 0;

		ML_NODISCARD virtual shared<index_buffer> const & get_indices() const = 0;
		
		ML_NODISCARD virtual uint32_t get_mode() const = 0;
		
		ML_NODISCARD virtual pmr::vector<shared<vertex_buffer>> const & get_vertices() const = 0;

		ML_NODISCARD static shared<vertex_array> create(uint32_t mode, allocator alloc = {});
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vertex buffer
	class ML_RENDERER_API vertex_buffer : public trackable, public non_copyable
	{
	public:
		virtual ~vertex_buffer() = default;

		virtual void bind() const = 0;
		
		virtual void unbind() const = 0;

		virtual void set_data(buffer vertices, uint32_t size, uint32_t offset = 0) = 0;

		virtual void set_layout(buffer_layout const & value) = 0;

		ML_NODISCARD virtual handle get_handle() const = 0;

		ML_NODISCARD virtual buffer_layout const & get_layout() const = 0;

		ML_NODISCARD virtual uint32_t get_size() const = 0;

		ML_NODISCARD static shared<vertex_buffer> create(buffer vertices, uint32_t size, uint32_t usage = usage_static, allocator alloc = {});

		ML_NODISCARD static shared<vertex_buffer> create(uint32_t size, uint32_t usage = usage_dynamic, allocator alloc = {});
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// index buffer
	class ML_RENDERER_API index_buffer : public trackable, public non_copyable
	{
	public:
		virtual ~index_buffer() = default;

		virtual void bind() const = 0;
		
		virtual void unbind() const = 0;

		virtual void set_data(buffer indices, uint32_t count) = 0;

		ML_NODISCARD virtual uint32_t get_count() const = 0;

		ML_NODISCARD virtual handle get_handle() const = 0;

		ML_NODISCARD static shared<index_buffer> create(buffer indices, uint32_t count, allocator alloc = {});
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// frame buffer
	class ML_RENDERER_API frame_buffer : public trackable, public non_copyable
	{
	public:
		virtual ~frame_buffer() = default;

		virtual void bind() const = 0;
		
		virtual void unbind() const = 0;

		virtual void resize(vec2i const & value) = 0;

		ML_NODISCARD virtual uint32_t get_color_attachment() const = 0;

		ML_NODISCARD virtual uint32_t get_depth_attachment() const = 0;

		ML_NODISCARD virtual uint32_t get_format() const = 0;

		ML_NODISCARD virtual handle get_handle() const = 0;

		ML_NODISCARD virtual vec2i get_size() const = 0;

		ML_NODISCARD static shared<frame_buffer> create(uint32_t format, vec2i const & size, allocator alloc = {});
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// shader
	class ML_RENDERER_API shader_object : public trackable, public non_copyable
	{
	public:
		virtual ~shader_object() = default;

		virtual void bind() const = 0;
		
		virtual void unbind() const = 0;

		ML_NODISCARD virtual handle get_handle() const = 0;

		ML_NODISCARD static shared<shader_object> create(allocator alloc = {});
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// texture
	class ML_RENDERER_API texture_object : public trackable, public non_copyable
	{
	public:
		virtual ~texture_object() = default;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		ML_NODISCARD virtual handle get_handle() const = 0;

		ML_NODISCARD virtual uint32_t get_type() const = 0;

		ML_NODISCARD static shared<texture_object> create(allocator alloc = {});
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// states
namespace ml::gl
{
	struct ML_NODISCARD alpha_function final
	{
		uint32_t func;
		float_t ref;
	};

	struct ML_NODISCARD blend_equation final
	{
		uint32_t
			modeRGB,
			modeAlpha = modeRGB;
	};

	struct ML_NODISCARD blend_function final
	{
		uint32_t
			sfactorRGB,
			dfactorRGB,
			sfactorAlpha = sfactorRGB,
			dfactorAlpha = dfactorRGB;
	};

	struct ML_NODISCARD depth_range final
	{
		float_t
			nearVal,
			farVal;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// api
namespace ml::gl
{
	// render api
	class ML_RENDERER_API render_api : public trackable, public non_copyable
	{
	protected:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_api() noexcept = default;

		virtual bool do_initialize() = 0;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static render_api * get_instance() noexcept;

		inline bool initialize() noexcept
		{
			bool temp{};
			static ML_scope{ temp = this->do_initialize(); }; // once
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual uint32_t get_error() const = 0;
		
		virtual cstring get_extensions() const = 0;
		
		virtual int32_t get_major_version() const = 0;
		
		virtual int32_t get_minor_version() const = 0;
		
		virtual int32_t get_num_extensions() const = 0;
		
		virtual cstring get_renderer() const = 0;
		
		virtual cstring get_shading_language_version() const = 0;
		
		virtual cstring get_vendor() const = 0;
		
		virtual cstring get_version() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool get_alpha_enabled() const = 0;

		virtual alpha_function get_alpha_function() const = 0;
		
		virtual bool get_blend_enabled() const = 0;

		virtual color get_blend_color() const = 0;
		
		virtual blend_equation get_blend_equation() const = 0;
		
		virtual blend_function get_blend_function() const = 0;
		
		virtual color get_clear_color() const = 0;

		virtual bool get_cull_enabled() const = 0;

		virtual uint32_t get_cull_face() const = 0;

		virtual uint32_t get_cull_order() const = 0;
		
		virtual bool get_depth_enabled() const = 0;

		virtual uint32_t get_depth_function() const = 0;
		
		virtual bool get_depth_mask() const = 0;

		virtual depth_range get_depth_range() const = 0;

		virtual int_rect get_viewport() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_alpha_enabled(bool enabled) = 0;

		virtual void set_alpha_function(alpha_function const & value) = 0;
		
		virtual void set_blend_color(color const & value) = 0;

		virtual void set_blend_enabled(bool enabled) = 0;
		
		virtual void set_blend_equation(blend_equation const & value) = 0;
		
		virtual void set_blend_function(blend_function const & value) = 0;
		
		virtual void set_clear_color(color const & value) = 0;
		
		virtual void set_cull_enabled(bool enabled) = 0;

		virtual void set_cull_face(uint32_t facet) = 0;

		virtual void set_cull_order(uint32_t order) = 0;
		
		virtual void set_depth_enabled(bool enabled) = 0;

		virtual void set_depth_function(uint32_t predicate) = 0;
		
		virtual void set_depth_mask(bool enabled) = 0;

		virtual void set_depth_range(depth_range const & value) = 0;

		virtual void set_viewport(int_rect const & bounds) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void clear(uint32_t flags) = 0;

		virtual void draw(shared<vertex_array> const & value) = 0;
		
		virtual void draw_arrays(uint32_t primitive, uint32_t first, uint32_t count) = 0;
		
		virtual void draw_indexed(uint32_t primitive, int32_t first, uint32_t index_type, buffer indices) = 0;
		
		virtual void flush() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// commands
namespace ml::gl
{
	// render command generators
	class render_command final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static command set_alpha_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_alpha_enabled, render_api::get_instance(), enabled);
		}

		ML_NODISCARD static command set_alpha_function(alpha_function const & value) noexcept
		{
			return std::bind(&render_api::set_alpha_function, render_api::get_instance(), value);
		}

		ML_NODISCARD static command set_blend_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_blend_enabled, render_api::get_instance(), enabled);
		}
		
		ML_NODISCARD static command set_blend_color(color const & value) noexcept
		{
			return std::bind(&render_api::set_blend_color, render_api::get_instance(), value);
		}

		ML_NODISCARD static command set_blend_equation(blend_equation const & value) noexcept
		{
			return std::bind(&render_api::set_blend_equation, render_api::get_instance(), value);
		}

		ML_NODISCARD static command set_blend_function(blend_function const & value) noexcept
		{
			return std::bind(&render_api::set_blend_function, render_api::get_instance(), value);
		}

		ML_NODISCARD static command set_clear_color(color const & value) noexcept
		{
			return std::bind(&render_api::set_clear_color, render_api::get_instance(), value);
		}

		ML_NODISCARD static command set_cull_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_cull_enabled, render_api::get_instance(), enabled);
		}

		ML_NODISCARD static command set_cull_face(uint32_t facet) noexcept
		{
			return std::bind(&render_api::set_cull_face, render_api::get_instance(), facet);
		}

		ML_NODISCARD static command set_cull_order(uint32_t order) noexcept
		{
			return std::bind(&render_api::set_cull_order, render_api::get_instance(), order);
		}

		ML_NODISCARD static command set_depth_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_depth_enabled, render_api::get_instance(), enabled);
		}

		ML_NODISCARD static command set_depth_function(uint32_t predicate) noexcept
		{
			return std::bind(&render_api::set_depth_function, render_api::get_instance(), predicate);
		}
		
		ML_NODISCARD static command set_depth_mask(bool enabled) noexcept
		{
			return std::bind(&render_api::set_depth_mask, render_api::get_instance(), enabled);
		}

		ML_NODISCARD static command set_depth_range(depth_range const & value) noexcept
		{
			return std::bind(&render_api::set_depth_range, render_api::get_instance(), value);
		}

		ML_NODISCARD static command set_viewport(int_rect const & bounds) noexcept
		{
			return std::bind(&render_api::set_viewport, render_api::get_instance(), bounds);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static command clear(uint32_t flags) noexcept
		{
			return std::bind(&render_api::clear, render_api::get_instance(), flags);
		}

		ML_NODISCARD static command draw(shared<vertex_array> const & value) noexcept
		{
			return std::bind(&render_api::draw, render_api::get_instance(), value);
		}

		ML_NODISCARD static command draw_arrays(uint32_t primitive, uint32_t first, uint32_t count) noexcept
		{
			return std::bind(&render_api::draw_arrays, render_api::get_instance(), primitive, first, count);
		}

		ML_NODISCARD static command draw_indexed(uint32_t primitive, int32_t first, uint32_t index_type, buffer indices) noexcept
		{
			return std::bind(&render_api::draw_indexed, render_api::get_instance(), primitive, first, index_type, indices);
		}

		ML_NODISCARD static command flush() noexcept
		{
			return std::bind(&render_api::flush, render_api::get_instance());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RENDERER_API_HPP_