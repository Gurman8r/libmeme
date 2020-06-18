#ifndef _ML_RENDER_API_HPP_
#define _ML_RENDER_API_HPP_

#include <libmeme/Core/Color.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Rect.hpp>
#include <libmeme/Graphics/Binder.hpp>
#include <libmeme/Graphics/Image.hpp>
#include <libmeme/Graphics/Vertex.hpp>
#include <libmeme/Platform/ContextSettings.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// types
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class device			;
	class device_resource	;

	class vertexbuffer		;
	class indexbuffer		;
	class vertexarray		;

	class texture			;
	class texture2d			;
	class texturecube		;
	class framebuffer		;

	class shader			;
	class program			;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_handle(resource_id) ; // object handle
	ML_decl_handle(uniform_id) 	; // uniform binding

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias address_t		= typename void const *							; // input data address
	ML_alias buffer_t		= typename pmr::vector<byte_t>					; // buffer data
	ML_alias shader_map_t	= typename ds::map<uint32_t, shared<shader>>	; // shader map
	ML_alias shader_src_t	= typename pmr::vector<pmr::string>				; // shader source
	ML_alias tex_buffer_t	= typename pmr::vector<shared<texture2d>>		; // texture buffer
	ML_alias tex_cache_t	= typename ds::map<uniform_id, shared<texture>>	; // texture cache
	ML_alias uni_binder_t	= typename std::function<void(uniform_id)>		; // uniform binder
	ML_alias uni_cache_t	= typename ds::map<hash_t, uniform_id>			; // uniform cache

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// enums
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// conversion helpers
	
	struct to_user : std::false_type {}; // ( false_type ) | backend -> frontend

	struct to_impl : std::true_type {}; // ( true_type ) | frontend -> backend

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum buffer_bit_ : uint32_t
	{
		buffer_bit_none		= 0,
		buffer_bit_color	= 1 << 0,
		buffer_bit_depth	= 1 << 1,
		buffer_bit_stencil	= 1 << 2,

		buffer_bit_all
			= buffer_bit_color
			| buffer_bit_depth
			| buffer_bit_stencil,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum error_type_ : uint32_t
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

	constexpr cstring error_descriptions[] = // error descriptions
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
		"clockwise",
		"counter-clockwise",
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

	enum shader_type_ : uint32_t
	{
		shader_type_vertex,
		shader_type_fragment,
		shader_type_geometry,

		shader_type_max,
	};

	constexpr cstring shader_type_names[] =
	{
		"vertex shader",
		"fragment shader",
		"geometry shader",
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// texture types
	enum texture_type_ : uint32_t
	{
		texture_type_1d,
		texture_type_2d,
		texture_type_3d,
		texture_type_cubemap,

		texture_type_max,
	};

	// texture type names
	constexpr cstring texture_type_names[] =
	{
		"texture 1d",
		"texture 2d",
		"texture 3d",
		"texture cube map",
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// buffer
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// buffer copy
	template <class Elem = byte_t
	> buffer_t bufcpy(size_t count, address_t data, buffer_t::allocator_type alloc = {}) noexcept
	{
		return data
			? buffer_t{ (byte_t *)data, (byte_t *)data + count * sizeof(Elem), alloc }
			: buffer_t{ count * sizeof(Elem), (byte_t)0, alloc };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// get element base type
	ML_NODISCARD constexpr hash_t get_element_base_type(hash_t type) noexcept
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

	template <class T> ML_NODISCARD constexpr hash_t get_element_base_type() noexcept
	{
		return get_element_base_type(hashof_v<T>);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// get element component count
	ML_NODISCARD constexpr uint32_t get_element_component_count(hash_t type) noexcept
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

	template <class T> ML_NODISCARD constexpr uint32_t get_element_component_count() noexcept
	{
		return get_element_component_count(hashof_v<T>);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vertex buffer layout element
	struct ML_NODISCARD buffer_element final
	{
		template <class T
		> static constexpr bool is_valid_type
		{
			util::is_any_of_v<T,
				bool,
				int32_t, vec2i, vec3i, vec4i, mat2i, mat3i, mat4i,
				float_t, vec2f, vec3f, vec4f, mat2f, mat3f, mat4f
			>
		};

		cstring		name		{};
		hash_t		type		{};
		uint32_t	size		{};
		bool		normalized	{};
		uint32_t	offset		{};

		constexpr buffer_element(cstring name, hash_t type, uint32_t size, bool normalized) noexcept
			: name{ name }, type{ type }, size{ size }, normalized{ normalized }, offset{}
		{
		}

		template <class Elem
		> constexpr buffer_element(Elem, cstring name, bool normalized = false) noexcept
			: buffer_element{ name, hashof_v<Elem>, sizeof(Elem), normalized }
		{
			static_assert(is_valid_type<Elem>);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// default vertex buffer layout
	constexpr std::initializer_list<buffer_element> default_layout
	{
		{ vec3{}, "a_position"	},
		{ vec3{}, "a_normal"	},
		{ vec2{}, "a_texcoord"	},
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vertex buffer layout
	struct ML_NODISCARD buffer_layout final
	{
		using storage_type				= typename pmr::vector<buffer_element>;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		buffer_layout(std::initializer_list<buffer_element> init = default_layout) noexcept
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

		ML_NODISCARD auto elements() const noexcept -> storage_type const & { return m_elements; }

		ML_NODISCARD auto stride() const noexcept -> uint32_t { return m_stride; }

	private:
		uint32_t		m_stride	{}; // stride
		storage_type	m_elements	{}; // elements
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// render states
namespace ml::gfx
{
	struct ML_NODISCARD alpha_mode final
	{
		uint32_t	pred	{ predicate_greater };
		float_t		ref		{ 0.001f };
	};

	struct ML_NODISCARD blend_mode final
	{
		uint32_t
			color_equation	{ function_add },
			color_sfactor	{ factor_src_alpha },
			color_dfactor	{ factor_one_minus_src_alpha },
			alpha_equation	{ color_equation },
			alpha_sfactor	{ color_sfactor },
			alpha_dfactor	{ color_dfactor };
	};

	struct ML_NODISCARD cull_mode final
	{
		uint32_t	facet	{ facet_back };
		uint32_t	order	{ order_ccw };
	};

	struct ML_NODISCARD depth_mode final
	{
		uint32_t	pred	{ predicate_less };
		vec2		range	{ 0.f, 1.f };
	};

	struct ML_NODISCARD stencil_mode final
	{
		uint32_t	pred	{ predicate_always };
		int32_t		ref		{ 0 };
		uint32_t	mask	{ 0xffffffff };
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// device info
	struct ML_NODISCARD devinfo final : trackable, non_copyable
	{
		// version
		pmr::string renderer, vendor, version;
		int32_t major_version, minor_version;
		pmr::vector<pmr::string> extensions;

		// textures
		bool texture_edge_clamp_available;
		uint32_t max_texture_slots;

		// framebuffers
		uint32_t max_color_attachments;
		uint32_t max_samples;

		// shaders
		bool shaders_available;
		bool geometry_shaders_available;
		bool separate_shaders_available;
		pmr::string shading_language_version;

		// functions
		std::function<uint32_t()> get_error;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// base device
	class ML_GRAPHICS_API device : public trackable, public non_copyable
	{
	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static device * g_devctx; // global device context

	protected:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~device() noexcept = default;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static device * create_context(context_settings const & cs);

		static void destroy_context(device * value);

		ML_NODISCARD static device * get_current_context() noexcept { return g_devctx; }

		static void set_current_context(device * value) noexcept { g_devctx = value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual devinfo const & get_devinfo() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual bool get_alpha_enabled() const = 0;

		ML_NODISCARD virtual alpha_mode get_alpha_mode() const = 0;
		
		ML_NODISCARD virtual bool get_blend_enabled() const = 0;

		ML_NODISCARD virtual color get_blend_color() const = 0;
		
		ML_NODISCARD virtual blend_mode get_blend_mode() const = 0;
		
		ML_NODISCARD virtual color get_clear_color() const = 0;

		ML_NODISCARD virtual bool get_cull_enabled() const = 0;

		ML_NODISCARD virtual cull_mode get_cull_mode() const = 0;

		ML_NODISCARD virtual bool get_depth_enabled() const = 0;

		ML_NODISCARD virtual depth_mode get_depth_mode() const = 0;

		ML_NODISCARD virtual bool get_depth_write() const = 0;

		ML_NODISCARD virtual bool get_stencil_enabled() const = 0;

		ML_NODISCARD virtual stencil_mode get_stencil_mode() const = 0;

		ML_NODISCARD virtual int_rect get_viewport() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_alpha_enabled(bool enabled) = 0;

		virtual void set_alpha_mode(alpha_mode const & value) = 0;
		
		virtual void set_blend_color(color const & value) = 0;

		virtual void set_blend_enabled(bool enabled) = 0;
		
		virtual void set_blend_mode(blend_mode const & value) = 0;
		
		virtual void set_clear_color(color const & value) = 0;
		
		virtual void set_cull_enabled(bool enabled) = 0;

		virtual void set_cull_mode(cull_mode const & value) = 0;

		virtual void set_depth_enabled(bool enabled) = 0;

		virtual void set_depth_mode(depth_mode const & value) = 0;

		virtual void set_depth_write(bool enabled) = 0;

		virtual void set_stencil_enabled(bool enabled) = 0;

		virtual void set_stencil_mode(stencil_mode const & value) = 0;

		virtual void set_viewport(int_rect const & bounds) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void clear(uint32_t flags) = 0;

		virtual void draw(shared<vertexarray> const & value) = 0;

		virtual void draw_arrays(uint32_t mode, size_t first, size_t count) = 0;
		
		virtual void draw_indexed(uint32_t mode, size_t count) = 0;
		
		virtual void flush() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void upload(uniform_id loc, bool value) = 0;

		virtual void upload(uniform_id loc, int32_t value) = 0;

		virtual void upload(uniform_id loc, float_t value) = 0;

		virtual void upload(uniform_id loc, vec2 const & value) = 0;

		virtual void upload(uniform_id loc, vec3 const & value) = 0;

		virtual void upload(uniform_id loc, vec4 const & value) = 0;

		virtual void upload(uniform_id loc, mat2 const & value) = 0;

		virtual void upload(uniform_id loc, mat3 const & value) = 0;

		virtual void upload(uniform_id loc, mat4 const & value) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// device resource
namespace ml::gfx
{
	// base device resource
	class ML_GRAPHICS_API device_resource : public trackable, public non_copyable
	{
	public:
		virtual ~device_resource() override = default;

		virtual bool invalidate() = 0;

		ML_NODISCARD virtual resource_id get_handle() const noexcept = 0;

		ML_NODISCARD virtual typeof<> const & get_type_info() const noexcept = 0;

	public:
		ML_NODISCARD inline bool has_value() const noexcept { return (bool)get_handle(); }

		ML_NODISCARD inline operator bool() const noexcept { return (bool)get_handle(); }

		ML_NODISCARD inline bool is_equal(device_resource const & other) const noexcept
		{
			return (this == std::addressof(other)) || do_is_equal(other);
		}

		ML_NODISCARD inline bool operator==(device_resource const & other) const noexcept
		{
			return (this == std::addressof(other)) || do_is_equal(other);
		}

	protected:
		virtual bool do_is_equal(device_resource const & other) const noexcept = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gfx
{
	// base vertexbuffer
	class ML_GRAPHICS_API vertexbuffer : public device_resource
	{
	public:
		ML_NODISCARD static shared<vertexbuffer> create(uint32_t usage, size_t count, address_t data) noexcept;

		ML_NODISCARD static shared<vertexbuffer> create(size_t count, address_t data = nullptr) noexcept
		{
			return create(data ? usage_static_draw : usage_dynamic_draw, count, data);
		}

	public:
		virtual ~vertexbuffer() override = default;

		virtual bool invalidate() override = 0;

		ML_NODISCARD virtual resource_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof<> const & get_type_info() const noexcept override = 0;

	public:
		static void bind(vertexbuffer const * value) noexcept;

		static void bind(shared<vertexbuffer> const & value) noexcept
		{
			bind(static_cast<vertexbuffer const *>(value.get()));
		}

		inline void bind() const noexcept { bind(this); }

		inline void unbind() const noexcept { bind(nullptr); }

	public:
		virtual void set_data(size_t count, address_t data, size_t offset = 0) = 0;

		virtual void set_layout(buffer_layout const & value) = 0;

		ML_NODISCARD virtual buffer_t const & get_buffer() const noexcept = 0;

		ML_NODISCARD virtual size_t get_count() const noexcept = 0;

		ML_NODISCARD virtual buffer_layout const & get_layout() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_usage() const noexcept = 0;

	protected:
		virtual bool do_is_equal(device_resource const & other) const noexcept override = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gfx
{
	// base indexbuffer
	class ML_GRAPHICS_API indexbuffer : public device_resource
	{
	public:
		ML_NODISCARD static shared<indexbuffer> create(uint32_t usage, size_t count, address_t data) noexcept;

		ML_NODISCARD static shared<indexbuffer> create(size_t count, address_t data = nullptr) noexcept
		{
			return create(data ? usage_static_draw : usage_dynamic_draw, count, data);
		}

	public:
		virtual ~indexbuffer() override = default;

		virtual bool invalidate() override = 0;

		ML_NODISCARD virtual resource_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof<> const & get_type_info() const noexcept override = 0;

	public:
		static void bind(indexbuffer const * value) noexcept;

		static void bind(shared<indexbuffer> const & value) noexcept
		{
			bind(static_cast<indexbuffer const *>(value.get()));
		}

		inline void bind() const noexcept { bind(this); }

		inline void unbind() const noexcept { bind(nullptr); }

	public:
		virtual void set_data(size_t count, address_t data, size_t offset = 0) = 0;

		ML_NODISCARD virtual buffer_t const & get_buffer() const noexcept = 0;

		ML_NODISCARD virtual size_t get_count() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_usage() const noexcept = 0;

	protected:
		virtual bool do_is_equal(device_resource const & other) const noexcept override = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gfx
{
	// base vertexarray
	class ML_GRAPHICS_API vertexarray : public device_resource
	{
	public:
		ML_NODISCARD static shared<vertexarray> create(uint32_t mode = primitive_triangles) noexcept;

	public:
		virtual ~vertexarray() override = default;

		virtual bool invalidate() override = 0;

		ML_NODISCARD virtual resource_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof<> const & get_type_info() const noexcept override = 0;

	public:
		static void bind(vertexarray const * value) noexcept;

		static void bind(shared<vertexarray> const & value) noexcept
		{
			bind(static_cast<vertexarray const *>(value.get()));
		}

		inline void bind() const noexcept { bind(this); }

		inline void unbind() const noexcept { bind(nullptr); }

	public:
		virtual void add_vertices(shared<vertexbuffer> const & value) = 0;

		virtual void set_indices(shared<indexbuffer> const & value) = 0;

		ML_NODISCARD virtual shared<indexbuffer> const & get_indices() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_mode() const noexcept = 0;
		
		ML_NODISCARD virtual pmr::vector<shared<vertexbuffer>> const & get_vertices() const noexcept = 0;

	protected:
		virtual bool do_is_equal(device_resource const & other) const noexcept override = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture
namespace ml::gfx
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// texture flags
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

	// texture format
	struct ML_NODISCARD texfmt final
	{
		uint32_t
			color	{ format_rgba },		// color format
			pixel	{ color },				// pixel format
			type	{ type_unsigned_byte };	// pixel type
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// texture options
	struct ML_NODISCARD texopts final
	{
		vec2i		size	{ 0, 0 }					; // texture size
		texfmt		format	{ format_rgba }				; // texture format
		int32_t		flags	{ texture_flags_default }	; // texture flags
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD constexpr uint32_t get_channel_format(size_t value) noexcept
	{
		switch (value)
		{
		default	: return 0;
		case 1	: return format_red;
		case 3	: return format_rgb;
		case 4	: return format_rgba;
		}
	}

	ML_NODISCARD constexpr size_t get_bits_per_pixel(uint32_t value) noexcept
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

	// base texture
	class ML_GRAPHICS_API texture : public device_resource
	{
	public:
		virtual ~texture() override = default;

		virtual bool invalidate() override = 0;

		ML_NODISCARD virtual resource_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof<> const & get_type_info() const noexcept override = 0;

	public:
		static void bind(texture const * value, uint32_t slot = 0) noexcept;

		static void bind(shared<texture> const & value, uint32_t slot = 0) noexcept
		{
			bind(static_cast<texture const *>(value.get()), slot);
		}

		virtual void bind(uint32_t slot = 0) const noexcept = 0;

		virtual void unbind(uint32_t slot = 0) const noexcept = 0;

	public:
		virtual void lock() = 0;

		virtual void unlock() = 0;

		ML_NODISCARD virtual texopts const & get_texopts() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_texture_type() const noexcept = 0;

		ML_NODISCARD inline uint32_t get_color_format() const noexcept { return get_texopts().format.color; }

		ML_NODISCARD inline int32_t get_flags() const noexcept { return get_texopts().flags; }

		ML_NODISCARD inline uint32_t get_pixel_format() const noexcept { return get_texopts().format.pixel; }

		ML_NODISCARD inline uint32_t get_pixel_type() const noexcept { return get_texopts().format.type; }

		ML_NODISCARD inline vec2i const & get_size() const noexcept { return get_texopts().size; }

		ML_NODISCARD inline bool is_mipmapped() const noexcept { return get_texopts().flags & texture_flags_mipmapped; }

		ML_NODISCARD inline bool is_repeated() const noexcept { return get_texopts().flags & texture_flags_repeated; }

		ML_NODISCARD inline bool is_smooth() const noexcept { return get_texopts().flags & texture_flags_smooth; }

	protected:
		virtual bool do_is_equal(device_resource const & other) const noexcept override = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gfx
{
	// base texture2d
	class ML_GRAPHICS_API texture2d : public texture
	{
	public:
		ML_NODISCARD static shared<texture2d> create(texopts const & opts, address_t data = nullptr) noexcept;

		ML_NODISCARD static shared<texture2d> create(image const & img, int32_t flags = texture_flags_default) noexcept
		{
			return create({ img.size(), { get_channel_format(img.channels()) }, flags }, img.data());
		}

		ML_NODISCARD static shared<texture2d> create(fs::path const & path, int32_t flags = texture_flags_default) noexcept
		{
			return create(image{ path }, flags);
		}

	public:
		virtual ~texture2d() override = default;

		virtual bool invalidate() override = 0;

		ML_NODISCARD virtual resource_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof<> const & get_type_info() const noexcept override = 0;

	public:
		static void bind(texture2d const * value, uint32_t slot = 0) noexcept;

		static void bind(shared<texture2d> const & value, uint32_t slot = 0) noexcept
		{
			bind(static_cast<texture2d const *>(value.get()), slot);
		}

		inline void bind(uint32_t slot = 0) const noexcept override { bind(this, slot); }

		inline void unbind(uint32_t slot = 0) const noexcept override { bind(nullptr, slot); }

	public:
		virtual void lock() override = 0;

		virtual void unlock() override = 0;

		virtual void update(vec2i const & size, address_t data = nullptr) = 0;

		virtual void update(vec2i const & pos, vec2i const & size, address_t data) = 0;

		virtual void set_mipmapped(bool value) = 0;

		virtual void set_repeated(bool value) = 0;

		virtual void set_smooth(bool value) = 0;

		ML_NODISCARD virtual image copy_to_image() const = 0;

		ML_NODISCARD virtual texopts const & get_texopts() const noexcept override = 0;

		ML_NODISCARD inline uint32_t get_texture_type() const noexcept { return texture_type_2d; }

	protected:
		virtual bool do_is_equal(device_resource const & other) const noexcept override = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texturecube
namespace ml::gfx
{
	// base texturecube
	class ML_GRAPHICS_API texturecube : public texture
	{
	public:
		ML_NODISCARD static shared<texturecube> create(texopts const & opts) noexcept;

	public:
		virtual ~texturecube() override = default;

		virtual bool invalidate() override = 0;

		ML_NODISCARD virtual resource_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof<> const & get_type_info() const noexcept override = 0;

	public:
		static void bind(texturecube const * value, uint32_t slot = 0) noexcept;

		static void bind(shared<texturecube> const & value, uint32_t slot = 0) noexcept
		{
			bind(static_cast<texturecube const *>(value.get()), slot);
		}

		inline void bind(uint32_t slot = 0) const noexcept override { bind(this, slot); }

		inline void unbind(uint32_t slot = 0) const noexcept override { bind(nullptr, slot); }

	public:
		virtual void lock() override = 0;

		virtual void unlock() override = 0;

		ML_NODISCARD virtual texopts const & get_texopts() const noexcept override = 0;

		ML_NODISCARD inline uint32_t get_texture_type() const noexcept override { return texture_type_cubemap; }

	protected:
		virtual bool do_is_equal(device_resource const & other) const noexcept override = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gfx
{
	// base framebuffer
	class ML_GRAPHICS_API framebuffer : public device_resource
	{
	public:
		ML_NODISCARD static shared<framebuffer> create(texopts const & opts);

	public:
		virtual ~framebuffer() override = default;

		virtual bool invalidate() override = 0;

		ML_NODISCARD virtual resource_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof<> const & get_type_info() const noexcept override = 0;

	public:
		static void bind(framebuffer const * value) noexcept;

		static void bind(shared<framebuffer> const & value) noexcept
		{
			bind(static_cast<framebuffer const *>(value.get()));
		}

		inline void bind() const noexcept { bind(this); }

		inline void unbind() const noexcept { bind(nullptr); }

	public:
		virtual bool attach(shared<texture2d> const & value) = 0;

		virtual bool detach(shared<texture2d> const & value) = 0;

		virtual void resize(vec2i const & value) = 0;

		ML_NODISCARD virtual tex_buffer_t const & get_color_attachments() const noexcept = 0;

		ML_NODISCARD virtual shared<texture2d> const & get_depth_attachment() const noexcept = 0;

		ML_NODISCARD virtual texopts const & get_texopts() const noexcept = 0;

		inline void bind_texture(uint32_t slot = 0) const noexcept
		{
			texture2d::bind(get_color_attachments()[(size_t)slot], slot);
		}

	protected:
		virtual bool do_is_equal(device_resource const & other) const noexcept override = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader
namespace ml::gfx
{
	// shader flags
	enum shader_flags_ : int32_t
	{
		shader_flags_none, // none

		// none
		shader_flags_default
			= shader_flags_none,
	};

	// base shader
	class ML_GRAPHICS_API shader : public device_resource
	{
	public:
		ML_NODISCARD static shared<shader> create(uint32_t type, int32_t flags = shader_flags_default) noexcept;

		template <class ... Args
		> ML_NODISCARD static shared<shader> create(uint32_t type, Args && ... args) noexcept
		{
			auto temp{ create(type) };
			temp->compile(ML_forward(args)...);
			return temp;
		}

	public:
		virtual ~shader() override = default;

		virtual bool invalidate() override = 0;

		ML_NODISCARD virtual resource_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof<> const & get_type_info() const noexcept override = 0;

	public:
		virtual bool compile(shader_src_t const & src) = 0;

		inline bool compile(size_t count, cstring const * src) noexcept
		{
			return compile(shader_src_t{ src, src + count });
		}

		inline bool compile(pmr::string const & src) noexcept
		{
			return compile(shader_src_t{ src });
		}

		inline bool compile(cstring src) noexcept
		{
			return compile(shader_src_t{ src });
		}

		ML_NODISCARD virtual pmr::string const & get_error_log() const noexcept = 0;

		ML_NODISCARD virtual int32_t get_flags() const noexcept = 0;

		ML_NODISCARD virtual uint32_t get_shader_type() const noexcept = 0;

		ML_NODISCARD virtual shader_src_t const & get_source() const noexcept = 0;

	protected:
		virtual bool do_is_equal(device_resource const & other) const noexcept override = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gfx
{
	// base program
	class ML_GRAPHICS_API program : public device_resource
	{
	public:
		ML_NODISCARD static shared<program> create() noexcept;

		ML_NODISCARD static shared<program> create(pmr::vector<shared<shader>> const & vs) noexcept
		{
			auto temp{ create() };
			for (auto const & pgm : vs) { temp->attach(pgm); }
			temp->link();
			return temp;
		}
		
	public:
		virtual ~program() override = default;

		virtual bool invalidate() override = 0;

		ML_NODISCARD virtual resource_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof<> const & get_type_info() const noexcept override = 0;

	public:
		static void bind(program const * value) noexcept;

		static void bind(shared<program> const & value) noexcept
		{
			bind(static_cast<program const *>(value.get()));
		}

		inline void bind() const noexcept { bind(this); }

		inline void unbind() const noexcept { bind(nullptr); }

	public:
		virtual bool attach(shared<shader> const & value) = 0;

		virtual bool detach(shared<shader> const & value) = 0;

		virtual bool link() = 0;

		virtual bool bind_uniform(cstring name, uni_binder_t const & fn) = 0;

		inline void bind_textures() const
		{
			uint32_t slot{};
			get_textures().for_each([&](uniform_id loc, shared<texture> const & tex) noexcept
			{
				texture::bind(tex, slot);

				device::get_current_context()->upload(loc, (int32_t)slot++);
			});
		}

		template <class T> bool set_uniform(cstring name, T && value) noexcept
		{
			return bind_uniform(name, [&](uniform_id loc) noexcept
			{
				if constexpr (std::is_convertible_v<T, shared<texture>>)
				{
					do_cache_texture(loc, ML_forward(value));
				}
				else
				{
					device::get_current_context()->upload(loc, ML_forward(value));
				}
			});
		}

		ML_NODISCARD virtual pmr::string const & get_error_log() const noexcept = 0;

		ML_NODISCARD virtual shader_map_t const & get_shaders() const noexcept = 0;

		ML_NODISCARD virtual tex_cache_t const & get_textures() const noexcept = 0;

		ML_NODISCARD virtual uni_cache_t const & get_uniforms() const noexcept = 0;

	protected:
		virtual bool do_is_equal(device_resource const & other) const noexcept override = 0;

		virtual void do_cache_texture(uniform_id loc, shared<texture> const & value) noexcept = 0;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RENDER_API_HPP_