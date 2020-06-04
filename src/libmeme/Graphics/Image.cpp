#include <libmeme/Graphics/Image.hpp>

#define STBI_MALLOC(s)				_ML memory::allocate(s)
#define STBI_FREE(p)				_ML memory::deallocate(p)
#define STBI_REALLOC(p, s)			_ML memory::reallocate(p, s)
#define STBI_REALLOC_SIZED(p, o, n) _ML memory::reallocate(p, o, n)

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool image::load_from_file(fs::path const & path, bool flip_v, size_t req_channels)
	{
		::stbi_set_flip_vertically_on_load(flip_v);

		byte_t * const temp{ ::stbi_load(
			path.string().c_str(),
			&m_size[0],
			&m_size[1],
			reinterpret_cast<int32_t *>(&m_channels),
			static_cast<int32_t>(req_channels)
		) };
		
		ML_defer{ ::stbi_image_free(temp); };

		if (temp)
		{
			m_pixels.resize(capacity());
			std::memcpy(&m_pixels[0], temp, capacity());
			return true;
		}
		else
		{
			this->clear();
			return false;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}