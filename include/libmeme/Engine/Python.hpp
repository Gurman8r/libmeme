#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Common.hpp>

namespace ml
{
	class ML_ENGINE_API python final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup(fs::path const & name, fs::path const & home);
		
		static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static int32_t do_string(C_string value);

		static int32_t do_string(pmr::string const & value);
		
		static int32_t do_file(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PYTHON_HPP_