#ifndef _ML_RENDERER_EXPORT_HPP_
#define _ML_RENDERER_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_API
#	define ML_RENDERER_API ML_API
#elif defined(ML_STATIC)
#	define ML_RENDERER_API
#elif defined(ML_RENDERER_EXPORTS)
#	define ML_RENDERER_API ML_API_EXPORT
#else
#	define ML_RENDERER_API ML_API_IMPORT
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RENDERER_EXPORT_HPP_