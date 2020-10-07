#ifndef _ML_SCENE_EXPORT_HPP_
#define _ML_SCENE_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

#if defined(ML_API)
#	define ML_SCENE_API ML_API
#elif defined(ML_SCENE_EXPORTS)
#	define ML_SCENE_API ML_API_EXPORT
#else
#	define ML_SCENE_API ML_API_IMPORT
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_SCENE_EXPORT_HPP_