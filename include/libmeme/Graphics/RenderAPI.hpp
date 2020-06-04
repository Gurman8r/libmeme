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

	// classes
	class render_api	;
	class vertexarray	;
	class vertexbuffer	;
	class indexbuffer	;
	class framebuffer	;
	class shader		;
	class texture2d		;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// aliases
	ML_alias buffer		= typename void const *				; // buffer type
	ML_alias command	= typename std::function<void()>	; // command type
	ML_alias handle		= typename void *					; // handle type

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
		vertex_shader,
		fragment_shader,
		geometry_shader,

		shader_type_MAX
	};

	constexpr cstring shader_type_names[] =
	{
		"vertex shader",
		"fragment shader",
		"geometry shader",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum texture_type_ : uint32_t
	{
		texture_1d,
		texture_2d,
		texture_3d,
		texture_cube_map,

		texture_type_MAX
	};

	constexpr cstring texture_type_names[] =
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

	ML_NODISCARD constexpr uint32_t channels_to_format(size_t value) noexcept
	{
		switch (value)
		{
		default	: return 0;
		case 1	: return format_red;
		case 3	: return format_rgb;
		case 4	: return format_rgba;
		}
	}

	ML_NODISCARD constexpr size_t format_to_channels(uint32_t value) noexcept
	{
		switch (value)
		{
		default				: return 0;
		case format_red	: return 1;
		case format_rgb	: return 3;
		case format_rgba: return 4;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// buffer element
	struct ML_NODISCARD buffer_element final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pmr::string name		{};
		hash_t		type		{};
		uint32_t	size		{};
		bool		normalized	{};
		uint32_t	offset		{};

		buffer_element(pmr::string const & name, hash_t type, uint32_t size, bool normalized) noexcept
			: name{ name }, type{ type }, size{ size }, normalized{ normalized }, offset{}
		{
		}

		template <class T
		> buffer_element(meta::tag<T>, pmr::string const & name, bool normalized = 0) noexcept
			: buffer_element{ name, hashof_v<T>, sizeof(T), normalized }
		{
			static_assert(util::is_any_of_v<T,
				bool,
				int32_t, vec2i, vec3i, vec4i, mat2i, mat3i, mat4i,
				float_t, vec2f, vec3f, vec4f, mat2f, mat3f, mat4f
			>);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// buffer layout
	struct ML_NODISCARD buffer_layout final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using storage_type				= typename pmr::vector<buffer_element>;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		buffer_layout(std::initializer_list<buffer_element> init) noexcept
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

		buffer_layout() noexcept : buffer_layout
		{
			{ meta::tag_v<vec3>, "a_position"	},
			{ meta::tag_v<vec3>, "a_normal"		},
			{ meta::tag_v<vec2>, "a_texcoord"	},
		}
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_elements() const & noexcept -> storage_type const & { return m_elements; }

		ML_NODISCARD auto get_stride() const noexcept -> uint32_t { return m_stride; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool empty() const noexcept { return m_elements.empty(); }

		ML_NODISCARD auto size() const noexcept -> uint32_t { return (uint32_t)m_elements.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto begin() noexcept -> iterator { return m_elements.begin(); }

		ML_NODISCARD auto begin() const noexcept -> const_iterator { return m_elements.begin(); }

		ML_NODISCARD auto cbegin() const noexcept -> const_iterator { return m_elements.cbegin(); }

		ML_NODISCARD auto cend() const noexcept -> const_iterator { return m_elements.cend(); }

		ML_NODISCARD auto end() noexcept -> iterator { return m_elements.end(); }

		ML_NODISCARD auto end() const noexcept -> const_iterator { return m_elements.end(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> void for_each(Fn && fn) noexcept
		{
			for (uint32_t i = 0, imax = size(); i < imax; ++i)
			{
				std::invoke(ML_forward(fn), i, m_elements[(size_t)i]);
			}
		}

		template <class Fn
		> void for_each(Fn && fn) const noexcept
		{
			for (uint32_t i = 0, imax = size(); i < imax; ++i)
			{
				std::invoke(ML_forward(fn), i, m_elements[(size_t)i]);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		uint32_t		m_stride	{}; // stride
		storage_type	m_elements	{}; // elements

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// api
namespace ml::gl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD api_info final
	{
		api_info() noexcept = default;

		// version
		int32_t major_version, minor_version;
		pmr::string renderer, vendor, version, shading_language_version;
		pmr::vector<pmr::string> extensions;

		// textures
		bool edge_clamp_available;
		uint32_t max_texture_slots;

		// shaders
		bool shaders_available;
		bool geometry_shaders_available;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// base render api
	class ML_GRAPHICS_API render_api : public trackable, public non_copyable
	{
	protected:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_api() noexcept override = default;

		virtual bool do_initialize() = 0;

		virtual void on_initialize() = 0;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static render_api * const get() noexcept;

		bool initialize() noexcept
		{
			bool temp{};
			static ML_scope // once
			{
				if (temp = do_initialize())
				{
					on_initialize();
				}
			};
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual uint32_t get_error() const = 0;

		virtual api_info const & get_info() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool get_alpha_enabled() const = 0;

		virtual alpha_fn get_alpha_fn() const = 0;
		
		virtual bool get_blend_enabled() const = 0;

		virtual color get_blend_color() const = 0;
		
		virtual blend_eq get_blend_eq() const = 0;
		
		virtual blend_fn get_blend_fn() const = 0;
		
		virtual color get_clear_color() const = 0;

		virtual bool get_cull_enabled() const = 0;

		virtual uint32_t get_cull_facet() const = 0;

		virtual uint32_t get_cull_order() const = 0;
		
		virtual bool get_depth_enabled() const = 0;

		virtual bool get_depth_mask() const = 0;
		
		virtual uint32_t get_depth_pr() const = 0;

		virtual depth_range get_depth_range() const = 0;

		virtual bool get_stencil_enabled() const = 0;

		virtual stencil_fn get_stencil_fn() const = 0;

		virtual int_rect get_viewport() const = 0;

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
		
		virtual void draw_arrays(uint32_t first, uint32_t count) = 0;
		
		virtual void draw_indexed(uint32_t count) = 0;
		
		virtual void flush() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void upload(handle loc, int32_t const & value) = 0;

		virtual void upload(handle loc, float_t const & value) = 0;

		virtual void upload(handle loc, vec2 const & value) = 0;

		virtual void upload(handle loc, vec3 const & value) = 0;

		virtual void upload(handle loc, vec4 const & value) = 0;

		virtual void upload(handle loc, mat2 const & value) = 0;

		virtual void upload(handle loc, mat3 const & value) = 0;

		virtual void upload(handle loc, mat4 const & value) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// graphics resource
namespace ml::gl
{
	// base graphics resource
	class ML_GRAPHICS_API graphics_resource : public trackable, public non_copyable
	{
	public:
		virtual ~graphics_resource() noexcept override = default;

		static render_api * const get_api() noexcept { return render_api::get(); }

		virtual bool generate() = 0;

		virtual bool release() = 0;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		ML_NODISCARD bool is_equal(graphics_resource const & other) const noexcept
		{
			return do_is_equal(other);
		}

	protected:
		virtual bool do_is_equal(graphics_resource const & other) const noexcept = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gl
{
	// base vertexarray
	class ML_GRAPHICS_API vertexarray : public graphics_resource
	{
	public:
		virtual ~vertexarray() noexcept override = default;

		virtual bool generate() override = 0;

		virtual bool release() override = 0;

		virtual void bind() const override = 0;

		virtual void unbind() const override = 0;

		virtual void add_vbo(shared<vertexbuffer> const & value) = 0;

		virtual void set_ibo(shared<indexbuffer> const & value) = 0;

		ML_NODISCARD virtual handle get_handle() const noexcept = 0;

		ML_NODISCARD virtual shared<indexbuffer> const & get_ibo() const noexcept = 0;
		
		ML_NODISCARD virtual pmr::vector<shared<vertexbuffer>> const & get_vbos() const noexcept = 0;

		ML_NODISCARD virtual bool has_value() const noexcept = 0;

		ML_NODISCARD static shared<vertexarray> create() noexcept;

	protected:
		virtual bool do_is_equal(graphics_resource const & other) const noexcept override = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gl
{
	// base vertexbuffer
	class ML_GRAPHICS_API vertexbuffer : public graphics_resource
	{
	public:
		virtual ~vertexbuffer() noexcept override = default;

		virtual bool generate() override = 0;

		virtual bool release() override = 0;

		virtual void bind() const override = 0;
		
		virtual void unbind() const override = 0;

		virtual void set_data(buffer verts, uint32_t count) = 0;

		virtual void set_data(buffer verts, uint32_t count, uint32_t offset) = 0;

		virtual void set_layout(buffer_layout const & value) = 0;

		ML_NODISCARD virtual uint32_t get_count() const noexcept = 0;

		ML_NODISCARD virtual handle get_handle() const noexcept = 0;

		ML_NODISCARD virtual buffer_layout const & get_layout() const noexcept = 0;

		ML_NODISCARD virtual bool has_value() const noexcept = 0;

		ML_NODISCARD static shared<vertexbuffer> create(buffer verts, uint32_t count) noexcept;

		ML_NODISCARD static shared<vertexbuffer> create(uint32_t count) noexcept;

	protected:
		virtual bool do_is_equal(graphics_resource const & other) const noexcept override = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gl
{
	// base indexbuffer
	class ML_GRAPHICS_API indexbuffer : public graphics_resource
	{
	public:
		virtual ~indexbuffer() noexcept override = default;

		virtual bool generate() override = 0;

		virtual bool release() override = 0;

		virtual void bind() const override = 0;
		
		virtual void unbind() const override = 0;

		virtual void set_data(buffer inds, uint32_t count) = 0;

		ML_NODISCARD virtual uint32_t get_count() const noexcept = 0;

		ML_NODISCARD virtual handle get_handle() const noexcept = 0;

		ML_NODISCARD virtual bool has_value() const noexcept = 0;

		ML_NODISCARD static shared<indexbuffer> create(buffer inds, uint32_t count) noexcept;

	protected:
		virtual bool do_is_equal(graphics_resource const & other) const noexcept override = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gl
{
	// base framebuffer
	class ML_GRAPHICS_API framebuffer : public graphics_resource
	{
	public:
		virtual ~framebuffer() noexcept override = default;

		virtual bool generate() override = 0;

		virtual bool release() override = 0;

		virtual void bind() const override = 0;
		
		virtual void unbind() const override = 0;

		virtual void bind_texture(uint32_t slot = 0) const = 0;

		virtual void resize(vec2i const & value) = 0;

		ML_NODISCARD virtual shared<texture2d> const & get_color_attachment() const noexcept = 0;

		ML_NODISCARD virtual shared<texture2d> const & get_depth_attachment() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_format() const noexcept = 0;

		ML_NODISCARD virtual handle get_handle() const noexcept = 0;

		ML_NODISCARD virtual vec2i get_size() const noexcept = 0;

		ML_NODISCARD virtual bool has_value() const noexcept = 0;

		ML_NODISCARD static shared<framebuffer> create(uint32_t format, vec2i const & size) noexcept;

	protected:
		virtual bool do_is_equal(graphics_resource const & other) const noexcept override = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gl
{
	// base texture2d
	class ML_GRAPHICS_API texture2d : public graphics_resource
	{
	public:
		virtual ~texture2d() noexcept override = default;

		virtual bool generate() override = 0;

		virtual bool release() override = 0;

		virtual void bind() const override = 0;

		virtual void unbind() const override = 0;

		virtual void update(vec2i const & size, buffer pixels = nullptr) = 0;

		virtual void set_mipmapped(bool value) = 0;

		virtual void set_repeated(bool value) = 0;

		virtual void set_smooth(bool value) = 0;

		ML_NODISCARD virtual image copy_to_image() const = 0;

		ML_NODISCARD virtual int32_t get_flags() const noexcept = 0;

		ML_NODISCARD virtual handle get_handle() const noexcept = 0;

		ML_NODISCARD virtual vec2i const & get_size() const noexcept = 0;

		ML_NODISCARD virtual bool has_value() const noexcept = 0;

		ML_NODISCARD static shared<texture2d> create(
			vec2i const &	size,
			uint32_t		iformat,
			uint32_t		cformat,
			uint32_t		ptype = type_unsigned_byte,
			int32_t			flags = texture_flags_default,
			buffer			pixels = nullptr
		) noexcept;

		ML_NODISCARD static shared<texture2d> create(image const & img, int32_t flags = texture_flags_default) noexcept
		{
			uint32_t const fmt{ channels_to_format(img.channels()) };
			return create(img.size(), fmt, fmt, type_unsigned_byte, flags, img.data());
		}

		ML_NODISCARD static shared<texture2d> create(fs::path const & path, int32_t flags = texture_flags_default) noexcept
		{
			return create(image{ path }, flags);
		}

	protected:
		virtual bool do_is_equal(graphics_resource const & other) const noexcept override = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gl
{
	// base shader
	class ML_GRAPHICS_API shader : public graphics_resource
	{
	public:
		virtual ~shader() noexcept override = default;

		virtual bool generate() override = 0;

		virtual bool release() override = 0;

		virtual void bind() const override = 0;
		
		virtual void unbind() const override = 0;

		virtual int32_t attach(uint32_t type, uint32_t count, cstring const * src) = 0;

		int32_t attach(uint32_t type, cstring src) noexcept { return attach(type, 1, &src); }

		virtual int32_t link() = 0;

		virtual void bind_textures() const = 0;

		virtual bool bind_uniform(cstring name, std::function<void(handle)> const & callback) = 0;

		template <class T> bool set_uniform(cstring name, T const & value) noexcept
		{
			return bind_uniform(name, [&](handle loc) noexcept
			{
				if constexpr (std::is_same_v<T, shared<texture2d>>)
				{
					do_cache_texture(loc, value ? value->get_handle() : nullptr);
				}
				else
				{
					get_api()->upload(loc, value);
				}
			});
		}

		ML_NODISCARD virtual handle get_handle() const noexcept = 0;

		ML_NODISCARD virtual bool has_value() const noexcept = 0;

		ML_NODISCARD static shared<shader> create() noexcept;

	protected:
		virtual bool do_is_equal(graphics_resource const & other) const noexcept override = 0;

		virtual void do_cache_texture(handle loc, handle value) noexcept = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RENDER_API_HPP_