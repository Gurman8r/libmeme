#ifndef _ML_SYSTEM_EXPORT_HPP_
#define _ML_SYSTEM_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

#if defined(ML_API)
#	define ML_SYSTEM_API ML_API
#elif defined(ML_SYSTEM_EXPORTS)
#	define ML_SYSTEM_API ML_API_EXPORT
#else
#	define ML_SYSTEM_API ML_API_IMPORT
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_SYSTEM_EXPORT_HPP_