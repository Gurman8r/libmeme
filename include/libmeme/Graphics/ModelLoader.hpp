#ifndef _ML_MODEL_HPP_
#define _ML_MODEL_HPP_

#include <libmeme/Graphics/Export.hpp>
#include <libmeme/Graphics/Vertex.hpp>

namespace ml
{
	class ML_GRAPHICS_API model_loader final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static pmr::vector<vertex> read(fs::path const & path);

		static pmr::vector<vertex> read(fs::path const & path, int32_t flags);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_MODEL_HPP_