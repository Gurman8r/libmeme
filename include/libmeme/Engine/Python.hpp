#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Platform/FileSystem.hpp>

namespace ml
{
	struct ML_ENGINE_API Python final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Python() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup(path_t const & name, path_t const & home);
		
		static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static int32_t do_string(std::string const & value);
		
		static int32_t do_file(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static bool		m_init;
		static path_t	m_name;
		static path_t	m_home;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PYTHON_HPP_