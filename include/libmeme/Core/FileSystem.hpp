#ifndef _ML_FILE_SYSTEM_HPP_
#define _ML_FILE_SYSTEM_HPP_

#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/StringUtility.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <dirent/include/dirent.h>
#else
#	include <dirent.h>
#endif

#if defined(ML_SYSTEM_WINDOWS)
#	define ML_MAX_PATH		260
#	define ML_PATH_DELIM	"\\"
#else
#	define ML_MAX_PATH		255
#	define ML_PATH_DELIM	"/"
#endif

#define ML_FS ::ml::FileSystem::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct FileSystem final : public Singleton<FileSystem>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Directory = typename std::unordered_map<char, std::vector<std::string>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::string const & root() const
		{ 
			return m_root;
		}

		inline std::string pathTo(std::string const & value) const
		{
			return (m_root + ML_PATH_DELIM + value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool setPath(std::string const & value)
		{
			if (dirExists(value))
			{
				std::filesystem::current_path(value.c_str());
				return true;
			}
			return false;
		}

		inline std::string getPath() const
		{
			return std::filesystem::current_path().string();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool getDirContents(std::string const & dirName, std::vector<char> & value) const
		{
			static std::string temp;
			if (getDirContents(dirName, temp))
			{
				value.assign(temp.begin(), temp.end());
				return true;
			}
			value.clear();
			return false;
		}

		inline bool getDirContents(std::string const & dirName, std::string & value) const
		{
			static std::stringstream temp;
			if (getDirContents(dirName, temp))
			{
				value.assign(temp.str());
				return true;
			}
			value.clear();
			return false;
		}

		inline bool getDirContents(std::string const & dirName, std::stringstream & value) const
		{
			value.str(std::string());
			if(DIR * dir{ ::opendir(dirName.c_str()) })
			{
				while (dirent * ent{ ::readdir(dir) })
				{
					switch (ent->d_type)
					{
					case DT_DIR	: value << ent->d_name << "/" << '\n'; break;
					default		: value << ent->d_name << " " << '\n'; break;
					}
				}
				::closedir(dir);
				return true;
			}
			return false;
		}

		inline bool getDirContents(std::string const & dirName, Directory & value) const
		{
			value.clear();
			if (DIR * dir{ ::opendir(dirName.c_str()) })
			{
				while (dirent * ent{ ::readdir(dir) })
				{
					char type;
					switch (ent->d_type)
					{
					case DT_DIR: (type = '/'); break;
					case DT_REG: (type = ' '); break;
					case DT_LNK: (type = '@'); break;
					default:	 (type = '*'); break;
					}
					value[type].push_back(ent->d_name);
				}
				::closedir(dir);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool getFileContents(std::string const & filename, std::vector<char> & value) const
		{
			value.clear();
			if (std::ifstream in{ filename, std::ios_base::binary })
			{
				in.seekg(0, std::ios_base::end);
				std::streamsize size;
				if ((size = in.tellg()) > 0)
				{
					in.seekg(0, std::ios_base::beg);
					value.resize(static_cast<size_t>(size));
					in.read(&value[0], size);
				}
				in.close();
			}
			return !value.empty();
		}

		inline bool getFileContents(std::string const & filename, std::string & value) const
		{
			std::vector<char> temp{};
			if (getFileContents(filename, temp))
			{
				value.assign(temp.begin(), temp.end());
			}
			return !value.empty();
		}

		inline bool getFileContents(std::string const & filename, std::stringstream & value) const
		{
			std::vector<char> temp{};
			if (getFileContents(filename, temp))
			{
				value.str().assign(temp.begin(), temp.end());
			}
			return !value.str().empty();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dirExists(std::string const & name) const
		{
			struct stat info;
			return 
				(stat(name.c_str(), &info) == EXIT_SUCCESS) && 
				(info.st_mode & S_IFDIR);
		}

		inline bool fileExists(std::string const & filename) const
		{
			return (bool)(std::ifstream(filename));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		inline std::string getFileType(std::string const & value) const
		{
			size_t i;
			return
				((i = value.find_last_of('.')) != std::string::npos) ||
				((i = value.find_last_of('/')) != std::string::npos) ||
				((i = value.find_last_of('\\')) != std::string::npos)
				? std::string(value.substr(i + 1, value.size() - i - 1))
				: value;
		}

		inline std::string getFileName(std::string const & value) const
		{
			size_t i;
			return
				((i = value.find_last_of('/')) != std::string::npos) ||
				((i = value.find_last_of('\\')) != std::string::npos)
				? std::string(value.substr(i + 1, value.size() - i - 1))
				: value;
		}

		inline std::string getFilePath(std::string const & value) const
		{
			size_t i;
			return (
				((i = value.find_last_of('/')) != std::string::npos) ||
				((i = value.find_last_of('\\')) != std::string::npos)
				? std::string(value.substr(0, i))
				: value
			);
		}

		inline size_t getFileSize(std::string const & value) const
		{
			std::ifstream stream;
			return (stream = std::ifstream(value, std::ifstream::ate | std::ifstream::binary))
				? (size_t)stream.tellg()
				: 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend Singleton<FileSystem>;
		
		FileSystem() : m_root { getPath() } {}
		
		~FileSystem() {}

		std::string m_root;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_SYSTEM_HPP_