#include <libmeme/Renderer/Texture.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture::Texture()
		: Texture{ GL::Texture2D }
	{
	}

	Texture::Texture(uint32_t sampler)
		: Texture{ sampler, TextureFlags_Smooth | TextureFlags_Repeated }
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

	Texture::Texture(Texture const & other)
		: m_handle{ NULL }
		, m_sampler{ other.m_sampler }
		, m_level{ other.m_level }
		, m_internalFormat{ other.m_internalFormat }
		, m_colorFormat{ other.m_colorFormat }
		, m_pixelType{ other.m_pixelType }
		, m_size{ other.m_size }
		, m_realSize{ other.m_realSize }
		, m_flags{ other.m_flags }
	{
		if (other.m_handle)
		{
			loadFromImage(other.copyToImage());
		}
	}

	Texture::Texture(Texture && other) noexcept
		: Texture{}
	{
		swap(std::move(other));
	}

	Texture::~Texture() { destroy(); }

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

	bool Texture::loadFromFile(path_t const & filename)
	{
		Image image;
		return image.loadFromFile(filename) && loadFromImage(image);
	}

	bool Texture::loadFromImage(Image const & image)
	{
		if (!image.channels() || !image.width() || !image.height() || image.pixels().empty())
		{
			return false;
		}

		m_internalFormat = m_colorFormat = image.getFormat();

		return loadFromMemory(image.size(), image.pixels());
	}

	bool Texture::loadFromMemory(vec2s const & size, Image::Pixels const & pixels)
	{
		return loadFromMemory((uint32_t)size[0], (uint32_t)size[1], pixels.data());
	}

	bool Texture::loadFromMemory(uint32_t w, uint32_t h, byte_t const * pixels)
	{
		static const uint32_t max_size
		{
			GL::getMaxTextureSize()
		};

		if (w == 0 || h == 0 || !pixels) { return false; }

		if (!destroy() || !create()) { return false; }

		m_size = { w, h };

		m_realSize = {
			GL::getValidTextureSize(m_size[0]),
			GL::getValidTextureSize(m_size[1])
		};

		if ((m_realSize[0] > max_size) || (m_realSize[1] > max_size))
		{
			return Debug::logError(
				"Failed creating texture, size is too large {0} max is {1}",
				m_realSize, vec2u{ max_size, max_size }
			);
		}

		Texture::bind(this);

		GL::texImage2D(
			m_sampler,
			m_level,
			m_internalFormat,
			m_size[0],
			m_size[1],
			0, // border: "This value must be 0" -khronos.org
			m_colorFormat,
			m_pixelType,
			const_cast<byte_t *>(pixels)
		);

		Texture::bind(nullptr);

		set_repeated(repeated());

		set_smooth(smooth());

		set_mipmapped(mipmapped());

		GL::flush();

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Texture::create()
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
			Texture::bind(this);

			GL::getTexImage(
				GL::Texture2D,
				m_level,
				m_internalFormat,
				m_pixelType,
				temp.data()
			);

			Texture::bind(nullptr);
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Texture::set_repeated(bool value)
	{
		m_flags = value 
			? (m_flags & ~TextureFlags_Repeated)
			: (m_flags | TextureFlags_Repeated);

		if (m_handle)
		{
			Texture::bind(this);
			
			GL::texParameter(m_sampler, GL::TexWrapS, repeated()
				? GL::Repeat
				: (GL::edgeClampAvailable() ? GL::ClampToEdge : GL::Clamp)
			);

			GL::texParameter(m_sampler, GL::TexWrapT, repeated()
				? GL::Repeat
				: (GL::edgeClampAvailable() ? GL::ClampToEdge : GL::Clamp)
			);
			
			Texture::bind(nullptr);
		}
		return false;
	}

	bool Texture::set_smooth(bool value)
	{
		m_flags = value
			? (m_flags & ~TextureFlags_Smooth)
			: (m_flags | TextureFlags_Smooth);

		if (m_handle)
		{
			Texture::bind(this);

			GL::texParameter(m_sampler, GL::TexMinFilter, mipmapped()
				? (smooth() ? GL::LinearMipmapLinear : GL::NearestMipmapLinear)
				: (smooth() ? GL::Linear : GL::Nearest)
			);

			GL::texParameter(m_sampler, GL::TexMinFilter, mipmapped()
				? (smooth() ? GL::LinearMipmapLinear : GL::NearestMipmapLinear)
				: (smooth() ? GL::Linear : GL::Nearest)
			);

			Texture::bind(nullptr);
		}
		return false;
	}

	bool Texture::set_mipmapped(bool value)
	{
		m_flags = value
			? (m_flags & ~TextureFlags_Mipmapped)
			: (m_flags | TextureFlags_Mipmapped);

		if (m_handle)
		{
			Texture::bind(this);

			if (mipmapped()) { GL::generateMipmap(m_sampler); }

			GL::texParameter(m_sampler, GL::TexMagFilter,
				(smooth() ? GL::Linear : GL::Nearest)
			);

			Texture::bind(nullptr);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}