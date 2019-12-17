#include <libmeme/Renderer/Texture.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture::Texture()
		: Texture{ GL::Texture2D, GL::RGBA, TextureFlags_Smooth | TextureFlags_Repeated }
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
		, m_size{}
		, m_realSize{}
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

	Texture::Texture(Texture const & copy)
		: m_handle{ NULL }
		, m_sampler{ copy.m_sampler }
		, m_level{ copy.m_level }
		, m_internalFormat{ copy.m_internalFormat }
		, m_colorFormat{ copy.m_colorFormat }
		, m_pixelType{ copy.m_pixelType }
		, m_size{ copy.m_size }
		, m_realSize{ copy.m_realSize }
		, m_flags{ copy.m_flags }
	{
	}

	Texture::Texture(Texture && copy) noexcept
		: Texture{}
	{
		swap(std::move(copy));
	}

	Texture::~Texture() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture & Texture::operator=(Texture const & other)
	{
		Texture temp{ other };
		swap(temp);
		return (*this);
	}

	Texture & Texture::operator=(Texture && other) noexcept
	{
		swap(std::move(other));
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

	bool Texture::create()
	{
		return !m_handle && (m_handle = GL::genTexture());
	}

	bool Texture::destroy()
	{
		bind(nullptr);
		if (m_handle)
		{
			GL::deleteTexture(&m_handle);
			m_handle = NULL;
			GL::flush();
		}
		return !(m_handle);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Texture::loadFromFile(path_t const & filename)
	{
		Image image;
		return image.loadFromFile(filename) && loadFromImage(image);
	}

	bool Texture::loadFromImage(Image const & value)
	{
		if (!value.channels()) return false;
		m_internalFormat = m_colorFormat = value.getFormat();
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	uint32_t Texture::channels() const noexcept
	{
		switch (m_internalFormat)
		{
		case GL::Red: return 1;
		case GL::RGB: return 3;
		case GL::RGBA: return 4;
		}
		return 0;
	}

	Image Texture::copyToImage() const
	{
		Image temp{ size(), channels() };
		if (m_handle)
		{
			bind(this);

			GL::getTexImage(GL::Texture2D, m_level, m_internalFormat, m_pixelType, &temp[0]);

			bind(nullptr);
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}