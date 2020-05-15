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
	enum gl_ : uint32_t // <- uncategorized
	{
		gl_element_array_buffer,
		gl_array_buffer_binding,
		gl_read_framebuffer,
		gl_draw_framebuffer,
		gl_read_framebuffer_binding,
		gl_draw_framebuffer_binding,
		gl_current_program,
		gl_vertex_array_binding,
		gl_texture_binding_2d,
		gl_sampler_binding,
		gl_active_texture,
		gl_polygon_mode,
		gl_viewport,
		gl_scissor_box,
	};

	enum backend_ : uint32_t
	{
		backend_vendor,
		backend_renderer,
		backend_version,
		backend_major_version,
		backend_minor_version,
		backend_extensions,
		backend_num_extensions,
		backend_shading_language_version,
		backend_info_log_length,
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
		capability_texture_cube_map_seamless,
	};

	enum clear_flags_ : uint32_t
	{
		clear_flags_none,
		clear_flags_depth	= 1 << 0,
		clear_flags_accum	= 1 << 1,
		clear_flags_stencil = 1 << 2,
		clear_flags_color	= 1 << 3,
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

	enum index_type_ : uint32_t
	{
		index_type_byte,
		index_type_unsigned_byte,
		index_type_short,
		index_type_unsigned_short,
		index_type_int,
		index_type_unsigned_int,
		index_type_float,
		index_type_half_float,
	};

	enum equation_ : uint32_t
	{
		equation_mult,
		equation_add,
		equation_subtract,
		equation_func_add,
	};

	enum blend_mode_
	{
		blend_mode_equation_rgb,
		blend_mode_equation_alpha,
		blend_mode_dst_rgb,
		blend_mode_src_rgb,
		blend_mode_dst_alpha,
		blend_mode_src_alpha,
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

	enum storage_ : uint32_t
	{
		storage_unpack_swap_bytes,
		storage_unpack_lsb_first,
		storage_unpack_row_length,
		storage_unpack_skip_rows,
		storage_unpack_skip_pixels,
		storage_unpack_alignment,

		storage_pack_swap_bytes,
		storage_pack_lsb_first,
		storage_pack_row_length,
		storage_pack_skip_rows,
		storage_pack_skip_pixels,
		storage_pack_alignment,
	};

	enum origin_ : uint32_t
	{
		gl_lower_left,
		gl_upper_left,
		gl_clip_origin,
	};

	enum texture_type_ : uint32_t
	{
		texture_type_2d,
		texture_type_3d,
		texture_type_cube_map,
	};

	enum texture_attr_ : uint32_t
	{
		texture_attr_max_size,
		texture_attr_max_combined_image_units,
	};

	enum texture_param_ : uint32_t
	{
		texture_param_nearest,
		texture_param_linear,
		texture_param_nearest_mipmap_nearest,
		texture_param_linear_mipmap_nearest,
		texture_param_nearest_mipmap_linear,
		texture_param_linear_mipmap_linear,
		texture_param_texture_mag_filter,
		texture_param_texture_min_filter,
		texture_param_texture_wrap_s,
		texture_param_texture_wrap_t,
		texture_param_texture_wrap_r,
		texture_param_clamp,
		texture_param_repeat,
		texture_param_clamp_to_edge,
		texture_param_texture_min_lod,
		texture_param_texture_max_lod,
		texture_param_texture_base_level,
		texture_param_texture_max_level,
	};

	enum texture_cube_map_ : uint32_t
	{
		texture_cube_map_positive_x,
		texture_cube_map_negative_x,
		texture_cube_map_positive_y,
		texture_cube_map_negative_y,
		texture_cube_map_positive_z,
		texture_cube_map_negative_z,
	};

	enum shader_attr_ : uint32_t
	{
		shader_attr_compile_status,
		shader_attr_delete_status,
		shader_attr_link_status,
		shader_attr_program_object,
	};

	enum shader_type_ : uint32_t
	{
		shader_type_vertex,
		shader_type_fragment,
		shader_type_geometry,
	};

	enum framebuffer_attr_
	{
		framebuffer_attr_complete,
	};

	enum framebuffer_attachment_ : uint32_t
	{
		framebuffer_attachment_default,
		framebuffer_attachment_undefined,
		framebuffer_attachment_depth_stencil,

		framebuffer_attachment_color_encoding,
		framebuffer_attachment_component_type,
		framebuffer_attachment_red_size,
		framebuffer_attachment_green_size,
		framebuffer_attachment_blue_size,
		framebuffer_attachment_alpha_size,
		framebuffer_attachment_depth_size,
		framebuffer_attachment_stencil_size,
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

		ML_NODISCARD virtual void const * get_handle() const noexcept = 0;

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

		ML_NODISCARD virtual void const * get_handle() const noexcept = 0;

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

		ML_NODISCARD virtual void const * get_handle() const noexcept = 0;

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

		ML_NODISCARD virtual void const * get_handle() const noexcept = 0;

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

		ML_NODISCARD virtual void const * get_handle() const noexcept = 0;

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

// render api
namespace ml
{
	struct ML_RENDERER_API render_api : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_api() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool initialize() = 0;
		virtual uint32_t get_error() = 0;

		virtual void clear(uint32_t flags) = 0;
		virtual void flush() = 0;

		virtual void draw_arrays(uint32_t mode, uint32_t first, uint32_t count) = 0;
		virtual void draw_indexed(uint32_t mode, int32_t first, uint32_t type, void const * indices) = 0;

		virtual void set_active_texture(void const * value) = 0;
		virtual void set_alpha_function(uint32_t value, float32_t ref) = 0;
		virtual void set_blend_equation(uint32_t modeRGB, uint32_t modeAlpha) = 0;
		virtual void set_blend_function(uint32_t sfactorRGB, uint32_t dfactorRGB, uint32_t sfactorAlpha, uint32_t dfactorAlpha) = 0;
		virtual void set_clear_color(color const & value) = 0;
		virtual void set_cull_mode(uint32_t value) = 0;
		virtual void set_depth_function(uint32_t value) = 0;
		virtual void set_depth_mask(bool value) = 0;
		virtual void set_enabled(uint32_t value, bool enabled) = 0;
		virtual void set_polygon_mode(uint32_t face, uint32_t mode) = 0;
		virtual void set_viewport(int_rect const & value) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// render commands
namespace ml
{
	class ML_RENDERER_API ML_NODISCARD render_command final : public singleton<render_command>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton<render_command>;

		render_command() noexcept;

		~render_command() noexcept;

		std::unique_ptr<render_api> m_api;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		static std::unique_ptr<render_api> const & api() noexcept
		{
			return get_instance().m_api;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static auto clear(uint32_t flags) noexcept
		{
			return std::bind(&render_api::clear, api().get(), flags);
		}

		static auto flush() noexcept
		{
			return std::bind(&render_api::flush, api().get());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static auto draw_arrays(uint32_t mode, uint32_t first, uint32_t count) noexcept
		{
			return std::bind(&render_api::draw_arrays, api().get(), mode, first, count);
		}

		static auto draw_indexed(uint32_t mode, int32_t first, uint32_t type, void const * indices) noexcept
		{
			return std::bind(&render_api::draw_indexed, api().get(), mode, first, type, indices);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static auto set_active_texture(void const * value) noexcept
		{
			return std::bind(&render_api::set_active_texture, api().get(), value);
		}
		
		static auto set_alpha_function(uint32_t value, float32_t ref) noexcept
		{
			return std::bind(&render_api::set_alpha_function, api().get(), value, ref);
		}
		
		static auto set_blend_equation(uint32_t modeRGB, uint32_t modeAlpha) noexcept
		{
			return std::bind(&render_api::set_blend_equation, api().get(), modeRGB, modeAlpha);
		}

		static auto set_blend_function(uint32_t sfactorRGB, uint32_t dfactorRGB, uint32_t sfactorAlpha, uint32_t dfactorAlpha) noexcept
		{
			return std::bind(&render_api::set_blend_function, api().get(), sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
		}
		
		static auto set_clear_color(color const & value) noexcept
		{
			return std::bind(&render_api::set_clear_color, api().get(), value);
		}

		static auto set_cull_mode(uint32_t value) noexcept
		{
			return std::bind(&render_api::set_cull_mode, api().get(), value);
		}

		static auto set_depth_function(uint32_t value) noexcept
		{
			return std::bind(&render_api::set_depth_function, api().get(), value);
		}
		
		static auto set_depth_mask(bool value) noexcept
		{
			return std::bind(&render_api::set_depth_mask, api().get(), value);
		}

		static auto set_enabled(uint32_t value, bool enabled) noexcept
		{
			return std::bind(&render_api::set_enabled, api().get(), value, enabled);
		}
		
		static auto set_polygon_mode(uint32_t face, uint32_t mode) noexcept
		{
			return std::bind(&render_api::set_polygon_mode, api().get(), face, mode);
		}

		static auto set_viewport(int_rect const & value) noexcept
		{
			return std::bind(&render_api::set_viewport, api().get(), value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RENDERER_API_HPP_