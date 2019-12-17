#include <libmeme/Renderer/Image.hpp>
#include <libmeme/Renderer/GL.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline Image generate_default_image()
	{
		Image img { vec2s{ 512, 512 }, 4 };
		for (size_t y = 0; y < img.height(); y++)
		{
			for (size_t x = 0; x < img.width(); x++)
			{
				img.setPixel(x, y,
					(((y < img.height() / 2) && (x < img.width() / 2)) ||
					((y >= img.height() / 2) && (x >= img.width() / 2))
						? Color(Color(0.1f).rgb(), 1.0)
						: (((y >= img.height() / 2) || (x >= img.width() / 2))
							? colors::magenta
							: colors::green
							)));
			}
		}
		return img;
	}

	const Image Image::Default { generate_default_image() };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image::Image()
		: Image { vec2s { NULL } }
	{
	}

	Image::Image(vec2s const & size)
		: Image { size, 4 }
	{
	}

	Image::Image(vec2s const & size, size_t channels) 
		: Image { size, Pixels(), channels }
	{
	}

	Image::Image(vec2s const & size, Pixels const & pixels)
		: Image { size, pixels, 4 }
	{
	}

	Image::Image(vec2s const & size, Pixels const & pixels, size_t channels)
		: m_size { size }
		, m_pixels { pixels }
		, m_channels { channels }
	{
		if (const size_t c { capacity() })
		{
			if (m_pixels.empty() || (m_pixels.size() != c))
			{
				m_pixels.resize(c);
			}
		}
	}

	Image::Image(Path const & filename)
		: Image{ filename, false }
	{
	}

	Image::Image(Path const & filename, bool flip)
		: Image { filename, flip, 0 }
	{
	}

	Image::Image(Path const & filename, bool flip, size_t req_comp)
		: Image {}
	{
		loadFromFile(filename, flip, req_comp);
	}

	Image::Image(Image const & copy)
		: Image {}
	{
		createFromPixels(copy.m_size, copy.m_channels, copy.m_pixels);
	}

	Image::~Image() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Image::swap(Image & other)
	{
		if (this != std::addressof(other))
		{
			m_pixels.swap(other.m_pixels);
			
			std::swap(m_size, other.m_size);
			
			std::swap(m_channels, other.m_channels);
		}
	}

	bool Image::dispose()
	{
		if (!m_pixels.empty()) { Pixels().swap(m_pixels); }

		m_size = { 0, 0 };
		
		m_channels = 0;
		
		return empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool Image::loadFromFile(Path const & filename)
	{
		return loadFromFile(filename, true);
	}

	bool Image::loadFromFile(Path const & filename, bool flip)
	{
		return loadFromFile(filename, flip, 0);
	}

	bool Image::loadFromFile(Path const & filename, bool flip, size_t req_comp)
	{
		::stbi_set_flip_vertically_on_load(flip);

		if (byte_t * data{ ::stbi_load(
			filename.string().c_str(),
			reinterpret_cast<int32_t *>(&m_size[0]),
			reinterpret_cast<int32_t *>(&m_size[1]),
			reinterpret_cast<int32_t *>(&m_channels),
			static_cast<int32_t>(req_comp)
		) })
		{
			createFromPixels({ data, data + capacity() });
			
			::stbi_image_free(data);
			
			return !empty();
		}
		return dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image & Image::createFromColor(vec2s const & size, Color32 const & color)
	{
		return createFromColor(size, channels(), color);
	}

	Image & Image::createFromColor(Color32 const & color)
	{
		return createFromColor(size(), channels(), color);
	}
	
	Image & Image::createFromColor(vec2s const & size, size_t channels, Color32 const & color)
	{
		if (size[0] && size[1] && channels)
		{
			m_size = size;
			m_channels = channels;
			m_pixels.resize(capacity());

			iterator it { begin() };
			while (it != end())
			{
				if (m_channels >= 1) *it++ = color[0];
				if (m_channels >= 2) *it++ = color[1];
				if (m_channels >= 3) *it++ = color[2];
				if (m_channels >= 4) *it++ = color[3];
			}
			return (*this);
		}
		dispose();
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	Image & Image::createFromPixels(vec2s const & size, Pixels const & pixels)
	{
		return createFromPixels(size, m_channels, pixels);
	}

	Image & Image::createFromPixels(Pixels const & pixels)
	{
		return createFromPixels(m_size, m_channels, pixels);
	}
	
	Image & Image::createFromPixels(vec2s const & size, size_t channels, Pixels const & pixels)
	{
		if (!pixels.empty() && (pixels.size() == (size[0] * size[1] * channels)))
		{
			m_size = size;
			m_channels = channels;
			m_pixels = pixels;
			return (*this);
		}
		dispose();
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image & Image::flipHorizontally()
	{
		if (!empty())
		{
			const size_t cols { width() * channels() };
			for (size_t y = 0; y < height(); ++y)
			{
				iterator lhs { begin() + y * cols };
				iterator rhs { begin() + (y + 1) * cols - channels() };
				for (size_t x = 0; x < width() / 2; ++x)
				{
					std::swap_ranges(lhs, lhs + channels(), rhs);
					lhs += channels();
					rhs -= channels();
				}
			}
		}
		return (*this);
	}

	Image & Image::flipVertically()
	{
		if (!empty())
		{
			const size_t cols { width() * channels() };
			iterator top { begin() };
			iterator bot { end() - cols };
			for (size_t y = 0; y < height() / 2; ++y)
			{
				std::swap_ranges(top, top + cols, bot);
				top += cols;
				bot -= cols;
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t Image::getFormat() const
	{
		switch (channels())
		{
		case 1: return GL::Red;
		case 3: return GL::RGB;
		case 4: return GL::RGBA;
		}
		return (GL::Format)0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	std::optional<Color32> Image::getPixel(size_t index) const
	{
		return (index < capacity())
			? std::make_optional(make_color32(
				(m_channels >= 1) ? *((cbegin() + index) + 0) : (byte_t)0,
				(m_channels >= 2) ? *((cbegin() + index) + 1) : (byte_t)0,
				(m_channels >= 3) ? *((cbegin() + index) + 2) : (byte_t)0,
				(m_channels >= 4) ? *((cbegin() + index) + 3) : (byte_t)0
				))
			: std::nullopt;
	}

	std::optional<Color32> Image::getPixel(size_t x, size_t y) const
	{
		return getPixel(x + y * width());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Image::setPixel(size_t index, Color32 const & color)
	{
		if (index < capacity())
		{
			iterator it{ begin() + index };
			if (m_channels >= 1) *it++ = color[0];
			if (m_channels >= 2) *it++ = color[1];
			if (m_channels >= 3) *it++ = color[2];
			if (m_channels >= 4) *it++ = color[3];
			return true;
		}
		return false;
	}

	bool Image::setPixel(size_t x, size_t y, Color32 const & color)
	{
		return setPixel(x + y * width(), color);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}