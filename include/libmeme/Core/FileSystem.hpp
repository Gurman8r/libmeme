#ifndef _ML_FILE_SYSTEM_HPP_
#define _ML_FILE_SYSTEM_HPP_

#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API FS final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		FS() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static Path current_path();

		static void current_path(Path const & value);

		static Path path_to(Path const & value);

		static Path const & root_path();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static std::optional<Dir> read_dir(Path const & path);

		static std::optional<File> read_file(Path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_SYSTEM_HPP_