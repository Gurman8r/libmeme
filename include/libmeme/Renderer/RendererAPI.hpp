#ifndef _ML_RENDERER_API_HPP_
#define _ML_RENDERER_API_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Color.hpp>
#include <libmeme/Core/Rect.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// enums
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct to_impl : std::bool_constant<false> {};	// convert backend enum -> user enum
	struct to_user : std::bool_constant<true> {};	// convert user enum -> backend enum

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
		error_none,
		error_invalid_enum,
		error_invalid_value,
		error_invalid_operation,
		error_stack_overflow,
		error_stack_underflow,
		error_out_of_memory,
		error_invalid_framebuffer_operation,
	};

	enum usage_ : uint32_t
	{
		usage_stream_draw,
		usage_static_draw,
		usage_dynamic_draw,
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

	enum cull_order_
	{
		cull_order_cw,
		cull_order_ccw,
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
	// base render api
	class ML_RENDERER_API render_api : public trackable, public non_copyable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_api() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool initialize() = 0;

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

		virtual bool get_multisample_enabled() const = 0;

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

		virtual void set_cull_facet(uint32_t facet) = 0;

		virtual void set_cull_order(uint32_t front_face) = 0;
		
		virtual void set_depth_enabled(bool enabled) = 0;

		virtual void set_depth_function(uint32_t predicate) = 0;
		
		virtual void set_depth_mask(bool enabled) = 0;

		virtual void set_depth_range(depth_range const & value) = 0;

		virtual void set_multisample_enabled(bool enabled) = 0;

		virtual void set_viewport(int_rect const & bounds) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void clear(uint32_t flags) = 0;
		
		virtual void draw_arrays(uint32_t primitive, uint32_t first, uint32_t count) = 0;
		
		virtual void draw_indexed(uint32_t primitive, int32_t first, uint32_t index_type, void const * indices) = 0;
		
		virtual void flush() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// context
namespace ml::gl
{
	// global render context
	class ML_RENDERER_API render_context final : public singleton<render_context>
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static std::unique_ptr<render_api> const & api() noexcept
		{
			return get_instance().m_api;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<render_context>;

		render_context() noexcept;
		
		~render_context() noexcept;

		std::unique_ptr<render_api> m_api;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// resources
namespace ml::gl
{
	// base graphics resource
	class ML_RENDERER_API graphics_resource : public trackable, public non_copyable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~graphics_resource() = default;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		ML_NODISCARD virtual void * get_handle() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// base vertex array
	class ML_RENDERER_API vertex_array : public graphics_resource
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~vertex_array() = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static vertex_array * create(uint32_t mode);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(vertex_array const * value);

		inline void bind() const override { bind(this); }

		inline void unbind() const override { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual void * get_handle() const = 0;

		ML_NODISCARD virtual uint32_t get_mode() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// base vertex buffer
	class ML_RENDERER_API vertex_buffer : public graphics_resource
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~vertex_buffer() = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static vertex_buffer * create(float_t const * vertices, uint32_t size);

		ML_NODISCARD static vertex_buffer * create(float_t const * vertices, uint32_t size, uint32_t offset);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(vertex_buffer const * value);

		inline void bind() const override { bind(this); }

		inline void unbind() const override { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_data(float_t const * vertices, uint32_t size) = 0;

		virtual void set_sub_data(float_t const * vertices, uint32_t size, uint32_t offset) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual void * get_handle() const = 0;

		ML_NODISCARD virtual uint32_t get_size() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// base index buffer
	class ML_RENDERER_API index_buffer : public graphics_resource
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~index_buffer() = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static index_buffer * create(uint32_t const * indices, uint32_t count);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(index_buffer const * value);

		inline void bind() const override { bind(this); }

		inline void unbind() const override { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_data(uint32_t const * indices, uint32_t count) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual void * get_handle() const = 0;

		ML_NODISCARD virtual uint32_t get_count() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// base frame buffer
	class ML_RENDERER_API frame_buffer : public graphics_resource
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~frame_buffer() = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static frame_buffer * create();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(frame_buffer const * value);

		inline void bind() const override { bind(this); }

		inline void unbind() const override { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_render_buffer(void const * value, uint32_t attachment) = 0;

		virtual void set_texture2d(void const * value, uint32_t attachment, uint32_t level) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual void * get_handle() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// base render buffer
	class ML_RENDERER_API render_buffer : public graphics_resource
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_buffer() = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static render_buffer * create(uint32_t format, vec2i const & resolution);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(render_buffer const * value);

		inline void bind() const override { bind(this); }

		inline void unbind() const override { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_storage(uint32_t format, vec2i const & size) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual void * get_handle() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// base shader
	class ML_RENDERER_API shader_object : public graphics_resource
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~shader_object() = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static shader_object * create();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(shader_object const * value);

		inline void bind() const override { bind(this); }

		inline void unbind() const override { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual void * get_handle() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// base texture
	class ML_RENDERER_API texture_object : public graphics_resource
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~texture_object() = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static texture_object * create();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(texture_object const * value);

		inline void bind() const override { bind(this); }

		inline void unbind() const override { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual void * get_handle() const = 0;

		ML_NODISCARD virtual uint32_t get_type() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// commands
namespace ml::gl
{
	// render command type
	ML_alias render_command = typename std::function<void()>;

	// render command factory
	class ML_NODISCARD command final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static render_command set_alpha_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_alpha_enabled, render_context::api().get(), enabled);
		}

		static render_command set_alpha_function(alpha_function const & value) noexcept
		{
			return std::bind(&render_api::set_alpha_function, render_context::api().get(), value);
		}

		static render_command set_blend_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_blend_enabled, render_context::api().get(), enabled);
		}
		
		static render_command set_blend_color(color const & value) noexcept
		{
			return std::bind(&render_api::set_blend_color, render_context::api().get(), value);
		}

		static render_command set_blend_equation(blend_equation const & value) noexcept
		{
			return std::bind(&render_api::set_blend_equation, render_context::api().get(), value);
		}

		static render_command set_blend_function(blend_function const & value) noexcept
		{
			return std::bind(&render_api::set_blend_function, render_context::api().get(), value);
		}

		static render_command set_clear_color(color const & value) noexcept
		{
			return std::bind(&render_api::set_clear_color, render_context::api().get(), value);
		}

		static render_command set_cull_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_cull_enabled, render_context::api().get(), enabled);
		}

		static render_command set_cull_facet(uint32_t facet) noexcept
		{
			return std::bind(&render_api::set_cull_facet, render_context::api().get(), facet);
		}

		static render_command set_cull_order(uint32_t order) noexcept
		{
			return std::bind(&render_api::set_cull_order, render_context::api().get(), order);
		}

		static render_command set_depth_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_depth_enabled, render_context::api().get(), enabled);
		}

		static render_command set_depth_function(uint32_t predicate) noexcept
		{
			return std::bind(&render_api::set_depth_function, render_context::api().get(), predicate);
		}
		
		static render_command set_depth_mask(bool enabled) noexcept
		{
			return std::bind(&render_api::set_depth_mask, render_context::api().get(), enabled);
		}

		static render_command set_depth_range(depth_range const & value) noexcept
		{
			return std::bind(&render_api::set_depth_range, render_context::api().get(), value);
		}

		static render_command set_multisample_enabled(bool enabled) noexcept
		{
			return std::bind(&render_api::set_multisample_enabled, render_context::api().get(), enabled);
		}

		static render_command set_viewport(int_rect const & bounds) noexcept
		{
			return std::bind(&render_api::set_viewport, render_context::api().get(), bounds);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static render_command clear(uint32_t flags) noexcept
		{
			return std::bind(&render_api::clear, render_context::api().get(), flags);
		}

		static render_command draw_arrays(uint32_t primitive, uint32_t first, uint32_t count) noexcept
		{
			return std::bind(&render_api::draw_arrays, render_context::api().get(), primitive, first, count);
		}

		static render_command draw_indexed(uint32_t primitive, int32_t first, uint32_t index_type, void const * indices) noexcept
		{
			return std::bind(&render_api::draw_indexed, render_context::api().get(), primitive, first, index_type, indices);
		}

		static render_command flush() noexcept
		{
			return std::bind(&render_api::flush, render_context::api().get());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RENDERER_API_HPP_