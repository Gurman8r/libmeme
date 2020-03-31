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
		, m_size		{ 0, 0 }
		, m_real_size	{ 0, 0 }
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

	texture::texture(filesystem::path const & path)
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

	texture::~texture() { destroy(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool texture::load_from_file(filesystem::path const & path)
	{
		return load_from_image(make_image(path));
	}

	bool texture::load_from_image(image const & image)
	{
		if (!image.channels()) { return false; }

		m_intl_format = m_col_format = image.get_format();

		return create(image.size()) && update(image);
	}

	bool texture::load_from_texture(texture const & other)
	{
		return other.m_handle
			? create(other.size())
				? update(other.copy_to_image())
				: debug::log::error("Failed to copy texture, failed to create new texture")
			: false;
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
		if (value && value->m_handle)
		{
			GL::bindTexture(value->m_sampler, value->m_handle);
		}
		else
		{
			GL::bindTexture(GL::Texture2D, NULL);
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
			std::swap(m_col_format,	other.m_col_format);
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

	bool texture::create(image const & image, vec2u const & size)
	{
		return create(image.data(), size);
	}

	bool texture::create(image const & image, uint32_t w, uint32_t h)
	{
		return create(image.data(), w, h);
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
		static auto const max_size
		{
			GL::getMaxTextureSize()
		};
		if ((m_real_size[0] > max_size) || (m_real_size[1] > max_size))
		{
			return debug::log::error("texture size is too large ({0}) max is: {1}",
				m_real_size, vec2u{ max_size , max_size }
			);
		}

		// create
		{
			ML_BIND_SCOPE((*this));

			GL::texImage2D(
				m_sampler,
				m_level,
				m_intl_format,
				m_size[0],
				m_size[1],
				0, // border: "This value must be 0" -khronos.org
				m_col_format,
				m_pixel_type,
				(void *)pixels
			);

			impl::set_repeated(is_repeated(), m_sampler);
			
			impl::set_smooth(is_smooth(), m_sampler, is_mipmapped());
			
			impl::set_mipmapped(is_mipmapped(), m_sampler, is_smooth());
		}
		GL::flush();
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool texture::update(texture const & other, uint_rect const & area)
	{
		return update(other.copy_to_image(), area);
	}

	bool texture::update(texture const & other, vec2u const & position, vec2u const & size)
	{
		return update(other.copy_to_image(), position, size);
	}

	bool texture::update(texture const & other, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		return update(other.copy_to_image(), x, y, w, h);
	}

	bool texture::update(image const & image)
	{
		return update(image.data(), image.bounds());
	}

	bool texture::update(image const & image, uint_rect const & area)
	{
		return update(image.data(), area.position(), area.size());
	}

	bool texture::update(image const & image, vec2u const & position, vec2u const & size)
	{
		return update(image.data(), position[0], position[1], size[0], size[1]);
	}

	bool texture::update(image const & image, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		return update(image.data(), x, y, w, h);
	}

	bool texture::update(byte_t const * pixels)
	{
		return update(pixels, uint_rect{ width(), height() });
	}

	bool texture::update(byte_t const * pixels, uint_rect const & area)
	{
		return update(pixels, area.position(), area.size());
	}

	bool texture::update(byte_t const * pixels, vec2u const & position, vec2u const & size)
	{
		return update(pixels, position[0], position[1], size[0], size[1]);
	}

	bool texture::update(byte_t const * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		if (!w || !h || !pixels)
		{
			return debug::log::error("Failed updating texture, invalid size: {0}", vec2u{ w, h });
		}

		// update
		{
			ML_BIND_SCOPE((*this));

			GL::texSubImage2D(
				m_sampler,
				m_level,
				x, y, w, h,
				m_intl_format,
				m_pixel_type,
				(void *)pixels
			);

			impl::set_repeated(is_repeated(), m_sampler);

			impl::set_smooth(is_smooth(), m_sampler, is_mipmapped());

			impl::set_mipmapped(is_mipmapped(), m_sampler, is_smooth());
		}
		GL::flush();
		return true;
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

			ML_BIND_SCOPE((*this));

			impl::set_mipmapped(value, m_sampler, is_smooth());
		}
		GL::flush();
		return true;
	}

	bool texture::set_repeated(bool value)
	{
		if (!m_handle) { return false; }
		else
		{
			m_flags = value
				? (m_flags | texture_flags_repeated)
				: (m_flags & ~texture_flags_repeated);

			ML_BIND_SCOPE((*this));

			impl::set_repeated(value, m_sampler);
		}
		GL::flush();
		return true;
	}

	bool texture::set_smooth(bool value)
	{
		if (!m_handle) { return false; }
		else
		{
			m_flags = value
				? (m_flags | texture_flags_smooth)
				: (m_flags & ~texture_flags_smooth);

			ML_BIND_SCOPE((*this));

			impl::set_smooth(value, m_sampler, is_mipmapped());
		}
		GL::flush();
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
			ML_BIND_SCOPE((*this));
			
			GL::getTexImage(
				GL::Texture2D,
				m_level,
				m_intl_format,
				m_pixel_type,
				&temp[0]
			);
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}