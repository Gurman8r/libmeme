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

	bool image::read_pixels(fs::path const & path, pixels_type & px, vec2i & sz, size_t & ch, int32_t rc, bool fv)
	{
		px.clear();

		stbi_set_flip_vertically_on_load(fv);

		if (byte_t * const temp
		{
			stbi_load(path.string().c_str(), &sz[0], &sz[1], (int32_t *)&ch, rc)
		})
		{
			px = { temp, temp + (sz[0] * sz[1] * ch) };

			stbi_image_free(temp);
		}

		return !px.empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}