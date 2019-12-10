#ifndef _ML_GRAPHICS_EXPORT_HPP_
#define _ML_GRAPHICS_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#if defined(ML_STATIC)
#	define ML_GRAPHICS_API
#elif defined(ML_GRAPHICS_EXPORTS)
#	define ML_GRAPHICS_API ML_API_EXPORT
#else
#	define ML_GRAPHICS_API ML_API_IMPORT
#endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_GRAPHICS_EXPORT_HPP_