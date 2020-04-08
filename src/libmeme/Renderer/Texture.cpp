#include <libmeme/Renderer/Texture.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static void set_mipmapped(bool value, uint32_t sampler, bool smooth)
	{
		if (value) GL::generateMipmap(sampler);
		GL::texParameter(sampler, GL::TexMagFilter, value
			? smooth ? GL::LinearMipmapLinear : GL::NearestMipmapLinear
			: smooth ? GL::Linear : GL::Nearest
		);
	}

	static void set_repeated(bool value, uint32_t sampler)
	{
		GL::texParameter(sampler, GL::TexWrapS, value
			? GL::Repeat
			: (GL::edgeClampAvailable() ? GL::ClampToEdge : GL::Clamp)
		);
		GL::texParameter(sampler, GL::TexWrapT, value
			? GL::Repeat
			: (GL::edgeClampAvailable() ? GL::ClampToEdge : GL::Clamp)
		);
	}

	static void set_smooth(bool value, uint32_t sampler, bool mipmapped)
	{
		GL::texParameter(sampler, GL::TexMagFilter,
			value ? GL::Linear : GL::Nearest
		);
		GL::texParameter(sampler, GL::TexMinFilter, mipmapped
			? value ? GL::LinearMipmapLinear : GL::NearestMipmapLinear
			: value ? GL::Linear : GL::Nearest
		);
	}

	static bool refresh_flags(texture & t) noexcept
	{
		if (!t) return false;

		impl::set_repeated(t.is_repeated(), t.sampler());

		impl::set_smooth(t.is_smooth(), t.sampler(), t.is_mipmapped());

		impl::set_mipmapped(t.is_mipmapped(), t.sampler(), t.is_smooth());

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	texture::texture(uint32_t sampler, int32_t level, uint32_t internal_format, uint32_t color_format, uint32_t pixel_type, int32_t flags)
		: m_handle		{ NULL }
		, m_sampler		{ sampler }
		, m_level		{ level }
		, m_intl_format	{ internal_format }
		, m_col_format	{ color_format }
		, m_pixel_type	{ pixel_type }
		, m_size		{}
		, m_real_size	{}
		, m_flags		{ flags }
	{
	}

	texture::texture(uint32_t sampler, uint32_t internal_format, uint32_t color_format, int32_t flags)
		: texture{ sampler, 0, internal_format, color_format, GL::UnsignedByte, flags }
	{
	}

	texture::texture(uint32_t sampler, uint32_t format, int32_t flags)
		: texture{ sampler, 0, format, format, GL::UnsignedByte, flags }
	{
	}

	texture::texture(uint32_t sampler, int32_t flags)
		: texture{ sampler, GL::RGBA, flags }
	{
	}

	texture::texture(uint32_t sampler)
		: texture{ sampler, texture_flags_default }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	texture::texture()
		: texture{ GL::Texture2D }
	{
	}

	texture::texture(fs::path const & path)
		: texture{}
	{
		load_from_file(path);
	}

	texture::texture(image const & image)
		: texture{}
	{
		load_from_image(image);
	}

	texture::texture(texture const & other) : texture{
		other.m_sampler,
		other.m_level,
		other.m_intl_format,
		other.m_col_format,
		other.m_pixel_type,
		other.m_flags
	}
	{
		load_from_texture(other);
	}

	texture::texture(texture && other) noexcept
		: texture{}
	{
		swap(std::move(other));
	}

	texture::~texture()
	{
		destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool texture::load_from_file(fs::path const & path)
	{
		return load_from_image(make_image(path));
	}

	bool texture::load_from_image(image const & img)
	{
		if (!img.channels()) { return false; }

		m_intl_format = m_col_format = img.get_format();

		return create(img.data(), img.size());
	}

	bool texture::load_from_texture(texture const & other)
	{
		return load_from_image(other.copy_to_image());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool texture::generate()
	{
		return !m_handle && (m_handle = GL::genTexture());
	}

	bool texture::destroy()
	{
		unbind();
		if (m_handle)
		{
			GL::deleteTexture(&m_handle);
			m_handle = NULL;
			GL::flush();
		}
		return !(m_handle);
	}

	void texture::bind(texture const * value)
	{
		if (!value || !value->m_handle)
		{
			GL::bindTexture(GL::Texture2D, NULL);
		}
		else
		{
			GL::bindTexture(value->m_sampler, value->m_handle);
		}
	}

	void texture::swap(texture & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_handle,			other.m_handle);
			std::swap(m_sampler,		other.m_sampler);
			std::swap(m_level,			other.m_level);
			std::swap(m_intl_format,	other.m_intl_format);
			std::swap(m_col_format,		other.m_col_format);
			std::swap(m_pixel_type,		other.m_pixel_type);
			std::swap(m_size,			other.m_size);
			std::swap(m_real_size,		other.m_real_size);
			std::swap(m_flags,			other.m_flags);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool texture::create(vec2u const & size)
	{
		return create(nullptr, size);
	}

	bool texture::create(byte_t const * pixels, vec2u const & size)
	{
		return create(pixels, size[0], size[1]);
	}

	bool texture::create(byte_t const * pixels, uint32_t w, uint32_t h)
	{
		// size cannot be zero
		if (!w || !h)
		{
			return debug::log::error("texture size cannot be zero");
		}

		// failed creating handle
		if (!destroy() || !generate())
		{
			return debug::log::error("texture failed generating handle");
		}

		// set size
		m_size = { w, h };
		m_real_size = {
			GL::getValidTextureSize(m_size[0]),
			GL::getValidTextureSize(m_size[1])
		};

		// validate size
		static auto const max_size{ GL::getMaxTextureSize() };
		if ((m_real_size[0] > max_size) || (m_real_size[1] > max_size))
		{
			return debug::log::error("texture size is too large ({0}) max is: {1}",
				m_real_size, vec2u{ max_size , max_size }
			);
		}

		ML_defer{ GL::flush(); };

		ML_bind_scope(*this);

		GL::texImage2D(
			m_sampler,
			m_level,
			m_intl_format,
			m_size[0],
			m_size[1],
			0, // "border must be 0" -khronos
			m_col_format,
			m_pixel_type,
			(void *)pixels);

		impl::refresh_flags(*this);

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool texture::update(byte_t const * pixels, uint_rect const & area)
	{
		return update(pixels, area.position(), area.size());
	}

	bool texture::update(byte_t const * pixels, vec2u const & pos, vec2u const & size)
	{
		return update(pixels, pos[0], pos[1], size[0], size[1]);
	}

	bool texture::update(byte_t const * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		if (!m_handle)
		{
			return debug::log::error("texture handle not set");
		}

		if (!w || !h)
		{
			return debug::log::error("texture size cannot be zero");
		}

		ML_defer{ GL::flush(); };

		ML_bind_scope(*this);

		GL::texSubImage2D(
			m_sampler,
			m_level,
			x, y, w, h,
			m_intl_format,
			m_pixel_type,
			(void *)pixels);

		impl::refresh_flags(*this);
		
		return true;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t texture::channels() const noexcept
	{
		switch (m_intl_format)
		{
		case GL::Red: return 1;
		case GL::RGB: return 3;
		case GL::RGBA: return 4;
		}
		return 0;
	}

	image texture::copy_to_image() const
	{
		auto temp{ make_image(size(), channels()) };
		if (m_handle)
		{
			ML_bind_scope(*this);

			GL::getTexImage(
				GL::Texture2D,
				m_level,
				m_intl_format,
				m_pixel_type,
				temp.data());
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool texture::set_mipmapped(bool value)
	{
		if (!m_handle) { return false; }
		else
		{
			m_flags = value
				? (m_flags | texture_flags_mipmapped)
				: (m_flags & ~texture_flags_mipmapped);

			ML_defer{ GL::flush(); };

			ML_bind_scope(*this);

			impl::set_mipmapped(value, m_sampler, is_smooth());
			
			return true;
		}
	}

	bool texture::set_repeated(bool value)
	{
		if (!m_handle) { return false; }
		else
		{
			m_flags = value
				? (m_flags | texture_flags_repeated)
				: (m_flags & ~texture_flags_repeated);

			ML_defer{ GL::flush(); };

			ML_bind_scope(*this);

			impl::set_repeated(value, m_sampler);

			return true;
		}
	}

	bool texture::set_smooth(bool value)
	{
		if (!m_handle) { return false; }
		else
		{
			m_flags = value
				? (m_flags | texture_flags_smooth)
				: (m_flags & ~texture_flags_smooth);

			ML_defer{ GL::flush(); };

			ML_bind_scope(*this);

			impl::set_smooth(value, m_sampler, is_mipmapped());
			
			return true;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}