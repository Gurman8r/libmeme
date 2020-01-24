#ifndef _ML_FILE_SYSTEM_HPP_
#define _ML_FILE_SYSTEM_HPP_

#include <libmeme/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class FS final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline std::optional<pmr::vector<char>> read_file(fs::path const & path)
		{
			if (std::ifstream in{ path, std::ios_base::binary })
			{
				pmr::vector<char> temp{};
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline pmr::string get_file_contents(fs::path const & path)
		{
			if (auto const o{ FS::read_file(path.string()) })
			{
				return pmr::string{ o->cbegin(), o->cend() };
			}
			return {};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_SYSTEM_HPP_