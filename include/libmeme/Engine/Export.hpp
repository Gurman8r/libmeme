#ifndef _ML_ENGINE_EXPORT_HPP_
#define _ML_ENGINE_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_API
#	define ML_ENGINE_API ML_API
#else
#	ifdef ML_ENGINE_EXPORTS
#		define ML_ENGINE_API ML_API_EXPORT
#	else
#		define ML_ENGINE_API ML_API_IMPORT
#	endif
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_ENGINE_EXPORT_HPP_