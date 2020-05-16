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
	enum capability_ : uint32_t
	{
		capability_alpha_test,
		capability_blend,
		capability_cull_face,
		capability_depth_clamp,
		capability_depth_test,
		capability_dither,
		capability_framebuffer_srgb,
		capability_line_smooth,
		capability_multisample,
		capability_polygon_smooth,
		capability_sample_mask,
		capability_scissor_test,
		capability_stencil_test,
		capability_texture_1d,
		capability_texture_2d,
		capability_texture_3d,
		capability_texture_cube_map,
		capability_texture_cube_map_seamless,
	};

	enum error_type_ : uint32_t
	{
		error_type_none,
		error_type_invalid_enum,
		error_type_invalid_value,
		error_type_invalid_operation,
		error_type_stack_overflow,
		error_type_stack_underflow,
		error_type_out_of_memory,
		error_type_invalid_framebuffer_operation,
	};

	enum clear_flags_ : uint32_t
	{
		clear_flags_none,
		clear_flags_accum	= 1 << 0,
		clear_flags_color	= 1 << 1,
		clear_flags_depth	= 1 << 2,
		clear_flags_stencil = 1 << 3,
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

	enum front_face_
	{
		front_face_cw,
		front_face_ccw,
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

	enum color_attachment_ : uint32_t
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
	};

	enum draw_buffer_ : uint32_t
	{
		draw_buffer_0,
		draw_buffer_1,
		draw_buffer_2,
		draw_buffer_3,
		draw_buffer_4,
		draw_buffer_5,
		draw_buffer_6,
		draw_buffer_7,
		draw_buffer_8,
		draw_buffer_9,
		draw_buffer_10,
		draw_buffer_11,
		draw_buffer_12,
		draw_buffer_13,
		draw_buffer_14,
		draw_buffer_15,
	};

	enum texture_attachment_ : uint32_t
	{
		texture_attachment_0,
		texture_attachment_1,
		texture_attachment_2,
		texture_attachment_3,
		texture_attachment_4,
		texture_attachment_5,
		texture_attachment_6,
		texture_attachment_7,
		texture_attachment_8,
		texture_attachment_9,
		texture_attachment_10,
		texture_attachment_11,
		texture_attachment_12,
		texture_attachment_13,
		texture_attachment_14,
		texture_attachment_15,
		texture_attachment_16,
		texture_attachment_17,
		texture_attachment_18,
		texture_attachment_19,
		texture_attachment_20,
		texture_attachment_21,
		texture_attachment_22,
		texture_attachment_23,
		texture_attachment_24,
		texture_attachment_25,
		texture_attachment_26,
		texture_attachment_27,
		texture_attachment_28,
		texture_attachment_29,
		texture_attachment_30,
		texture_attachment_31,
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// buffers
namespace ml
{
	// vertex array base
	struct ML_RENDERER_API vertex_array : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~vertex_array() = default;

		ML_NODISCARD virtual void * get_handle() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_mode() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(vertex_array const * value) noexcept;

		void bind() const noexcept { bind(this); }

		void unbind() const noexcept { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static vertex_array * create(uint32_t mode) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// vertex buffer base
	struct ML_RENDERER_API vertex_buffer : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~vertex_buffer() = default;

		virtual void set_data(float_t const * vertices, uint32_t size) = 0;

		virtual void set_sub_data(float_t const * vertices, uint32_t size, uint32_t offset) = 0;

		ML_NODISCARD virtual void * get_handle() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_size() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(vertex_buffer const * value) noexcept;

		void bind() const noexcept { bind(this); }

		void unbind() const noexcept { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static vertex_buffer * create(float_t const * vertices, uint32_t size) noexcept;

		ML_NODISCARD static vertex_buffer * create(float_t const * vertices, uint32_t size, uint32_t offset) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// index buffer base
	struct ML_RENDERER_API index_buffer : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~index_buffer() = default;

		virtual void set_data(uint32_t const * indices, uint32_t count) = 0;

		ML_NODISCARD virtual void * get_handle() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_count() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(index_buffer const * value) noexcept;

		void bind() const noexcept { bind(this); }

		void unbind() const noexcept { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static index_buffer * create(uint32_t const * indices, uint32_t count) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// frame buffer base
	struct ML_RENDERER_API frame_buffer : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~frame_buffer() = default;

		virtual void set_render_buffer(void const * value, uint32_t attachment) = 0;

		virtual void set_texture2d(void const * value, uint32_t attachment, uint32_t level) = 0;

		ML_NODISCARD virtual void * get_handle() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(frame_buffer const * value) noexcept;

		void bind() const noexcept { bind(this); }

		void unbind() const noexcept { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static frame_buffer * create() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// render buffer base
	struct ML_RENDERER_API render_buffer : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_buffer() = default;

		virtual void set_storage(uint32_t format, int32_t width, int32_t height) = 0;

		ML_NODISCARD virtual void * get_handle() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(render_buffer const * value) noexcept;

		void bind() const noexcept { bind(this); }

		void unbind() const noexcept { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static render_buffer * create(uint32_t format, int32_t width, int32_t height) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
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
namespace ml
{
	struct ML_RENDERER_API render_api : trackable, non_copyable
	{
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

		virtual gl::alpha_function get_alpha_function() const = 0;
		
		virtual color get_blend_color() const = 0;
		
		virtual gl::blend_equation get_blend_equation() const = 0;
		
		virtual gl::blend_function get_blend_function() const = 0;
		
		virtual color get_clear_color() const = 0;
		
		virtual uint32_t get_depth_function() const = 0;
		
		virtual bool get_depth_mask() const = 0;

		virtual gl::depth_range get_depth_range() const = 0;
		
		virtual bool get_enabled(uint32_t capability) const = 0;
		
		virtual uint32_t get_front_face() const = 0;
		
		virtual int_rect get_viewport() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_alpha_function(gl::alpha_function const & value) = 0;
		
		virtual void set_blend_color(color const & value) = 0;
		
		virtual void set_blend_equation(gl::blend_equation const & value) = 0;
		
		virtual void set_blend_function(gl::blend_function const & value) = 0;
		
		virtual void set_clear_color(color const & value) = 0;
		
		virtual void set_cull_mode(uint32_t facet) = 0;
		
		virtual void set_depth_function(uint32_t predicate) = 0;
		
		virtual void set_depth_mask(bool enabled) = 0;

		virtual void set_depth_range(gl::depth_range const & value) = 0;
		
		virtual void set_enabled(uint32_t capability, bool enabled) = 0;
		
		virtual void set_front_face(uint32_t front_face) = 0;
		
		virtual void set_viewport(int_rect const & bounds) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void clear(uint32_t flags) = 0;
		
		virtual void draw_arrays(uint32_t primitive, uint32_t first, uint32_t count) = 0;
		
		virtual void draw_indexed(uint32_t primitive, int32_t first, uint32_t type, void const * indices) = 0;
		
		virtual void flush() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// context
namespace ml
{
	// global render context
	struct ML_RENDERER_API render_context final : singleton<render_context>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static std::unique_ptr<render_api> const & api() noexcept
		{
			return get_instance().m_api;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<render_context>;

		render_context() noexcept;

		inline ~render_context() noexcept {}

		std::unique_ptr<render_api> m_api;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// commands
namespace ml
{
	// render command type
	namespace gl { ML_alias command = std::function<void()>; }

	// render comand factory
	class ML_NODISCARD render_command final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static auto const & api() noexcept { return render_context::api(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static gl::command set_alpha_function(gl::alpha_function const & value) noexcept
		{
			return std::bind(&render_api::set_alpha_function, api().get(), value);
		}
		
		static gl::command set_blend_color(color const & value) noexcept
		{
			return std::bind(&render_api::set_blend_color, api().get(), value);
		}

		static gl::command set_blend_equation(gl::blend_equation const & value) noexcept
		{
			return std::bind(&render_api::set_blend_equation, api().get(), value);
		}

		static gl::command set_blend_function(gl::blend_function const & value) noexcept
		{
			return std::bind(&render_api::set_blend_function, api().get(), value);
		}

		static gl::command set_clear_color(color const & value) noexcept
		{
			return std::bind(&render_api::set_clear_color, api().get(), value);
		}

		static gl::command set_cull_mode(uint32_t facet) noexcept
		{
			return std::bind(&render_api::set_cull_mode, api().get(), facet);
		}

		static gl::command set_depth_function(uint32_t predicate) noexcept
		{
			return std::bind(&render_api::set_depth_function, api().get(), predicate);
		}
		
		static gl::command set_depth_mask(bool enabled) noexcept
		{
			return std::bind(&render_api::set_depth_mask, api().get(), enabled);
		}

		static gl::command set_depth_range(gl::depth_range const & value) noexcept
		{
			return std::bind(&render_api::set_depth_range, api().get(), value);
		}

		static gl::command set_enabled(uint32_t capability, bool enabled) noexcept
		{
			return std::bind(&render_api::set_enabled, api().get(), capability, enabled);
		}

		static gl::command set_front_face(uint32_t front_face) noexcept
		{
			return std::bind(&render_api::set_front_face, api().get(), front_face);
		}
		
		static gl::command set_viewport(int_rect const & bounds) noexcept
		{
			return std::bind(&render_api::set_viewport, api().get(), bounds);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static gl::command clear(uint32_t flags) noexcept
		{
			return std::bind(&render_api::clear, api().get(), flags);
		}

		static gl::command draw_arrays(uint32_t primitive, uint32_t first, uint32_t count) noexcept
		{
			return std::bind(&render_api::draw_arrays, api().get(), primitive, first, count);
		}

		static gl::command draw_indexed(uint32_t primitive, int32_t first, uint32_t type, void const * indices) noexcept
		{
			return std::bind(&render_api::draw_indexed, api().get(), primitive, first, type, indices);
		}

		static gl::command flush() noexcept
		{
			return std::bind(&render_api::flush, api().get());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RENDERER_API_HPP_