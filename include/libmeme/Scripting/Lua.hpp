#ifndef _ML_LUA_HPP_
#define _ML_LUA_HPP_

#include <libmeme/Common.hpp>
#include <libmeme/Scripting/Export.hpp>
#include <libmeme/Core/Singleton.hpp>

namespace ml
{
	class ML_SCRIPTING_API ml_lua final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool is_initialized() noexcept;

		static bool initialize();

		static bool finalize();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t do_string(cstring const & value);

		static int32_t do_string(pmr::string const & value);

		static int32_t do_file(filesystem::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_LUA_HPP_