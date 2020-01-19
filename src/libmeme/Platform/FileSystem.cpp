#include <libmeme/Platform/FileSystem.hpp>

#ifdef ML_OS_WINDOWS
#	include <dirent/include/dirent.h>
#else
#	include <dirent.h>
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static path_t s_root{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	path_t FS::current_path()
	{
		return std::filesystem::current_path();
	}

	void FS::current_path(path_t const & value)
	{
		if (s_root.empty())
		{
			s_root = current_path();
		}
		return std::filesystem::current_path(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	path_t FS::path_to(path_t const & value)
	{
		return path_t{ root_path().string() + "/"s + value.string() };
	}

	path_t const & FS::root_path()
	{
		if (s_root.empty())
		{
			s_root = current_path();
		}
		return s_root;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}