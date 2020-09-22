#ifndef _ML_CLIENT_EXPORT_HPP_
#define _ML_CLIENT_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

#if defined(ML_API)
#	define ML_CLIENT_API ML_API
#elif defined(ML_APP_EXPORTS)
#	define ML_CLIENT_API ML_API_EXPORT
#else
#	define ML_CLIENT_API ML_API_IMPORT
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_CLIENT_EXPORT_HPP_