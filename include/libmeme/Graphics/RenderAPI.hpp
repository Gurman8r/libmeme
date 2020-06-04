#ifndef _ML_RENDER_API_HPP_
#define _ML_RENDER_API_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Graphics/Export.hpp>
#include <libmeme/Graphics/Binder.hpp>
#include <libmeme/Graphics/Image.hpp>
#include <libmeme/Graphics/Vertex.hpp>
#include <libmeme/Core/Color.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Rect.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// types
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// interfaces
	class device			;
	class resource			;
	class vertexarray		;
	class vertexbuffer		;
	class indexbuffer		;
	class framebuffer		;
	class texture2d			;
	class program			;
	class shader			;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// aliases
	ML_alias buffer_t	= typename void const *				; // generic buffer data type
	ML_alias cmd_t		= typename std::function<void()>	; // render command type
	ML_alias handle_t	= typename void *					; // generic handle type
	ML_alias view_t		= typename pmr::vector<byte_t>		; // local buffer data type

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// conversion helpers
	struct to_user : std::false_type {}; // ( false_type ) | backend -> frontend
	struct to_impl : std::true_type {}; // ( true_type ) | frontend -> backend

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// enums
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum buffer_bit_ : uint32_t
	{
		color_bit	= 1 << 0,
		depth_bit	= 1 << 1,
		stencil_bit	= 1 << 2,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
		error_context_lost,
	};

	constexpr cstring error_names[] = // error names
	{
		"no error",
		"invalid enum",
		"invalid value",
		"invalid operation",
		"stack overflow",
		"stack underflow",
		"out of memory",
		"invalid framebuffer operation",
		"context lost",
	};

	constexpr cstring error_descs[] = // error descriptions
	{
		"no description",
		"an unacceptable value has been specified for an enumerated argument",
		"a numeric argument is out of range",
		"the specified operation is not allowed in the current state",
		"this command would cause a stack overflow",
		"this command would cause a stack underflow",
		"there is not enough memory left to execute the command",
		"attempting to read from or write/render to a framebuffer that is not complete",
		"the context has been lost due to a graphics card reset",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum usage_ : uint32_t
	{
		usage_stream_draw,
		usage_static_draw,
		usage_dynamic_draw,
	};

	constexpr cstring usage_names[] =
	{
		"stream draw",
		"static draw",
		"dynamic draw",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum action_ : uint32_t
	{
		action_keep,
		action_zero,
		action_replace,
		action_inc,
		action_inc_wrap,
		action_dec,
		action_dec_wrap,
		action_invert,
	};

	constexpr cstring action_names[] =
	{
		"keep",
		"zero",
		"replace",
		"increment",
		"increment wrap",
		"decrement",
		"decrement wrap",
		"invert",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	constexpr cstring primitive_names[] =
	{
		"points",
		"lines",
		"line loop",
		"line strip",
		"triangles",
		"triangle strip",
		"triangle fan",
		"fill",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	constexpr cstring predicate_names[] =
	{
		"never",
		"less",
		"equal",
		"lequal",
		"greater",
		"not equal",
		"gequal",
		"always",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		type_unsigned_int_24_8,
	};

	constexpr cstring type_names[] =
	{
		"byte",
		"unsigned byte",
		"short",
		"unsigned short",
		"int",
		"unsigned int",
		"float",
		"half float",

		"unsigned int 24 8",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum function_ : uint32_t
	{
		function_add,
		function_subtract,
		function_reverse_subtract,
		function_min,
		function_max,
	};

	constexpr cstring function_names[] =
	{
		"add",
		"subtract",
		"reverse subtract",
		"min",
		"max",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum order_
	{
		order_cw,
		order_ccw,
	};

	constexpr cstring order_names[] =
	{
		"cw",
		"ccw",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	constexpr cstring facet_names[] =
	{
		"front left",
		"front right",
		"back left",
		"back right",
		"front",
		"back",
		"left",
		"right",
		"front and back",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	constexpr cstring factor_names[] =
	{
		"zero",
		"one",
		"src color",
		"one minus src color",
		"src alpha",
		"one minus src alpha",
		"dst alpha",
		"one minus dst alpha",
		"dst color",
		"one minus dst color",
		"src alpha saturate",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		format_depth_stencil,
		format_depth24_stencil8,
	};

	constexpr cstring format_names[] =
	{
		"red",
		"green",
		"blue",
		"alpha",
		"rgb",
		"rgba",
		"luminance",
		"luminance alpha",

		"srgb",
		"srgb8",
		"srgb alpha",
		"srgb8 alpha8",
		"sluminance alpha",
		"sluminance8 alpha8",
		"sluminance",
		"sluminance8",

		"depth stencil",
		"depth24 stencil8",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum shader_type_ : uint32_t
	{
		shader_type_vertex,
		shader_type_fragment,
		shader_type_geometry,

		shader_type_max,
	};

	constexpr cstring shader_names[] =
	{
		"vertex shader",
		"fragment shader",
		"geometry shader",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum texture_type_ : uint32_t
	{
		texture_type_1d,
		texture_type_2d,
		texture_type_3d,
		texture_type_cubemap,

		texture_type_max,
	};

	constexpr cstring texture_names[] =
	{
		"texture 1d",
		"texture 2d",
		"texture 3d",
		"texture cube map",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum texture_flags_ : int32_t
	{
		texture_flags_none		= 0,		// none
		texture_flags_smooth	= (1 << 0),	// smooth
		texture_flags_repeated	= (1 << 1),	// repeated
		texture_flags_mipmapped	= (1 << 2),	// mipmapped

		// smooth / repeated
		texture_flags_default
			= texture_flags_smooth
			| texture_flags_repeated,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// utility
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Elem
	> void copy_buffer(view_t & dst, buffer_t src, size_t count) noexcept
	{
		dst.resize(count * sizeof(Elem));

		std::copy((byte_t *)src, (byte_t *)src + (count * sizeof(Elem)), dst.begin());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD constexpr uint32_t get_image_format(size_t value) noexcept
	{
		switch (value)
		{
		default	: return 0;
		case 1	: return format_red;
		case 3	: return format_rgb;
		case 4	: return format_rgba;
		}
	}

	ML_NODISCARD constexpr size_t get_channel_count(uint32_t value) noexcept
	{
		switch (value)
		{
		default			: return 0;
		case format_red	: return 1;
		case format_rgb	: return 3;
		case format_rgba: return 4;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr hash_t get_element_base_type(hash_t type) noexcept
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

	constexpr uint32_t get_element_component_count(hash_t type) noexcept
	{
		switch (type)
		{
		default					: return 0;
		case hashof_v<bool>		:
		case hashof_v<int32_t>	:
		case hashof_v<float_t>	: return 1;
		case hashof_v<vec2i>	:
		case hashof_v<vec2f>	: return 2;
		case hashof_v<vec3i>	:
		case hashof_v<vec3f>	: return 3;
		case hashof_v<vec4i>	:
		case hashof_v<vec4f>	: return 4;
		case hashof_v<mat2i>	:
		case hashof_v<mat2f>	: return 2 * 2;
		case hashof_v<mat3i>	:
		case hashof_v<mat3f>	: return 3 * 3;
		case hashof_v<mat4i>	:
		case hashof_v<mat4f>	: return 4 * 4;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// buffer layout element
	struct ML_NODISCARD buffer_element final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		cstring		name		{};
		hash_t		type		{};
		uint32_t	size		{};
		bool		normalized	{};
		uint32_t	offset		{};

		auto get_base_type() const noexcept { return get_element_base_type(type); }

		auto get_component_count() const noexcept { return get_element_component_count(type); }

		constexpr buffer_element(cstring name, hash_t type, uint32_t size, bool normalized) noexcept
			: name{ name }, type{ type }, size{ size }, normalized{ normalized }, offset{}
		{
		}

		template <class T
		> constexpr buffer_element(meta::tag<T>, cstring name, bool normalized = 0) noexcept
			: buffer_element{ name, hashof_v<T>, sizeof(T), normalized }
		{
			static_assert(util::is_any_of_v<T,
				bool,
				int32_t, vec2i, vec3i, vec4i, mat2i, mat3i, mat4i,
				float_t, vec2f, vec3f, vec4f, mat2f, mat3f, mat4f
			>);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vertex buffer layout
	struct ML_NODISCARD layout final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using storage_type				= typename pmr::vector<buffer_element>;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr std::initializer_list<buffer_element> defaults
		{
			{ meta::tag_v<vec3>, "a_position"	},
			{ meta::tag_v<vec3>, "a_normal"		},
			{ meta::tag_v<vec2>, "a_texcoord"	},
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		layout(std::initializer_list<buffer_element> init = defaults) noexcept
			: m_elements{ init.begin(), init.end() }
		{
			uint32_t offset{};
			for (auto & e : m_elements)
			{
				e.offset = offset;
				offset += e.size;
				m_stride += e.size;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto elements() const noexcept -> storage_type const & { return m_elements; }

		ML_NODISCARD auto stride() const noexcept -> uint32_t { return m_stride; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		uint32_t		m_stride	{}; // stride
		storage_type	m_elements	{}; // elements

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// states
namespace ml::gl
{
	struct ML_NODISCARD alpha_fn final
	{
		uint32_t pred	{ predicate_greater };
		float_t	 ref	{ 0.001f };
	};

	struct ML_NODISCARD blend_eq final
	{
		uint32_t
			modeRGB		{ function_add },
			modeAlpha	{ modeRGB };
	};

	struct ML_NODISCARD blend_fn final
	{
		uint32_t
			sfactorRGB	{ factor_src_alpha },
			dfactorRGB	{ factor_one_minus_src_alpha },
			sfactorAlpha{ sfactorRGB },
			dfactorAlpha{ dfactorRGB };
	};

	struct ML_NODISCARD depth_range final
	{
		float_t
			nearVal		{ 0.f },
			farVal		{ 1.f };
	};

	struct ML_NODISCARD stencil_fn final
	{
		uint32_t pred	{ predicate_always };
		int32_t  ref	{ 0 };
		uint32_t mask	{ static_cast<uint32_t>(-1) };
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device
namespace ml::gl
{
	// base device
	class ML_GRAPHICS_API device : public trackable, public non_copyable
	{
	protected:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool do_initialize() = 0;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~device() noexcept override = default;

		ML_NODISCARD static unique<device> const & get() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_NODISCARD info final : public trackable
		{
			// version
			pmr::string renderer, vendor, version, shading_language_version;
			int32_t major_version, minor_version;
			pmr::vector<pmr::string> extensions;

			// textures
			bool texture_edge_clamp_available;
			uint32_t max_texture_slots;

			// shaders
			bool shaders_available;
			bool geometry_shaders_available;
			pmr::vector<uint32_t> shader_binary_formats;
		};

		ML_NODISCARD virtual device::info const & get_info() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual uint32_t get_error() const noexcept = 0;

		static void check_error(cstring file, size_t line, cstring expr) noexcept
		{
			if (auto const code{ device::get()->get_error() }; code != error_none)
			{
				std::cout
					<< "An internal graphics error occurred ( " << code << " ):\n"
					<< " " << fs::path{ file }.filename() << " (" << line << ")\n"
					<< " expression:\n"
					<< "  " << expr << "\n"
					<< " description:\n"
					<< "  " << error_names[code] << "\n"
					<< "  " << error_descs[code] << "\n"
					<< "\n";
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual bool get_alpha_enabled() const = 0;

		ML_NODISCARD virtual alpha_fn get_alpha_fn() const = 0;
		
		ML_NODISCARD virtual bool get_blend_enabled() const = 0;

		ML_NODISCARD virtual color get_blend_color() const = 0;
		
		ML_NODISCARD virtual blend_eq get_blend_eq() const = 0;
		
		ML_NODISCARD virtual blend_fn get_blend_fn() const = 0;
		
		ML_NODISCARD virtual color get_clear_color() const = 0;

		ML_NODISCARD virtual bool get_cull_enabled() const = 0;

		ML_NODISCARD virtual uint32_t get_cull_facet() const = 0;

		ML_NODISCARD virtual uint32_t get_cull_order() const = 0;
		
		ML_NODISCARD virtual bool get_depth_enabled() const = 0;

		ML_NODISCARD virtual bool get_depth_mask() const = 0;
		
		ML_NODISCARD virtual uint32_t get_depth_pr() const = 0;

		ML_NODISCARD virtual depth_range get_depth_range() const = 0;

		ML_NODISCARD virtual bool get_stencil_enabled() const = 0;

		ML_NODISCARD virtual stencil_fn get_stencil_fn() const = 0;

		ML_NODISCARD virtual int_rect get_viewport() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_alpha_enabled(bool enabled) = 0;

		virtual void set_alpha_fn(alpha_fn const & value) = 0;
		
		virtual void set_blend_color(color const & value) = 0;

		virtual void set_blend_enabled(bool enabled) = 0;
		
		virtual void set_blend_eq(blend_eq const & value) = 0;
		
		virtual void set_blend_fn(blend_fn const & value) = 0;
		
		virtual void set_clear_color(color const & value) = 0;
		
		virtual void set_cull_enabled(bool enabled) = 0;

		virtual void set_cull_facet(uint32_t facet) = 0;

		virtual void set_cull_order(uint32_t order) = 0;
		
		virtual void set_depth_enabled(bool enabled) = 0;

		virtual void set_depth_mask(bool enabled) = 0;

		virtual void set_depth_pr(uint32_t predicate) = 0;
		
		virtual void set_depth_range(depth_range const & value) = 0;

		virtual void set_stencil_enabled(bool enabled) = 0;

		virtual void set_stencil_fn(stencil_fn const & value) = 0;

		virtual void set_viewport(int_rect const & bounds) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void clear(uint32_t flags) = 0;

		virtual void draw(shared<vertexarray> const & value) = 0;
		
		virtual void draw_arrays(size_t first, size_t count) = 0;
		
		virtual void draw_indexed(size_t count) = 0;
		
		virtual void flush() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void upload(handle_t loc, bool const & value) = 0;

		virtual void upload(handle_t loc, int32_t const & value) = 0;

		virtual void upload(handle_t loc, float_t const & value) = 0;

		virtual void upload(handle_t loc, vec2 const & value) = 0;

		virtual void upload(handle_t loc, vec3 const & value) = 0;

		virtual void upload(handle_t loc, vec4 const & value) = 0;

		virtual void upload(handle_t loc, mat2 const & value) = 0;

		virtual void upload(handle_t loc, mat3 const & value) = 0;

		virtual void upload(handle_t loc, mat4 const & value) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// resource
namespace ml::gl
{
	// base device resource
	class ML_GRAPHICS_API resource : public trackable, public non_copyable
	{
	public:
		virtual ~resource() noexcept override = default;

		virtual resource & generate() = 0;

		virtual resource & destroy() = 0;

		ML_NODISCARD virtual handle_t get_handle() const noexcept = 0;

		ML_NODISCARD virtual typeof<> get_typeof() const noexcept = 0;

		ML_NODISCARD operator bool() const noexcept { return get_handle(); }

		ML_NODISCARD bool is_equal(resource const & other) const noexcept
		{
			return do_is_equal(other);
		}

	protected:
		virtual bool do_is_equal(resource const & other) const noexcept = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gl
{
	// base vertexarray
	class ML_GRAPHICS_API vertexarray : public resource
	{
	public:
		ML_NODISCARD static shared<vertexarray> allocate() noexcept;

		virtual ~vertexarray() noexcept override = default;

		virtual vertexarray const & bind() const = 0;

		virtual vertexarray const & unbind() const = 0;

		virtual vertexarray & generate() override = 0;

		virtual vertexarray & destroy() override = 0;

		virtual vertexarray & add_vb(shared<vertexbuffer> const & value) & = 0;

		virtual vertexarray & set_ib(shared<indexbuffer> const & value) & = 0;

		ML_NODISCARD virtual handle_t get_handle() const noexcept override = 0;

		ML_NODISCARD virtual shared<indexbuffer> const & get_ib() const noexcept = 0;

		ML_NODISCARD virtual typeof<> get_typeof() const noexcept override = 0;
		
		ML_NODISCARD virtual pmr::vector<shared<vertexbuffer>> const & get_vbs() const noexcept = 0;

	protected:
		virtual bool do_is_equal(resource const & other) const noexcept override = 0;
	};

	template <class ... Args
	> ML_NODISCARD auto make_vertexarray(Args && ... args) noexcept
	{
		return vertexarray::allocate(ML_forward(args)...);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gl
{
	// base vertexbuffer
	class ML_GRAPHICS_API vertexbuffer : public resource
	{
	public:
		ML_NODISCARD static shared<vertexbuffer> allocate(buffer_t data, size_t count) noexcept;

		ML_NODISCARD static shared<vertexbuffer> allocate(size_t count) noexcept;

		virtual ~vertexbuffer() noexcept override = default;

		virtual vertexbuffer const & bind() const = 0;

		virtual vertexbuffer const & unbind() const = 0;

		virtual vertexbuffer & generate() override = 0;

		virtual vertexbuffer & destroy() override = 0;

		virtual vertexbuffer & set_data(buffer_t data, size_t count) = 0;

		virtual vertexbuffer & set_data(buffer_t data, size_t count, size_t offset) = 0;

		virtual vertexbuffer & set_layout(layout const & value) = 0;

		ML_NODISCARD virtual size_t get_count() const noexcept = 0;

		ML_NODISCARD virtual handle_t get_handle() const noexcept override = 0;

		ML_NODISCARD virtual layout const & get_layout() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_usage() const noexcept = 0;

		ML_NODISCARD virtual view_t const & get_view() const noexcept = 0;

	protected:
		virtual bool do_is_equal(resource const & other) const noexcept override = 0;
	};

	template <class ... Args
	> ML_NODISCARD auto make_vertexbuffer(Args && ... args) noexcept
	{
		return vertexbuffer::allocate(ML_forward(args)...);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gl
{
	// base indexbuffer
	class ML_GRAPHICS_API indexbuffer : public resource
	{
	public:
		ML_NODISCARD static shared<indexbuffer> allocate(buffer_t data, size_t count) noexcept;
		
		virtual ~indexbuffer() noexcept override = default;

		virtual indexbuffer const & bind() const = 0;

		virtual indexbuffer const & unbind() const = 0;

		virtual indexbuffer & generate() override = 0;

		virtual indexbuffer & destroy() override = 0;

		virtual indexbuffer & set_data(buffer_t data, size_t count) = 0;

		ML_NODISCARD virtual size_t get_count() const noexcept = 0;

		ML_NODISCARD virtual handle_t get_handle() const noexcept override = 0;

		ML_NODISCARD virtual uint32_t get_usage() const noexcept = 0;

		ML_NODISCARD virtual view_t const & get_view() const noexcept = 0;

	protected:
		virtual bool do_is_equal(resource const & other) const noexcept override = 0;
	};

	template <class ... Args
	> ML_NODISCARD auto make_indexbuffer(Args && ... args) noexcept
	{
		return indexbuffer::allocate(ML_forward(args)...);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gl
{
	// base framebuffer
	class ML_GRAPHICS_API framebuffer : public resource
	{
	public:
		ML_NODISCARD static shared<framebuffer> allocate(vec2i const & size, uint32_t format = format_rgba) noexcept;
		
		virtual ~framebuffer() noexcept override = default;

		virtual framebuffer const & bind() const = 0;

		virtual framebuffer const & unbind() const = 0;

		virtual framebuffer & generate() override = 0;

		virtual framebuffer & destroy() override = 0;

		virtual framebuffer const & bind_texture(uint32_t slot = 0) const & = 0;

		virtual framebuffer & resize(vec2i const & value) & = 0;

		ML_NODISCARD virtual shared<texture2d> const & get_color_attachment() const noexcept = 0;

		ML_NODISCARD virtual shared<texture2d> const & get_depth_attachment() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_format() const noexcept = 0;

		ML_NODISCARD virtual handle_t get_handle() const noexcept override = 0;

		ML_NODISCARD virtual vec2i const & get_size() const noexcept = 0;

	protected:
		virtual bool do_is_equal(resource const & other) const noexcept override = 0;
	};

	template <class ... Args
	> ML_NODISCARD auto make_framebuffer(Args && ... args) noexcept
	{
		return framebuffer::allocate(ML_forward(args)...);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gl
{
	// base texture2d
	class ML_GRAPHICS_API texture2d : public resource
	{
	public:
		ML_NODISCARD static shared<texture2d> allocate(
			vec2i const &	size,
			uint32_t		iformat,
			uint32_t		cformat,
			uint32_t		ptype	= type_unsigned_byte,
			int32_t			flags	= texture_flags_default,
			buffer_t		data	= nullptr
		) noexcept;

		ML_NODISCARD static shared<texture2d> allocate(image const & img, int32_t flags = texture_flags_default) noexcept
		{
			auto const fmt{ std::invoke([&]() noexcept -> uint32_t
			{
				switch (img.channels())
				{
				default	: return 0;
				case 1	: return format_red;
				case 3	: return format_rgb;
				case 4	: return format_rgba;
				}
			}) };
			return allocate(img.size(), fmt, fmt, type_unsigned_byte, flags, img.data());
		}

		ML_NODISCARD static shared<texture2d> allocate(fs::path const & path, int32_t flags = texture_flags_default) noexcept
		{
			return allocate(image{ path }, flags);
		}

		virtual ~texture2d() noexcept override = default;

		virtual texture2d const & bind() const = 0;

		virtual texture2d const & unbind() const = 0;

		virtual texture2d & generate() override = 0;

		virtual texture2d & destroy() override = 0;

		virtual texture2d & set_mipmapped(bool value) & = 0;

		virtual texture2d & set_repeated(bool value) & = 0;

		virtual texture2d & set_smooth(bool value) & = 0;

		virtual texture2d & update(vec2i const & size, buffer_t data = nullptr) & = 0;

		ML_NODISCARD virtual image copy_to_image() const = 0;

		ML_NODISCARD virtual int32_t get_flags() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_color_format() const noexcept = 0;

		ML_NODISCARD virtual handle_t get_handle() const noexcept override = 0;

		ML_NODISCARD virtual uint32_t get_internal_format() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_pixel_type() const noexcept = 0;

		ML_NODISCARD virtual vec2i const & get_size() const noexcept = 0;

		ML_NODISCARD uint32_t get_texture_type() const noexcept { return texture_type_2d; }

		ML_NODISCARD virtual typeof<> get_typeof() const noexcept override = 0;

	protected:
		virtual bool do_is_equal(resource const & other) const noexcept override = 0;
	};

	template <class ... Args
	> ML_NODISCARD auto make_texture2d(Args && ... args) noexcept
	{
		return texture2d::allocate(ML_forward(args)...);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gl
{
	// base program
	class ML_GRAPHICS_API program : public resource
	{
	public:
		ML_NODISCARD static shared<program> allocate(uint32_t type) noexcept;

		ML_NODISCARD static shared<program> allocate(uint32_t type, size_t count, cstring const * src) noexcept
		{
			return std::invoke([&, temp = allocate(type)]() noexcept
			{
				temp->compile(count, src);
				return temp;
			});
		}

		ML_NODISCARD static shared<program> allocate(uint32_t type, cstring src) noexcept
		{
			return allocate(type, 1, &src);
		}

		virtual ~program() noexcept override = default;

		virtual program & generate() override = 0;

		virtual program & destroy() override = 0;

		virtual int32_t compile(size_t count, cstring const * src) = 0;

		int32_t compile(cstring src) noexcept { return compile(1, &src); }

		ML_NODISCARD virtual handle_t get_handle() const noexcept override = 0;

		ML_NODISCARD virtual pmr::vector<char> const & get_log() const noexcept = 0;

		ML_NODISCARD virtual int32_t get_status() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_shader_type() const noexcept = 0;

		ML_NODISCARD virtual typeof<> get_typeof() const noexcept override = 0;

	protected:
		virtual bool do_is_equal(resource const & other) const noexcept override = 0;
	};

	template <class ... Args
	> ML_NODISCARD auto make_program(Args && ... args) noexcept
	{
		return program::allocate(ML_forward(args)...);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gl
{
	// base shader
	class ML_GRAPHICS_API shader : public resource
	{
	public:
		ML_NODISCARD static shared<shader> allocate() noexcept;

		ML_NODISCARD static shared<shader> allocate(std::initializer_list<shared<program>> init) noexcept
		{
			return std::invoke([&, temp = allocate()]() noexcept
			{
				for (auto const & e : init) { temp->attach(e); }
				temp->link();
				return temp;
			});
		}
		
		virtual ~shader() noexcept override = default;

		virtual shader const & bind() const = 0;

		virtual shader const & unbind() const = 0;

		virtual shader & generate() override = 0;

		virtual shader & destroy() override = 0;

		virtual int32_t attach(shared<program> const & value) = 0;

		int32_t attach(uint32_t type, size_t count, cstring const * src) noexcept
		{
			return attach(make_program(type, count, src));
		}

		int32_t attach(uint32_t type, cstring src) noexcept
		{
			return attach(type, 1, &src);
		}

		virtual int32_t link() = 0;

		virtual shader const & bind_textures() const = 0;

		virtual bool bind_uniform(cstring name, std::function<void(handle_t)> const & callback) = 0;

		template <class T> bool set_uniform(cstring name, T const & value) noexcept
		{
			return bind_uniform(name, [&](handle_t loc) noexcept
			{
				if constexpr (std::is_same_v<T, shared<texture2d>>)
				{
					do_cache_texture(loc, value ? value->get_handle() : nullptr);
				}
				else
				{
					device::get()->upload(loc, value);
				}
			});
		}

		ML_NODISCARD virtual handle_t get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof<> get_typeof() const noexcept override = 0;

	protected:
		virtual bool do_is_equal(resource const & other) const noexcept override = 0;

		virtual void do_cache_texture(handle_t loc, handle_t value) noexcept = 0;
	};

	template <class ... Args
	> ML_NODISCARD auto make_shader(Args && ... args) noexcept
	{
		return shader::allocate(ML_forward(args)...);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RENDER_API_HPP_