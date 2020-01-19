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

	std::optional<directory_t> FS::read_dir(path_t const & path)
	{
		if (::DIR * dir{ ::opendir(path.string().c_str()) })
		{
			directory_t temp{};
			while (::dirent * ent{ ::readdir(dir) })
			{
				temp.at(([type = ent->d_type]() {
					switch (type)
					{
					case DT_DIR	: return '/';
					case DT_REG	: return ' ';
					case DT_LNK	: return '@';
					default		: return '*';
					}
				})()).emplace_back(path_t{ ent->d_name });
			}
			return std::make_optional(std::move(temp));
		}
		return std::nullopt;
	}

	std::optional<file_t> FS::read_file(path_t const & path)
	{
		if (std::ifstream in{ path, std::ios_base::binary })
		{
			file_t temp{};
			in.seekg(0, std::ios_base::end);
			std::streamsize size;
			if ((size = in.tellg()) > 0)
			{
				in.seekg(0, std::ios_base::beg);
				temp.resize(static_cast<size_t>(size));
				in.read(&temp[0], size);
			}
			in.close();
			return std::make_optional(std::move(temp));
		}
		return std::nullopt;
	}

	pmr::string FS::get_file_contents(path_t const & path)
	{
		if (auto const o{ FS::read_file(path.string()) })
		{
			return pmr::string{ o->cbegin(), o->cend() };
		}
		else
		{
			return pmr::string{};
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}