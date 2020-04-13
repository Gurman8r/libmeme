#ifndef _ML_RENDER_API_HPP_
#define _ML_RENDER_API_HPP_

// WIP

#include <libmeme/Renderer/Export.hpp>

namespace ml
{
	class ML_RENDERER_API render_api
	{
	public:
		static bool is_initialized();

		static bool initialize();

	};
}

#endif // !_ML_RENDER_API_HPP_