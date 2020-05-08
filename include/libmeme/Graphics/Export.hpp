#ifndef _ML_GRAPHICS_EXPORT_HPP_
#define _ML_GRAPHICS_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_API
#	define ML_GRAPHICS_API ML_API
#else
#	ifdef ML_GRAPHICS_EXPORTS
#		define ML_GRAPHICS_API ML_API_EXPORT
#	else
#		define ML_GRAPHICS_API ML_API_IMPORT
#	endif
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_GRAPHICS_EXPORT_HPP_