#ifndef _ML_CORE_EXPORT_HPP_
#define _ML_CORE_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

#ifdef ML_API
#	define ML_CORE_API ML_API
#else
#	ifdef ML_CORE_EXPORTS
#		define ML_CORE_API ML_API_EXPORT
#	else
#		define ML_CORE_API ML_API_IMPORT
#	endif
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_CORE_EXPORT_HPP_