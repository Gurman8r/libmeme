#ifndef _ML_FILE_SYSTEM_HPP_
#define _ML_FILE_SYSTEM_HPP_

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ML_USING path_t = typename std::filesystem::path;

	ML_USING file_t = typename std::vector<char>;

	ML_USING directory_t = typename ds::flat_map<char, std::vector<path_t>>;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PLATFORM_API FS final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FS() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static path_t current_path();

		ML_NODISCARD static void current_path(path_t const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static path_t path_to(path_t const & value);

		ML_NODISCARD static path_t const & root_path();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static std::optional<directory_t> read_dir(path_t const & path);

		ML_NODISCARD static std::optional<file_t> read_file(path_t const & path);

		ML_NODISCARD static std::string get_file_contents(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static path_t s_root;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_SYSTEM_HPP_