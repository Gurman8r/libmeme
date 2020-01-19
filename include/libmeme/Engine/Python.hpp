#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Platform/FileSystem.hpp>

namespace ml
{
	class ML_ENGINE_API Python final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup(path_t const & name, path_t const & home);
		
		static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static int32_t do_string(C_string value);

		static int32_t do_string(pmr::string const & value);
		
		static int32_t do_file(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PYTHON_HPP_