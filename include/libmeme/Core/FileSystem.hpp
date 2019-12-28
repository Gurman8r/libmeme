#ifndef _ML_FILE_SYSTEM_HPP_
#define _ML_FILE_SYSTEM_HPP_

#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/StringUtility.hpp>
#include <libmeme/Core/DenseMap.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	ML_USING path_t = typename std::filesystem::path;

	ML_USING file_t = typename std::vector<char>;

	ML_USING directory_t = typename dense::map<char, std::vector<path_t>>;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API FS final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FS() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static path_t current_path();

		static void current_path(path_t const & value);

		static path_t path_to(path_t const & value);

		static path_t const & root_path();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static std::optional<directory_t> read_dir(path_t const & path);

		static std::optional<file_t> read_file(path_t const & path);

		static std::string get_file_contents(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_SYSTEM_HPP_