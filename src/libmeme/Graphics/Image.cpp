#include <libmeme/Graphics/Image.hpp>

#define STBI_MALLOC(s)				_ML memory_manager::allocate(s)
#define STBI_FREE(p)				_ML memory_manager::deallocate(p)
#define STBI_REALLOC(p, s)			_ML memory_manager::reallocate(p, s)
#define STBI_REALLOC_SIZED(p, o, n) _ML memory_manager::reallocate(p, o, n)

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	image const image::default_rgba{ ([]() noexcept
	{
		image temp{ vec2u{ 512, 512 }, 3 };
		for (size_t i = 0, w = temp.width(), h = temp.height(); i < w * h; ++i)
		{
			size_t const y{ i % w }, x{ i / w };

			temp.set_pixel(x, y, ((y < h / 2) && (x < w / 2)) || ((y >= h / 2) && (x >= w / 2))
				? color{ color{ 0.1f }.rgb(), 1.0 }
				: (y >= h / 2) || (x >= w / 2) ? colors::magenta : colors::green
			);
		}
		return temp;
	})() };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool image::load_from_file(fs::path const & path, bool flip_v, size_t req_channels)
	{
		::stbi_set_flip_vertically_on_load(flip_v);

		byte_t * const temp{ ::stbi_load(
			path.string().c_str(),
			reinterpret_cast<int32_t *>(&m_size[0]),
			reinterpret_cast<int32_t *>(&m_size[1]),
			reinterpret_cast<int32_t *>(&m_channels),
			static_cast<int32_t>(req_channels)
		) };
		
		ML_defer{ ::stbi_image_free(temp); };

		if (temp)
		{
			m_pixels.resize(capacity());

			std::memcpy(&m_pixels[0], temp, capacity());
		}

		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}