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

	constexpr cstring error_names[] =
	{
		"none",
		"invalid enum",
		"invalid value",
		"invalid operation",
		"stack overflow",
		"stack underflow",
		"out of memory",
		"invalid framebuffer operation",
		"context lost",
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

		shader_type_MAX
	};

	constexpr cstring shader_type_names[shader_type_MAX] =
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
		texture_type_cube_map,

		texture_type_MAX
	};

	constexpr cstring texture_type_names[texture_type_MAX] =
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
	ML_NODISCARD constexpr uint32_t channels_to_format(size_t value) noexcept
	{
		switch (value)
		{
		default	: return 0;
		case 1	: return gl::format_red;
		case 3	: return gl::format_rgb;
		case 4	: return gl::format_rgba;
		}
	}

	ML_NODISCARD constexpr size_t format_to_channels(uint32_t value) noexcept
	{
		switch (value)
		{
		default				: return 0;
		case gl::format_red	: return 1;
		case gl::format_rgb	: return 3;
		case gl::format_rgba: return 4;
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// layout
namespace ml
{
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
			for (uint32_t i = 0, imax = this->size(); i < imax; ++i)
			{
				std::invoke(ML_forward(fn), i, m_elements[(size_t)i]);
			}
		}

		template <class Fn
		> void for_each(Fn && fn) const noexcept
		{
			for (uint32_t i = 0, imax = this->size(); i < imax; ++i)
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

// objects
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class vertexarray		; // 
	class vertexbuffer		; // 
	class indexbuffer		; // 
	class framebuffer		; // 
	class shader_object		; // 
	class texture2d			; // 

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vertex array
	class ML_GRAPHICS_API vertexarray : public trackable, public non_copyable
	{
	public:
		virtual ~vertexarray() = default;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual void add_vbo(shared<vertexbuffer> const & value) = 0;

		virtual void set_ibo(shared<indexbuffer> const & value) = 0;

		ML_NODISCARD virtual gl::handle get_handle() const = 0;

		ML_NODISCARD virtual shared<indexbuffer> const & get_ibo() const = 0;
		
		ML_NODISCARD virtual pmr::vector<shared<vertexbuffer>> const & get_vbos() const = 0;

		ML_NODISCARD static shared<vertexarray> create();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vertex buffer
	class ML_GRAPHICS_API vertexbuffer : public trackable, public non_copyable
	{
	public:
		virtual ~vertexbuffer() = default;

		virtual void bind() const = 0;
		
		virtual void unbind() const = 0;

		virtual void set_data(gl::buffer vertices, uint32_t count, uint32_t offset = 0) = 0;

		virtual void set_layout(buffer_layout const & value) = 0;

		ML_NODISCARD virtual uint32_t get_count() const = 0;

		ML_NODISCARD virtual gl::handle get_handle() const = 0;

		ML_NODISCARD virtual buffer_layout const & get_layout() const = 0;

		ML_NODISCARD static shared<vertexbuffer> create(
			gl::buffer	vertices,
			uint32_t	count,
			uint32_t	usage = gl::usage_static_draw);

		ML_NODISCARD static shared<vertexbuffer> create(
			uint32_t count,
			uint32_t usage = gl::usage_dynamic_draw);

		ML_NODISCARD static shared<vertexbuffer> create(
			pmr::vector<float_t> const & vertices, uint32_t usage = gl::usage_static_draw)
		{
			return create(vertices.data(),
				(uint32_t)vertices.size() * sizeof(float_t), usage);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// index buffer
	class ML_GRAPHICS_API indexbuffer : public trackable, public non_copyable
	{
	public:
		virtual ~indexbuffer() = default;

		virtual void bind() const = 0;
		
		virtual void unbind() const = 0;

		virtual void set_data(gl::buffer indices, uint32_t count) = 0;

		ML_NODISCARD virtual uint32_t get_count() const = 0;

		ML_NODISCARD virtual gl::handle get_handle() const = 0;

		ML_NODISCARD static shared<indexbuffer> create(
			gl::buffer	indices,
			uint32_t	count);

		ML_NODISCARD static shared<indexbuffer> create(pmr::vector<uint32_t> const & indices)
		{
			return create(indices.data(), (uint32_t)indices.size());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// frame buffer
	class ML_GRAPHICS_API framebuffer : public trackable, public non_copyable
	{
	public:
		virtual ~framebuffer() = default;

		virtual void bind() const = 0;
		
		virtual void unbind() const = 0;

		virtual void bind_texture(uint32_t slot = 0) const = 0;

		virtual void resize(vec2i const & value) = 0;

		ML_NODISCARD virtual shared<texture2d> const & get_color_attachment() const = 0;

		ML_NODISCARD virtual shared<texture2d> const & get_depth_attachment() const = 0;

		ML_NODISCARD virtual uint32_t get_format() const = 0;

		ML_NODISCARD virtual gl::handle get_handle() const = 0;

		ML_NODISCARD virtual vec2i get_size() const = 0;

		ML_NODISCARD static shared<framebuffer> create(
			uint32_t		format,
			vec2i const &	size);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// texture2d
	class ML_GRAPHICS_API texture2d : public trackable, public non_copyable
	{
	public:
		virtual ~texture2d() = default;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual void update(vec2i const & size, gl::buffer pixels = nullptr) = 0;

		virtual void set_mipmapped(bool value) = 0;

		virtual void set_repeated(bool value) = 0;

		virtual void set_smooth(bool value) = 0;

		ML_NODISCARD virtual image copy_to_image() const = 0;

		ML_NODISCARD virtual int32_t get_flags() const = 0;

		ML_NODISCARD virtual gl::handle get_handle() const = 0;

		ML_NODISCARD virtual vec2i const & get_size() const = 0;

		ML_NODISCARD inline bool is_mipmapped() const noexcept { return get_flags() & gl::texture_flags_mipmapped; }

		ML_NODISCARD inline bool is_repeated() const noexcept { return get_flags() & gl::texture_flags_repeated; }

		ML_NODISCARD inline bool is_smooth() const noexcept { return get_flags() & gl::texture_flags_smooth; }

		ML_NODISCARD static shared<texture2d> create(
			vec2i const &	size,
			uint32_t		iformat,
			uint32_t		cformat,
			uint32_t		ptype = gl::type_unsigned_byte,
			int32_t			flags = gl::texture_flags_default,
			gl::buffer		pixels = nullptr
		);

		ML_NODISCARD static shared<texture2d> create(image const & img, int32_t flags = gl::texture_flags_default) noexcept
		{
			uint32_t const fmt{ gl::channels_to_format(img.channels()) };
			return create(img.size(), fmt, fmt, gl::type_unsigned_byte, flags, img.data());
		}

		ML_NODISCARD static shared<texture2d> create(fs::path const & path, int32_t flags = gl::texture_flags_default) noexcept
		{
			return create(image{ path }, flags);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// shader
	class ML_GRAPHICS_API shader_object : public trackable, public non_copyable
	{
	public:
		virtual ~shader_object() = default;

		virtual void bind(bool bind_textures = true) const = 0;
		
		virtual void unbind() const = 0;

		ML_NODISCARD virtual bool set_uniform(cstring name, bool value) = 0;

		ML_NODISCARD virtual bool set_uniform(cstring name, int32_t value) = 0;

		ML_NODISCARD virtual bool set_uniform(cstring name, float_t value) = 0;

		ML_NODISCARD virtual bool set_uniform(cstring name, vec2 const & value) = 0;

		ML_NODISCARD virtual bool set_uniform(cstring name, vec3 const & value) = 0;

		ML_NODISCARD virtual bool set_uniform(cstring name, vec4 const & value) = 0;

		ML_NODISCARD virtual bool set_uniform(cstring name, mat2 const & value) = 0;

		ML_NODISCARD virtual bool set_uniform(cstring name, mat3 const & value) = 0;

		ML_NODISCARD virtual bool set_uniform(cstring name, mat4 const & value) = 0;

		ML_NODISCARD virtual bool set_uniform(cstring name, shared<texture2d> const & value) = 0;

		ML_NODISCARD virtual gl::handle get_handle() const = 0;

		ML_NODISCARD static shared<shader_object> create(
			cstring v_src,
			cstring f_src,
			cstring g_src = nullptr);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// states
namespace ml
{
	struct ML_NODISCARD alpha_fn final
	{
		uint32_t func;
		float_t ref;
	};

	struct ML_NODISCARD blend_eq final
	{
		uint32_t
			modeRGB = gl::function_add,
			modeAlpha = modeRGB;
	};

	struct ML_NODISCARD blend_fn final
	{
		uint32_t
			sfactorRGB = gl::factor_src_alpha,
			dfactorRGB = gl::factor_one_minus_src_alpha,
			sfactorAlpha = sfactorRGB,
			dfactorAlpha = dfactorRGB;
	};

	struct ML_NODISCARD depth_range final
	{
		float_t
			nearVal = 0.f,
			farVal = 1.f;
	};

	struct ML_NODISCARD stencil_fn final
	{
		uint32_t pred	{ gl::predicate_always };
		int32_t  ref	{ 0 };
		uint32_t mask	{ static_cast<uint32_t>(-1) };
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// api
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// render api capabilities
	struct ML_NODISCARD render_api_info final
	{
		int32_t major_version, minor_version;
		pmr::string renderer, vendor, version, shading_language_version;
		pmr::vector<pmr::string> extensions;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// render api
	class ML_GRAPHICS_API render_api : public trackable, public non_copyable
	{
	protected:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_api() noexcept = default;

		virtual bool do_initialize() = 0;

		virtual void on_initialize() = 0;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static render_api * const get() noexcept;

		inline bool initialize() noexcept
		{
			bool temp{};
			static ML_scope // once
			{
				if (temp = this->do_initialize())
				{
					this->on_initialize();
				}
			};
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual uint32_t get_error() const = 0;

		virtual render_api_info const & get_info() const = 0;

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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_API_HPP_