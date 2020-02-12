#ifndef _ML_EDITOR_EXPORT_HPP_
#define _ML_EDITOR_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_API
#	define ML_EDITOR_API ML_API
#elif defined(ML_EDITOR_EXPORTS)
#	define ML_EDITOR_API ML_API_EXPORT
#else
#	define ML_EDITOR_API ML_API_IMPORT
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_EDITOR_EXPORT_HPP_