#include <libmeme/Renderer/Image.hpp>
#include <libmeme/Renderer/GL.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image const Image::Default{ ([]()
	{
		Image img { vec2u{ 512, 512 }, 3 };
		for (size_t y = 0; y < img.height(); y++)
		{
			for (size_t x = 0; x < img.width(); x++)
			{
				img.set_pixel(x, y,
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
	})() };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image::Image()
		: Image{ allocator_type{} }
	{
	}

	Image::Image(allocator_type const & alloc)
		: m_size	{ 0 }
		, m_channels{ 0 }
		, m_pixels	{ alloc }
	{
	}

	Image::Image(vec2u const & size, allocator_type const & alloc)
		: Image{ size, 4, alloc }
	{
	}

	Image::Image(vec2u const & size, size_t channels, allocator_type const & alloc)
		: Image{ size, channels, pixels_type{}, alloc }
	{
	}

	Image::Image(vec2u const & size, pixels_type const & pixels, allocator_type const & alloc)
		: Image{ size, 4, pixels, alloc }
	{
	}

	Image::Image(vec2u const & size, size_t channels, pixels_type const & pixels, allocator_type const & alloc)
		: m_size	{ size }
		, m_channels{ channels }
		, m_pixels	{ pixels, alloc }
	{
		if (size_t const c{ capacity() }; m_pixels.empty() || (m_pixels.size() != c))
		{
			m_pixels.resize(c);
		}
	}

	Image::Image(path_t const & path, allocator_type const & alloc)
		: Image{ path, false, alloc }
	{
	}

	Image::Image(path_t const & path, bool flip, allocator_type const & alloc)
		: Image{ path, flip, 0, alloc }
	{
	}

	Image::Image(path_t const & path, bool flip, size_t req_comp, allocator_type const & alloc)
		: Image{ alloc }
	{
		load_from_file(path, flip, req_comp);
	}

	Image::Image(Image const & other, allocator_type const & alloc)
		: m_size	{ other.m_size }
		, m_channels{ other.m_channels }
		, m_pixels	{ other.m_pixels }
	{
	}

	Image::Image(Image && other, allocator_type const & alloc) noexcept
		: Image{ alloc }
	{
		swap(std::move(other));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image & Image::operator=(Image const & other)
	{
		Image temp{ other };
		swap(temp);
		return (*this);
	}

	Image & Image::operator=(Image && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void Image::swap(Image & other)
	{
		if (this != std::addressof(other))
		{
			std::swap(m_pixels, other.m_pixels);
			
			std::swap(m_size, other.m_size);
			
			std::swap(m_channels, other.m_channels);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool Image::load_from_file(path_t const & path)
	{
		return load_from_file(path, true);
	}

	bool Image::load_from_file(path_t const & path, bool flip)
	{
		return load_from_file(path, flip, 0);
	}

	bool Image::load_from_file(path_t const & path, bool flip, size_t req_comp)
	{
		::stbi_set_flip_vertically_on_load(flip);

		if (byte_t * data{ ::stbi_load(
			path.string().c_str(),
			reinterpret_cast<int32_t *>(&m_size[0]),
			reinterpret_cast<int32_t *>(&m_size[1]),
			reinterpret_cast<int32_t *>(&m_channels),
			static_cast<int32_t>(req_comp)
		) })
		{
			create_from_pixels({ data, data + capacity() });
			
			::stbi_image_free(data);
			
			return !empty();
		}
		return dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image & Image::create_from_color(vec2u const & size, Color32 const & color)
	{
		return create_from_color(size, channels(), color);
	}

	Image & Image::create_from_color(Color32 const & color)
	{
		return create_from_color(size(), channels(), color);
	}
	
	Image & Image::create_from_color(vec2u const & size, size_t channels, Color32 const & color)
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
	
	Image & Image::create_from_pixels(vec2u const & size, pixels_type const & pixels)
	{
		return create_from_pixels(size, m_channels, pixels);
	}

	Image & Image::create_from_pixels(pixels_type const & pixels)
	{
		return create_from_pixels(m_size, m_channels, pixels);
	}
	
	Image & Image::create_from_pixels(vec2u const & size, size_t channels, pixels_type const & pixels)
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

	bool Image::dispose()
	{
		pixels_type().swap(m_pixels);

		m_size = { 0, 0 };

		m_channels = 0;

		return empty();
	}

	Image & Image::flip_vertically()
	{
		if (!empty())
		{
			size_t const cols { width() * channels() };
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

	Image & Image::flip_horizontally()
	{
		if (!empty())
		{
			size_t const cols { width() * channels() };
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
		case 1	: return GL::Red;
		case 3	: return GL::RGB;
		case 4	:
		default	: return GL::RGBA;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	std::optional<Color32> Image::get_pixel(size_t index) const
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

	std::optional<Color32> Image::get_pixel(size_t x, size_t y) const
	{
		return get_pixel((x + y * m_size[0]) * m_channels);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Image::set_pixel(size_t index, Color32 const & color)
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

	bool Image::set_pixel(size_t x, size_t y, Color32 const & color)
	{
		return set_pixel((x + y * m_size[0]) * m_channels, color);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}