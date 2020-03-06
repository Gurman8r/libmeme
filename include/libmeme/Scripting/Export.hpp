#ifndef _ML_SCRIPTNG_EXPORT_HPP_
#define _ML_SCRIPTNG_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_API
#	define ML_SCRIPTING_API ML_API
#elif defined(ML_SDK_EXPORTS)
#	define ML_SCRIPTING_API ML_API_EXPORT
#else
#	define ML_SCRIPTING_API ML_API_IMPORT
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_SCRIPTNG_EXPORT_HPP_