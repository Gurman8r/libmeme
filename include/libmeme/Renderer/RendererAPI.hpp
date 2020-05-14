#ifndef _ML_RENDERER_API_HPP_
#define _ML_RENDERER_API_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Color.hpp>
#include <libmeme/Core/Rect.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// enums
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum gl_ : uint32_t
	{
		gl_vendor,
		gl_renderer,
		gl_version,
		gl_major_version,
		gl_minor_version,
		gl_extensions,
		gl_num_extensions,

		gl_no_error,
		gl_invalid_enum,
		gl_invalid_value,
		gl_invalid_operation,
		gl_stack_overflow,
		gl_stack_underflow,
		gl_out_of_memory,
		gl_invalid_framebuffer_operation,

		gl_stream_draw,
		gl_static_draw,
		gl_dynamic_draw,

		gl_array_buffer,
		gl_element_array_buffer,
		gl_array_buffer_binding,
		gl_framebuffer,
		gl_renderbuffer,
		gl_program_object,
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

		gl_cull_face,
		gl_depth_test,
		gl_alpha_test,
		gl_blend,
		gl_multisample,
		gl_framebuffer_srgb,
		gl_scissor_test,

		gl_blend_equation_rgb,
		gl_blend_equation_alpha,
		gl_blend_dst_rgb,
		gl_blend_src_rgb,
		gl_blend_dst_alpha,
		gl_blend_src_alpha,

		gl_lower_left,
		gl_upper_left,
		gl_clip_origin,

		gl_points,
		gl_lines,
		gl_line_loop,
		gl_line_strip,
		gl_triangles,
		gl_triangle_strip,
		gl_triangle_fan,
		gl_fill,

		gl_mult,
		gl_add,
		gl_subtract,
		gl_func_add,

		gl_never,
		gl_less,
		gl_equal,
		gl_lequal,
		gl_greater,
		gl_notequal,
		gl_gequal,
		gl_always,

		gl_zero,
		gl_one,
		gl_src_color,
		gl_one_minus_src_color,
		gl_src_alpha,
		gl_one_minus_src_alpha,
		gl_dst_alpha,
		gl_one_minus_dst_alpha,
		gl_dst_color,
		gl_one_minus_dst_color,
		gl_src_alpha_saturate,

		gl_front_left,
		gl_front_right,
		gl_back_left,
		gl_back_right,
		gl_front,
		gl_back,
		gl_left,
		gl_right,
		gl_front_and_back,

		gl_byte,
		gl_unsigned_byte,
		gl_short,
		gl_unsigned_short,
		gl_int,
		gl_unsigned_int,
		gl_float,
		gl_half_float,

		gl_red,
		gl_green,
		gl_blue,
		gl_alpha,
		gl_rgb,
		gl_rgba,
		gl_luminance,
		gl_luminance_alpha,
		gl_srgb,
		gl_srgb8,
		gl_srgb_alpha,
		gl_srgb8_alpha8,
		gl_sluminance_alpha,
		gl_sluminance8_alpha8,
		gl_sluminance,
		gl_sluminance8,
		gl_depth24_stencil8,

		gl_nearest,
		gl_linear,
		gl_nearest_mipmap_nearest,
		gl_linear_mipmap_nearest,
		gl_nearest_mipmap_linear,
		gl_linear_mipmap_linear,
		gl_texture_mag_filter,
		gl_texture_min_filter,
		gl_texture_wrap_s,
		gl_texture_wrap_t,
		gl_texture_wrap_r,
		gl_clamp,
		gl_repeat,
		gl_clamp_to_edge,
		gl_texture_min_lod,
		gl_texture_max_lod,
		gl_texture_base_level,
		gl_texture_max_level,

		gl_texture_2d,
		gl_texture_3d,
		gl_texture_cube_map,
		gl_max_texture_size,
		gl_max_combined_texture_image_units,

		gl_texture_cube_map_positive_x,
		gl_texture_cube_map_negative_x,
		gl_texture_cube_map_positive_y,
		gl_texture_cube_map_negative_y,
		gl_texture_cube_map_positive_z,
		gl_texture_cube_map_negative_z,

		gl_unpack_swap_bytes,
		gl_unpack_lsb_first,
		gl_unpack_row_length,
		gl_unpack_skip_rows,
		gl_unpack_skip_pixels,
		gl_unpack_alignment,

		gl_pack_swap_bytes,
		gl_pack_lsb_first,
		gl_pack_row_length,
		gl_pack_skip_rows,
		gl_pack_skip_pixels,
		gl_pack_alignment,

		gl_fragment_shader,
		gl_vertex_shader,
		gl_geometry_shader,
		gl_shading_language_version,

		gl_object_delete_status,
		gl_object_compile_status,
		gl_object_link_status,
		gl_info_log_length,
		gl_framebuffer_complete,

		gl_framebuffer_attachment_color_encoding,
		gl_framebuffer_attachment_component_type,
		gl_framebuffer_attachment_red_size,
		gl_framebuffer_attachment_green_size,
		gl_framebuffer_attachment_blue_size,
		gl_framebuffer_attachment_alpha_size,
		gl_framebuffer_attachment_depth_size,
		gl_framebuffer_attachment_stencil_size,
		gl_framebuffer_default,
		gl_framebuffer_undefined,
		gl_depth_stencil_attachment,

		gl_color_attachment0,
		gl_color_attachment1,
		gl_color_attachment2,
		gl_color_attachment3,
		gl_color_attachment4,
		gl_color_attachment5,
		gl_color_attachment6,
		gl_color_attachment7,
		gl_color_attachment8,
		gl_color_attachment9,
		gl_color_attachment10,
		gl_color_attachment11,
		gl_color_attachment12,
		gl_color_attachment13,
		gl_color_attachment14,
		gl_color_attachment15,

		gl_draw_buffer0,
		gl_draw_buffer1,
		gl_draw_buffer2,
		gl_draw_buffer3,
		gl_draw_buffer4,
		gl_draw_buffer5,
		gl_draw_buffer6,
		gl_draw_buffer7,
		gl_draw_buffer8,
		gl_draw_buffer9,
		gl_draw_buffer10,
		gl_draw_buffer11,
		gl_draw_buffer12,
		gl_draw_buffer13,
		gl_draw_buffer14,
		gl_draw_buffer15,

		gl_texture0,
		gl_texture1,
		gl_texture2,
		gl_texture3,
		gl_texture4,
		gl_texture5,
		gl_texture6,
		gl_texture7,
		gl_texture8,
		gl_texture9,
		gl_texture10,
		gl_texture11,
		gl_texture12,
		gl_texture13,
		gl_texture14,
		gl_texture15,
		gl_texture16,
		gl_texture17,
		gl_texture18,
		gl_texture19,
		gl_texture20,
		gl_texture21,
		gl_texture22,
		gl_texture23,
		gl_texture24,
		gl_texture25,
		gl_texture26,
		gl_texture27,
		gl_texture28,
		gl_texture29,
		gl_texture30,
		gl_texture31,

		gl_context_flags,
		gl_context_profile_mask,
		gl_context_core_profile_bit,
		gl_context_compatability_profile_bit,
		gl_context_flag_debug_bit,

		gl_depth_buffer_bit,
		gl_stencil_buffer_bit,
		gl_color_buffer_bit,

		gl_MAX_ENUM
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias gl_enum_table = typename ds::array<uint32_t, gl_MAX_ENUM>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		ML_NODISCARD virtual void const * get_handle() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_mode() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(vertex_array const * value) noexcept;

		ML_NODISCARD static vertex_array * create(uint32_t mode) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// vertex buffer base
	struct ML_RENDERER_API vertex_buffer : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~vertex_buffer() = default;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual void set_data(float_t const * vertices, uint32_t size) = 0;

		virtual void set_sub_data(float_t const * vertices, uint32_t size, uint32_t offset) = 0;

		ML_NODISCARD virtual void const * get_handle() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_size() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(vertex_buffer const * value) noexcept;

		ML_NODISCARD static vertex_buffer * create(float_t const * vertices, uint32_t size) noexcept;

		ML_NODISCARD static vertex_buffer * create(float_t const * vertices, uint32_t size, uint32_t offset) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// index buffer base
	struct ML_RENDERER_API index_buffer : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~index_buffer() = default;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual void set_data(uint32_t const * indices, uint32_t count) = 0;

		ML_NODISCARD virtual void const * get_handle() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_count() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(index_buffer const * value) noexcept;

		ML_NODISCARD static index_buffer * create(uint32_t const * indices, uint32_t count) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// frame buffer base
	struct ML_RENDERER_API frame_buffer : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~frame_buffer() = default;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual void set_render_buffer(void const * value, uint32_t attachment) = 0;

		virtual void set_texture2d(void const * value, uint32_t attachment, uint32_t level) = 0;

		ML_NODISCARD virtual void const * get_handle() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(frame_buffer const * value) noexcept;

		ML_NODISCARD static frame_buffer * create() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// render buffer base
	struct ML_RENDERER_API render_buffer : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_buffer() = default;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual void set_storage(uint32_t format, int32_t width, int32_t height) = 0;

		ML_NODISCARD virtual void const * get_handle() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(render_buffer const * value) noexcept;

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

		virtual void clear(uint32_t flags) = 0;
		virtual void flush() = 0;

		virtual void set_clear_color(color const & value) = 0;
		virtual void set_viewport(int_rect const & value) = 0;

		virtual void draw_arrays(uint32_t mode, uint32_t first, uint32_t count) = 0;
		virtual void draw_indexed(uint32_t mode, int32_t first, uint32_t type, void * indices) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// render commands
namespace ml
{
	class ML_RENDERER_API render_command final : public singleton<render_command>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend singleton<render_command>;

		render_command() noexcept;

		~render_command() noexcept;

		std::unique_ptr<render_api> m_api;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		static auto set_clear_color(color const & value) noexcept
		{
			return std::bind(&render_api::set_clear_color, api().get(), value);
		}

		static auto set_viewport(int_rect const & value) noexcept
		{
			return std::bind(&render_api::set_viewport, api().get(), value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static auto draw_arrays(uint32_t mode, uint32_t first, uint32_t count) noexcept
		{
			return std::bind(&render_api::draw_arrays, api().get(), mode, first, count);
		}

		static auto draw_indexed(uint32_t mode, int32_t first, uint32_t type, void * indices) noexcept
		{
			return std::bind(&render_api::draw_indexed, api().get(), mode, first, type, indices);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RENDERER_API_HPP_