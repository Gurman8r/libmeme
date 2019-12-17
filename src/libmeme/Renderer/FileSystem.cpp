#include <libmeme/Core/FileSystem.hpp>

#ifdef ML_SYSTEM_WINDOWS
#	include <dirent/include/dirent.h>
#else
#	include <dirent.h>
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Path FS::current_path()
	{
		return std::filesystem::current_path();
	}

	void FS::current_path(Path const & value)
	{
		return std::filesystem::current_path(value);
	}

	Path FS::path_to(Path const & value)
	{
		return Path{ root_path().string() + '/' + value.string() };
	}

	Path const & FS::root_path()
	{
		static Path const temp{ std::filesystem::current_path() };
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	std::optional<Dir> FS::read_dir(Path const & path)
	{
		if (::DIR * dir{ ::opendir(path.string().c_str()) })
		{
			Dir temp{};
			
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
				})()].push_back(ent->d_name);
			}
			
			return std::make_optional(temp);
		}
		return std::nullopt;
	}

	std::optional<File> FS::read_file(Path const & path)
	{
		if (std::ifstream in{ path, std::ios_base::binary })
		{
			File temp{};

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