#ifndef _ML_STYLE_LOADER_HPP_
#define _ML_STYLE_LOADER_HPP_

#include <libmeme/Editor/Export.hpp>
#include <libmeme/Core/JSON.hpp>

namespace ml
{
	class ML_EDITOR_API style_loader final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool load_from_file(filesystem::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_STYLE_LOADER_HPP_