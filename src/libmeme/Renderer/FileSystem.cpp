#include <libmeme/Core/FileSystem.hpp>

#ifdef ML_SYSTEM_WINDOWS
#	include <dirent/include/dirent.h>
#else
#	include <dirent.h>
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	path_t FS::current_path()
	{
		return std::filesystem::current_path();
	}

	void FS::current_path(path_t const & value)
	{
		return std::filesystem::current_path(value);
	}

	path_t FS::path_to(path_t const & value)
	{
		return path_t{ root_path().string() + '/' + value.string() };
	}

	path_t const & FS::root_path()
	{
		static path_t const temp{ std::filesystem::current_path() };
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	std::optional<dir_t> FS::read_dir(path_t const & path)
	{
		if (::DIR * dir{ ::opendir(path.string().c_str()) })
		{
			dir_t temp{};
			
			while (::dirent * ent{ ::readdir(dir) })
			{
				temp[([ent]() {
					switch (ent->d_type)
					{
					case DT_DIR	: return '/';
					case DT_REG	: return ' ';
					case DT_LNK	: return '@';
					default		: return '*';
					}
				})()].push_back(path_t{ ent->d_name });
			}
			
			return std::make_optional(temp);
		}
		return std::nullopt;
	}

	std::optional<file_t> FS::read_file(path_t const & path)
	{
		if (std::ifstream in{ path, std::ios_base::binary })
		{
			file_t temp{};

			in.seekg(0, std::ios_base::beg);

			if (std::streamsize size{ in.tellg() }; size > 0)
			{
				in.seekg(0, std::ios_base::beg);

				temp.resize(static_cast<size_t>(size));

				in.read(&temp[0], size);
			}

			in.close();

			return std::make_optional(temp);
		}
		return std::nullopt;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}