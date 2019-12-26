#include <libmeme/Renderer/Texture.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture::Texture()
		: Texture{ GL::Texture2D }
	{
	}

	Texture::Texture(uint32_t sampler)
		: Texture{ sampler, TextureFlags_Smooth | TextureFlags_Repeated | TextureFlags_Mipmapped }
	{
	}

	Texture::Texture(uint32_t sampler, int32_t flags)
		: Texture{ sampler, GL::RGBA, flags }
	{
	}

	Texture::Texture(uint32_t sampler, uint32_t format, int32_t flags)
		: Texture{ sampler, 0, format, format, GL::UnsignedByte, flags }
	{
	}

	Texture::Texture(uint32_t sampler, uint32_t internalFormat, uint32_t colorFormat, int32_t flags)
		: Texture{ sampler, 0, internalFormat, colorFormat, GL::UnsignedByte, flags }
	{
	}

	Texture::Texture(uint32_t sampler, int32_t level, uint32_t internalFormat, uint32_t colorFormat, uint32_t pixelType, int32_t flags)
		: m_handle{ NULL }
		, m_sampler{ sampler }
		, m_level{ level }
		, m_internalFormat{ internalFormat }
		, m_colorFormat{ colorFormat }
		, m_pixelType{ pixelType }
		, m_size{ 0, 0 }
		, m_realSize{ 0, 0 }
		, m_flags{ flags }
	{
	}

	Texture::Texture(path_t const & filename)
		: Texture{}
	{
		loadFromFile(filename);
	}

	Texture::Texture(Image const & image)
		: Texture{}
	{
		loadFromImage(image);
	}

	Texture::Texture(Texture const & other)
		: m_handle{ NULL }
		, m_sampler{ other.m_sampler }
		, m_level{ other.m_level }
		, m_internalFormat{ other.m_internalFormat }
		, m_colorFormat{ other.m_colorFormat }
		, m_pixelType{ other.m_pixelType }
		, m_size{ 0, 0 }
		, m_realSize{ 0, 0 }
		, m_flags{ other.m_flags }
	{
		loadFromTexture(other);
	}

	Texture::~Texture() { destroy(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture & Texture::operator=(Texture const & other)
	{
		Texture temp{ other };
		swap(temp);
		return (*this);
	}

	void Texture::swap(Texture & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_handle,			other.m_handle);
			std::swap(m_sampler,		other.m_sampler);
			std::swap(m_level,			other.m_level);
			std::swap(m_internalFormat, other.m_internalFormat);
			std::swap(m_colorFormat,	other.m_colorFormat);
			std::swap(m_pixelType,		other.m_pixelType);
			std::swap(m_size,			other.m_size);
			std::swap(m_realSize,		other.m_realSize);
			std::swap(m_flags,			other.m_flags);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Texture::loadFromFile(path_t const & filename)
	{
		Image image;
		return image.loadFromFile(filename) && loadFromImage(image);
	}

	bool Texture::loadFromImage(Image const & image)
	{
		if (!image.channels()) { return false; }

		m_internalFormat = m_colorFormat = image.getFormat();

		return create(image.size()) && update(image);
	}

	bool Texture::loadFromTexture(Texture const & other)
	{
		return (other.handle()
			? (create(other.size())
				? update(other.copyToImage())
				: Debug::logError("Failed to copy texture, failed to create new texture"))
			: false
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool Texture::generate()
	{
		return !m_handle && (m_handle = GL::genTexture());
	}

	bool Texture::destroy()
	{
		Texture::bind(nullptr);

		if (m_handle)
		{
			GL::deleteTexture(&m_handle);
			
			m_handle = NULL;
			
			GL::flush();
		}

		return !(m_handle);
	}

	void Texture::bind(Texture const * value)
	{
		if (value)
		{
			GL::bindTexture(value->m_sampler, value->m_handle);
		}
		else
		{
			GL::bindTexture(GL::Texture2D, NULL);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Texture::create(vec2u const & size)
	{
		return create(nullptr, size);
	}

	bool Texture::create(Image const & image, vec2u const & size)
	{
		return create(image.data(), size);
	}

	bool Texture::create(Image const & image, uint32_t w, uint32_t h)
	{
		return create(image.data(), w, h);
	}

	bool Texture::create(byte_t const * pixels, vec2u const & size)
	{
		return create(pixels, size[0], size[1]);
	}

	bool Texture::create(byte_t const * pixels, uint32_t w, uint32_t h)
	{
		if (w && h)
		{
			if (destroy() && generate())
			{
				m_size = { w, h };
				m_realSize = {
					GL::getValidTextureSize(m_size[0]),
					GL::getValidTextureSize(m_size[1])
				};

				static const uint32_t max_size{ GL::getMaxTextureSize() };

				if ((m_realSize[0] > max_size) || (m_realSize[1] > max_size))
				{
					return Debug::logError(
						"Failed creating texture, size is too large {0} max is {1}",
						m_realSize, 
						vec2u { max_size , max_size }
					);
				}

				if (ML_BIND(Texture, (*this)))
				{
					GL::texImage2D(
						m_sampler,
						m_level,
						m_internalFormat,
						m_size[0],
						m_size[1],
						0, // border: "This value must be 0" -khronos.org
						m_colorFormat,
						m_pixelType,
						(void *)pixels
					);
				}

				set_repeated(repeated());
				
				set_smooth(smooth());
				
				set_mipmapped(mipmapped());
				
				return true;
			}
			return Debug::logError("Failed creating texture, failed setting handle.");
		}
		return Debug::logError("Failed creating texture, invalid size: {0} x {1}", w, h);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool Texture::update(Texture const & other, UintRect const & area)
	{
		return update(other.copyToImage(), area);
	}

	bool Texture::update(Texture const & other, vec2u const & position, vec2u const & size)
	{
		return update(other.copyToImage(), position, size);
	}

	bool Texture::update(Texture const & other, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		return update(other.copyToImage(), x, y, w, h);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Texture::update(Image const & image)
	{
		return update(image.data(), image.bounds());
	}

	bool Texture::update(Image const & image, UintRect const & area)
	{
		return update(image.data(), area.position(), area.size());
	}

	bool Texture::update(Image const & image, vec2u const & position, vec2u const & size)
	{
		return update(image.data(), position[0], position[1], size[0], size[1]);
	}

	bool Texture::update(Image const & image, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		return update(image.data(), x, y, w, h);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Texture::update(byte_t const * pixels)
	{
		return update(pixels, UintRect{ width(), height() });
	}

	bool Texture::update(byte_t const * pixels, UintRect const & area)
	{
		return update(pixels, area.position(), area.size());
	}

	bool Texture::update(byte_t const * pixels, vec2u const & position, vec2u const & size)
	{
		return update(pixels, position[0], position[1], size[0], size[1]);
	}

	bool Texture::update(byte_t const * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		if (w && h && pixels)
		{
			if (ML_BIND(Texture, (*this)))
			{
				GL::texSubImage2D(
					m_sampler,
					m_level,
					x, y, w, h,
					m_internalFormat,
					m_pixelType,
					(void *)pixels
				);
			}
			else
			{
				return Debug::logError("Failed updating texture, failed updating handle.");
			}
			
			set_repeated(repeated());
			
			set_smooth(smooth());
			
			set_mipmapped(mipmapped());
			
			return true;
		}
		return Debug::logError("Failed updating texture, invalid size: {0}", vec2u { w, h });
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t Texture::channels() const noexcept
	{
		switch (m_internalFormat)
		{
		case GL::Red: return 1;
		case GL::RGB: return 3;
		case GL::RGBA: return 4;
		default: return 0;
		}
	}

	Image Texture::copyToImage() const
	{
		Image temp{ size(), channels() };
		if (ML_BIND(Texture, (*this)))
		{
			GL::getTexImage(
				GL::Texture2D,
				m_level,
				m_internalFormat,
				m_pixelType,
				&temp[0]
			);
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Texture::set_repeated(bool value)
	{
		m_flags = value
			? (m_flags & ~TextureFlags_Repeated)
			: (m_flags | TextureFlags_Repeated);

		if (ML_BIND(Texture, (*this)))
		{
			GL::texParameter(m_sampler, GL::TexWrapS, repeated()
				? GL::Repeat
				: (GL::edgeClampAvailable() ? GL::ClampToEdge : GL::Clamp)
			);

			GL::texParameter(m_sampler, GL::TexWrapT, repeated()
				? GL::Repeat
				: (GL::edgeClampAvailable() ? GL::ClampToEdge : GL::Clamp)
			);
		}
		return false;
	}

	bool Texture::set_smooth(bool value)
	{
		m_flags = value
			? (m_flags & ~TextureFlags_Smooth)
			: (m_flags | TextureFlags_Smooth);

		if (ML_BIND(Texture, (*this)))
		{
			GL::texParameter(m_sampler, GL::TexMagFilter,
				smooth() ? GL::Linear : GL::Nearest
			);

			GL::texParameter(m_sampler, GL::TexMinFilter, mipmapped()
				? smooth() ? GL::LinearMipmapLinear : GL::NearestMipmapLinear
				: smooth() ? GL::Linear : GL::Nearest
			);
		}
		return false;
	}

	bool Texture::set_mipmapped(bool value)
	{
		m_flags = value
			? (m_flags & ~TextureFlags_Mipmapped)
			: (m_flags | TextureFlags_Mipmapped);

		if (ML_BIND(Texture, (*this)))
		{
			if (value)
			{
				GL::generateMipmap(m_sampler);
			}

			GL::texParameter(m_sampler, GL::TexMagFilter,
				smooth() ? GL::Linear : GL::Nearest
			);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}