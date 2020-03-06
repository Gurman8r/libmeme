#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Core/FileUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool material::load_from_file(filesystem::path const & path)
	{
		if (std::ifstream in{ path })
		{
			in.close();

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}