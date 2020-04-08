#ifndef _ML_SCRIPT_MANAGER_HPP_
#define _ML_SCRIPT_MANAGER_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct ML_ENGINE_API script_manager final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		script_manager() noexcept;

		~script_manager() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool startup(fs::path const & name, fs::path const & home);

		bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t do_file(fs::path const & path);

		int32_t do_string(pmr::string const & text);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCRIPT_MANAGER_HPP_