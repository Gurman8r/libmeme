#ifndef _ML_WINDOW_EXPORT_HPP_
#define _ML_WINDOW_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#if defined(ML_STATIC)
#	define ML_PLATFORM_API
#elif defined(ML_PLATFORM_EXPORTS)
#	define ML_PLATFORM_API ML_API_EXPORT
#else
#	define ML_PLATFORM_API ML_API_IMPORT
#endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_WINDOW_EXPORT_HPP_