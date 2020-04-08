#ifndef _ML_PLATFORM_EXPORT_HPP_
#define _ML_PLATFORM_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_API
#	define ML_PLATFORM_API ML_API
#else
#	ifdef ML_PLATFORM_EXPORTS
#		define ML_PLATFORM_API ML_API_EXPORT
#	else
#		define ML_PLATFORM_API ML_API_IMPORT
#	endif
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PLATFORM_EXPORT_HPP_