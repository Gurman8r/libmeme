#ifndef _ML_FILE_SYSTEM_HPP_
#define _ML_FILE_SYSTEM_HPP_

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING path_t = typename std::filesystem::path;

	ML_USING file_t = typename pmr::vector<char>;

	ML_USING directory_t = typename ds::flat_map<char, pmr::vector<path_t>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_PLATFORM_API FS final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static path_t current_path();

		ML_NODISCARD static void current_path(path_t const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static path_t path_to(path_t const & value);

		ML_NODISCARD static path_t const & root_path();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline std::optional<file_t> read_file(path_t const & path)
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

		static inline pmr::string get_file_contents(path_t const & path)
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